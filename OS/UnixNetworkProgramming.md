(1st Volume Third Edition) 

# Intro

Protocol = aggreement on how programs will communicate

" We must be careful when using TCP because it is a byte-stream protocol
with no record boundaries...Therefore, when reading from a TCP
socket, we always need to code the read in a loop and terminate the loop when either
read returns 0 (i.e., the other end closed the connection) or a value less than 0 (an
error)."

" TCP itself provides no record markers: If an application wants to delineate the
ends of records, it must do so itself and there are a few common ways to accomplish
this."

When an error occurs in a Unix function, the global variable ```errno``` is set to a positive value (success would be -1)
```errno``` is only set if an error occurs. 
sys/errno.h header normally defines the constants for error values. 

3 steps a TCP server uses to prepare a listening descriptor:
* socket
* bind
* listen

there is queuing of client connections for the/a listening descriptor.

QUESTION FOR LATER: so there is a queue of connections to the port when multiple clients try and connect to the same port at the same time??


Three way TCP handshake occurs in the accept call (ie: once it is done then accept returns and stops looping). The accept function 
returns a connected descriptor which is used to communicate with the new client. A new descriptor is returned by 
an accept for each client that connects to the server. 

function ```close``` is what initiates the TCP terminate sequence. 

"If multiple client connections arrive at about the same time, the kernel queues them, up to some limit, and
returns them to accept one at a time."

"The sockets programming interfaces described in this book are interfaces from the
upper three layers (the "application") into the transport layer."

Fun fact about the OSI model:
* top three layers are user space/process
* lower four layers are provided by OS so are more so kernel space

(page 60)
