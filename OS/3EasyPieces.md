# Intro 
https://github.com/remzi-arpacidusseau/ostep-code/tree/master/intro

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
