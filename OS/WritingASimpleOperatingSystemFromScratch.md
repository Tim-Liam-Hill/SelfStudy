by Mick Blundell

Last 2 bytes of 512 byte boot sector must be 0xaa55 in order to be detected as a boot sector (although this may not always
be needed I assume).

x86 architecture uses little-endian format

"So, in a nutshell, BIOS adds some of its own ISRs to the interrupt vector that
specialise in certain aspects of the computer, for example: interrupt 0x10 cause"


BIOS routines are stored in memory! 
Interupt vector is at the start of memory. 

"It is inconvenient to always have to account for this label--memory offset in your
code, so many assemblers will correct label references during assemblege if you include
the following instruction at the top of your code, telling it exactly where you expect the
code to loaded in memory:
[ org 0 x7c00 ]"