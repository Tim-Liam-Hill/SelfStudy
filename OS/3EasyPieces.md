# Intro 
https://github.com/remzi-arpacidusseau/ostep-code/tree/master/intro
https://pages.cs.wisc.edu/~remzi/OSTEP/Homework/homework.html
https://pages.cs.wisc.edu/~remzi/OSTEP/

___ 
## Reading for later
 * “Advanced Programming in the UNIX Environment” by W. Richard Stevens, Stephen

## Intro 

Virtualization makes systems easier to use, so the crux of the matter is how does an operating system virtualize resources.

Essentially, we are taking a physical resource and transforming it into an easy to use virtual form. Operating systems provider 
an API (in the form of system calls, a standard library).

Code in this chapter is interesting: it illustrates how program execution is interleaved on a single core system to give the illusion of a multiprocessor
system. 

If you don't have address randomization then memory address space all starts at the same place and multiple processes can have same address values (BUT these
will not point to the same place ie: address space for each process is still separate).

c volatile keyword:
- tells compiler that the value or the variable can change at any time without any action taken by the code the compiler sees nearby
In the context of the program threads, the error occurs regardless of whether or not we specify the variable as volatile. 

In general, all shared global objects must be declared volatile to prevent compiler optimization from introducing unexpected behaviours.

Procedure call - jumps to a procedure and just runs the code (not safe, offering OS routines as a library)

System call 
* now processes run in user space
* we request OS to do something for us
* hardware transfers control to a pre-specified trap handler
* privilege goes to kernel mode 
* once done, OS pases control back via a return-from-trap instruction

Job switching in CPU 
* I/O is slow
* so don't idle while we wait for it, do another task in the meantime

# Virtualization 

## Processes 

Process = instance of a running program 

How do we provide the illusion of many CPUs? 

mechanisms are low-level methods or protocols that implement a needed piece of functionality.

Time sharing has a disk counterpart -> space sharing. 

Policies are algorithms for making some kind of decision within the OS.

Machine state -what a program can read or update when it is running
* memory (address space)
* registers

Policies = high level (answers the question of 'which')
Mechanisms are low level (answer the question of 'how')

Any OS api should have:
* create method
* destroy method 
* wait
* misc control
* status

### Process Creation 

* load code and static data into memory
* modern OSes load things lazily
* allocate memory for run-time stack

Each process by default has three open file descriptors for standard Input, output and error

### Process States
* Running
* Ready
* Blocked


Process list => keeps track of all running programs in the system. 

## Homework

Its an interesting little program here, not actually interfacing with the underlying OS but instead scheduling things (if my brief scan of the code is correct).

1. Should be 100% CPU usage since there is no chance for either process to make an I/O call. -> correct. 
2. What happens depends on the nature of the scheduler. I am assuming this will take 4 + 2 cycles (4 instructions + 1 I/O wait + 1 I/O instruction).

I was wrong, mostly because I didn't know how long an I/O process would be blocked for. 

3. Switching order around will lead to far better utilization of CPU: not 100% since
I/O blocks for 4 instruction cycles and process 2 only does 4, but still, only 1 process 
cycle will be wasted blocking. -> correct

4. Back to original performance in Q2. -> correct
5. Back to q 3 behaviour -> correct
6. The process doing the IO will finish only after all other processes are finished. ->correct
7. There will be better interleaving of processes and less time blocked. ->correct

## Process API

### Fork

