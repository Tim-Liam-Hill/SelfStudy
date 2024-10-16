1. How large is boot sector?

2. How is a boot sector detected? 

3. What is the BIOS?

"Firstly, let’s think about what we are trying to do here. We’d like to print a character
on the screen but we do not know exactly how to communicate with the screen device,
since there may be many different kinds of screen devices and they may have different
interfaces. This is why we need to use BIOS, since BIOS has already done some auto
detection of the hardware and, evidently by the fact that BIOS earlier printed information
on the screen about self-testing and so on, so can offer us a hand."

4. What endian format does the x86 architecture use? 

5. What is a word?

(term that describes the size of the maximum processing unit of the current mode of the CPU).

6. What is 16-bit real mode? Why does it exist?

7. What is an interrupt? 

Mechanism that allows the CPU to temporarily stop what it is doing to run a higher prio task before resuming the original task.
- Can be raised by sortware instruction or a hardware device 


8. What is an ISR? how does it get set up and what is it used for?

- BIOS sets up a table  at start of memory upon startup
- it is a vector that maps each number to an Interrupt Service Routine
- an ISR is a set of machine instructions that deal with a specific interrupt
- so in a sense, each number in the ISR table represents a specific interrupt

9. How do you find out where the bootloader is loaded into memory? 

It is always at 0x7c00

10. What is the significance of the instruction [ org 0 x7c00 ]?

In assembly, the CPU will treat offsets as though they were from the start of memory but
the bootloader is not loaded at the start of memory. We need to add the offset of where the 
BIOS starts to get the real address of our 'labels'. By using this command, we tell the assembler
that all offsets are relative to 0x7c00 and thus we don't need to constantly add offsets. 

11. What is the stack byte boundaries? 


12. What are the stack base and stack pointer/top?

https://stackoverflow.com/questions/556714/how-does-the-stack-work-in-assembly-language


MADE IT UP TO Question 4 (need to do that)

13. Segment registers: what are they? 

14. What is the MBR?

15. What is a GCC cross compiler? Why do you need one? 

16. What is the difference between an OS and the Kernel?

17. What is a shell? 

18. Describe the following: Real Mode, Unreal Mode, Protected Mode, and Long Mode.