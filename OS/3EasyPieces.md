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