Creates an almost exact copy of the running program (doesn't restart execution from the top, it resumes from the same place after the call to fork).
- Child gets a return code of 0 from the fork call
- parent gets the PID of the new child. 

### Wait 

Sometimes we want the parent to wait for the child before carrying on, that is what 
wait does for us. 

### Exec 

Lets you run a prgram that is different from the calling program. 

Weird because: 
* give it the name of an executable and som arguments 
* it loads code and static data from said executable 
* overwrites its current code segment and current stack data with the above 
* heap an dstack and memory space of program get re-initialuzed
* OS then runs that program passing in arguments as the argv

DOES NOT CREATE A NEW PROCESS -> TRANSFORMS EXISTING PROCESS
* a successful call to exec never returns

separating fork from exec is important since:
* it lets us run code after the fork call but before the exec call
* so we can alter the environment the program will be run in.

THE SHELL IS A USER PROGRAM and it basically:
* gives you a prompt 
* creates a fork to run the executable you give
* runs exec in the child
* parent runs wait for the original child to finish

Unix systems start looking for free file descriptors at Zero

### Pipe

output of one process is connected to an in-kernel pipe (queue)
and the input of another process is connected to that same pipe.


TODO: look into [bash source code](https://github.com/bminor/bash/blob/master/shell.c)

### Kill
sends signals to a process
* control-c sends a SIGINT (interupt)
* control-z sends a SIGTSTP (stop)
a process uses ```signal``` system call to catch signals and suspect its normal execution


### Homework 

'the initial (OS) process is called init'

For q2 parent always seems to write to file first. They both write to the same file however. 

Q3: could use sockets but I have done that before so let's try something new - shared memory. 
Something to note about my solution: it seems to result in certain output that is normally only 
called once being called twice. I suppose thats worth looking into. 

TODO: Finish these exercizes (right now I am lazy)

## Direct Execution

How do we obtain high performance and still maintain control in a timesharing CPU virtualization environment?

#### Direct Execution

Basically just bring the executable into memory, put it into process list and run it directly on the CPU
Problems 
* OS isn't in control at all

But it is fast. 

How do we give processes the opportunity to perform I/O and other restricted operations without giving them full control? 

"Thus, the parts of the C library that
make system calls are hand-coded in assembly, as they need to carefully
follow convention in order to process arguments and return values correctly"

To solve prev issue we introduce user and kernel mode. Hardware assists OS by providing different modes of execution.  User mode code runs in a restructed mode 
so couldn't do thing like I/O requests.
How do we give them this functionality? Via system calls 

system calls -> allow kernel to expose certain functionality 

To execute system call, program must execute a special trap instruction
trap basically 
- elevates to kernel mode
- kernel checks if can perform
- if can then we do the work
- once done, return from trap returns control to calling program

OS NEEDS TO KEEP TRACK OF WHICH CODE TO RUN IN KERNEL MODE AND WHICH CODE TO JUMP BACK TO
- if you don't, user can jump around to arbitrary code locations and basically do what they want

Kernel sets up a trap table at boot time 
- machine boot up takes place in kernel mode
- OS tells hardware what code to run when certain exceptional events occur (eg: keyboard interrrupt)
this kind of explains why some config changes (eg: remap keys on keyboard) require a reboot to take effect. 
- also gives location of trap handlers 

We use system-call numbers which map a number to a unique system call
- Prevents users from having to specify locations in memory (which is unsafe, and this is easier anyway)

"Point to ponder: what horrible things could you do to a system if you
could install your own trap table? Could you take over the machine?"

You could completely brick the system by mapping silly calls to certain events(eg, open file on harddrive 
when a key is pressed). You could likely also confuse the system since, if the OS does its checking 
based on the system-call number and not the otherside (actual function being called by hardware) then 
you can exploit the system by calling a system-call nmber you are allowed to do normally but that maps to 
a forbieedn function on hardware (issue you would have is whether or not the OS passes all the 
arguments correctly)

This is 
#### Limited Direct Execution 

2 phases 

* at boot, kernel initializes trap table 
* cpu remembers this location for later 

second 
* when a proces is running and calls a system call
* kernel allocates a node to a process list, allocates memory and issues trap and return-from-trap instructions as necessary. 


but we have a problem, how do we switch between processes? 

if process is running on the CPU then by definition the OS is not running, so it can't interrupt the 
process now can it. How can the OS regain control? 

#### Cooperative Approach

OS trusts processes to run responsibly and processes that run for too long are assumed to periodically give 
up CPU so OS can run other tasks.

Obviously this isn't the most practical approach. What happens if a process doesn't play nice 
or gets stuck in an infinite loop?

#### Non-Cooperative Approach

OS needs help from hardware. 

"A timer device can be programmed to raise an interrupt every
so many milliseconds; when the interrupt is raised, the currently running
process is halted, and a pre-configured interrupt handler in the OS runs.
At this point, the OS has regained control of the CPU, and thus can do
what it pleases: stop the current process, and start a different one."

At boot, OS tells hardware what instruction to run when interrupt is raised, and it 
also starts the timer. Starting timer is obviously a privileged instruction since,
if it wasn't, an offending program could prevent the interrupt from ever taking place. 


One thing that is the same regardless whether process returns via system call or is interrupted
by OS is we need to save and restore its context. Are we going to continue running this process 
and switch to another is important. 

Context switch -OS will save registers onto its kernel stack and restore values of next 
   process from kernel stack. 

#### Concurrency

What if an interrupt occurs while you are already handling an interrupt? 
Concurrency mechanims need to come into play. 

"In an analogous manner, the OS “baby proofs” the CPU, by first (during boot time) setting up the trap handlers and starting an interrupt timer,
and then by only running processes in a restricted mode."

TODO: Homework

## CPU Scheduling 

We need a scheduling metric 

Tturnaround = Tcompletion − Tarrival 

This is a performance metric BUT we also need to consider fairness. 

### FIFO 

Good for when first jobs are fastest, bad when first jobs are longest.
Convoy effect ->  a number of relatively-short potential consumers of a resource get queued behind a heavyweight resource consumer

### Shortest Job First 


This is great, except for when the longest job arrives first (no interrupts).
Also, potential starvation (if I am correct)


### Shortest Time To Completion First (STCF)

We add in the idea of preempting a job -> if current job running will take longer than a new job coming in, it is more
efficient to run the new job (preempt the old job)
If only we always knew how long a job would take to run to completion

### Round Robin  (Response time) 

now we have users that want snappy responses

"We define response time as the time from when the job arrives in a
system to the first time it is scheduled3
. More formally:
Tresponse = Tf irstrun − Tarrival "

instead of running jobs to completion, RR runs a job for a
time slice (sometimes called a scheduling quantum) and then switches
to the next job in the run queue.

TIME OF SLICE MUST BE MULTIPLE OF INTERRUPT TIMER (because that is the level of granularity 
that is enforced, any time following between gets rounded up anyway in practice)

Short time slice -> better response, worse overhead for switching (and so on)

More generally, any policy (such as RR) that is fair, i.e., that evenly divides the CPU among active processes on a small time scale, will perform
poorly on metrics such as turnaround time.

The general technique of amortization is commonly used in systems
when there is a fixed cost to some operation. By incurring that cost less
often (i.e., by performing the operation fewer times), the total cost to the
system is reduced.

### Incorporatingh I/O

To get performance boost, interleave operations so that while we wait on I/O we get something else done.
Split I/O intensive jobs into 'sub-jobs'

But this whole time we have been assuming we know time to completion for each job, which is not necessarily 
the real world case. So we use multi-level feedback queue -> use the past performance to predict the future.

Something I am thinking of is how is prioritization of processes taking place here? In terms of niceness (Linux) 
etc? Current scheduling algorithm approaches here don't discuss this (maybe in next section it will).


### HOMEWORK 

Maybe another time. 

## MultiLevel Feedback.


The Multi-level Feedback Queue (MLFQ) scheduler was first described by Corbato et al.

Aims:
1. To optimize turnaround time 
2. system responsiveness

* we have a number of distinct queues 
* each queue has its own priority
* each job on only one queue 
* can have multiple jobs in a single queue
* higher prio queues jobs first
* RoundRobin for processes inside queue 

How we set priorities is important 

"Rather than giving a fixed priority to each job, MLFQ varies
the priority of a job based on its observed behavior. If, for example, a job
repeatedly relinquishes the CPU while waiting for input from the keyboard, MLFQ will keep its priority high, as this is how an interactive
process might behave. If, instead, a job uses the CPU intensively for long
periods of time, MLFQ will reduce its priority."

### How does priority change

Assumption: we have short jobs where response time is important and long jobs where turnaround time is more important 

The allotment is the amount of time a job can spend at a given priority
level before the scheduler reduces its priority. For simplicity, at first, we
will assume the allotment is equal to a single time slice.

• Rule 3: When a job enters the system, it is placed at the highest
priority (the topmost queue).
• Rule 4a: If a job uses up its allotment while running, its priority is
reduced (i.e., it moves down one queue).
• Rule 4b: If a job gives up the CPU (for example, by performing
an I/O operation) before the allotment is up, it stays at the same
priority level (i.e., its allotment is reset).

Basically, when a new job comes in, we assume it will be a short running job. 

if job does I/O, it will naturally give up CPU time while waiting for said I/O

### Problems 

* starvation 
* maliciously gaming the scheduler 
* programs change behaviour over time

Rule 5: After some time period S, move all the jobs in the system
to the topmost queue.

Choosing S is not trivial.
"As you can imagine, these are often left unmodified,
and thus we are left to hope that the defaults work well in the field. This
tip brought to you by our old OS professor, John Ousterhout, and hence
we call it Ousterhout’s Law."

I think fig 8.4 might be mistaken: on the right, we would expext the long 
running job to move down in prio again since it is using up entire allotment 
when in top of queue 

To stop malicious gaming:

 Rule 4: Once a job uses up its time allotment at a given level (regardless of how many times it has given up the CPU), its priority is
reduced (i.e., it moves down one queue).

### Tuning 

* How many queues?
* how large time slice?
* how often prio boost 

most MLFQ variants allow for varying time-slice length
across different queues. The high-priority queues are usually given short
time slices; they are comprised of interactive jobs, after all

"See Epema’s paper for an excellent overview of such decay-usage
algorithms and their properties [E95]."

We have only really looked in terms of a single processer, what about multi processer?
How does this all fit in with multithreading? 

" some schedulers reserve the highest priority levels
for operating system work"

### Homework

:3 

## Lottery Scheduling (Proportional share)

How can we design a scheduler to share the CPU in a proportional
manner?

### Tickets 

a ticket represents the share of a resource that a process should recieve
Lottery scheduling achieves proportional sharing probabilistically but not 
deterministically 

"Second, random also is lightweight, requiring little state to track alternatives. In a traditional fair-share scheduling algorithm, tracking how
much CPU each process has received requires per-process accounting,
which must be updated after running each process. Doing so randomly
necessitates only the most minimal of per-process state (e.g., the number
of tickets each has)."

SIDE TANGENT ON RANDOMNESS: 
- /dev/random and /dev/urandom are special files that serve as cryptographically secure pseudorandom number generators
urandom is non-blocking so technically less entropy (not an issue unless an attack is found in which case use /dev/random)

Randomness is quite strange 

QUESTION: how do we efficiently conduct the lottery? 

* ticket currency (processes have their own internal ticket currency that is then converted into global currency when comparing)
* ticket transfer (one process gives another its tickets)
* ticket inflation (useful when all processes trust each other to behave)


How do we assign tickets to jobs in the first place? 

### Stride Scheduling 

* each job in the system has a stride which is inversely proportional to the number of tickets it has
* every time a process runs, increment a pass value by its stride to track global progress
* pick process with lowest pass value to schedule next job

stride scheduling gets proportions right at the end of a scheduling cycle

Downsides (compared to lottery):
* need to maintain a global state
* what if new process joins halfway through?

### Linux completely fair scheduler

goal: fairly divide a CPU evenly among all competing processes
BUT: reduce amount of time spent scheduling (since if CPU spends a fair amount of time scheduling, then 
you lose out on efficiency)

virtual runtime = vruntime
* as a process runs, it accumulates vruntime
* it increases propotional to physical time
* when choosing next process, choose one with lowest vruntime

sched_latency: determines how long one process should run before considering a switch (ie: dynamic timeslices)

"CFS divides this value by the number (n) of processes
running on the CPU to determine the time slice for a process, and thus
ensures that over this period of time, CFS will be completely fair."

min_granularity: CFS will never set the time slice to lower than this to avoid switching too much.

Note that CFS utilizes a periodic timer interrupt, which means it can
only make decisions at fixed time intervals.


### Niceness

One smart aspect of the construction of the table of weights above is
that the table preserves CPU proportionality ratios when the difference in
nice values is constant. For example, if process A instead had a nice value
of 5 (not -5), and process B had a nice value of 10 (not 0), CFS would
schedule them in exactly the same manner as before. Run through the
math yourself to see why

### RedBlack trees

CFS uses this to be efficient
Only store running/runnable process in the tree

### I/O and sleeping

if a job has been sleeping and wakes up, set its vruntime to lowest value in tree.
