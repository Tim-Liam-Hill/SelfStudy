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


### Shortest Time TO Completion First (STCF)

We add in the idea of preempting a job -> 
