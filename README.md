# Using-SYSV-Semaphores-in-C


## Description
This was a particularly interesting lab in my system software class. We were tasked to use semaphores to 
try and solve the producer-consumer problem. The lab was particularly difficult becuase our professor was 
insistint on us using SYSV semaphores (conveniently the one with few stackOverflow posts), above a POSIX implementation.




## Producer-Consumer Problem
The producer-consumer problem is a common problem that computer science that attempts to solve the problem
of two process, attempting to seperately read and write from a data buffer stored as a qeue. One such solution 
is the use of system semaphores. You can read more about the at this [wikipedia link](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem).


## Semaphores
Semaphores are special kinds of variables that are controlled by the operating systems. They are mainly used
as a method to allow for smoother inter-process communication. If one process is using a semaphore, then another
process goes to sleep until the other process is ready.

In this program we use semaphores to help solve the producer-consumer problem
