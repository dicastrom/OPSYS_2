# COP 4610 Project 2 Kernel Module Programming


Project 2 of our Operating Systems Class
Group Members: Diego Castro and Liz Parra (Git: lizparra)
Instructor: Professor An-I Wang

--------------------------------------------------------------------------------------------------

## Part 1-System-call Tracing

### Deliverables: empty.c and part1.c

This part is about tracing system calls. Empty.c is an empty C program and part1.c is a program that has exactly 7 system-calls more than empty.c.

### Usage: 
First get both empty.c and part1.c on the same directory of the Virtual Machiene
gcc -o empty.x empty.c 
strace -o empty.trace ./empty.x 
gcc -o part1.x part1.c
strace -o part1.trace ./part1.x
Compare the two .trace files and see that the difference are the 7 extra system calls at the end of the file!

--------------------------------------------------------------------------------------------------
## Part 2- Kernel Module

### Deliverables: my_timer.c Makefile

This part is about creating a kernel module that displays the current time (in seconds) after the Unix Epoch (January 1st, 1970).



### Installing and Running: 
Assuming you are using a Virtual Machine, you should get the my_timer.c and Makefiles onto the Virtual Machiene.
Go to /usr/src and create a directory called my_timer (might have to sudo mkdir my_timer)
Move my_timer.c and Makefile into /usr/src/my_timer (might have to sudo chmod on the directory)
sudo make
sudo insmod my_timer.ko
Optional: lsmod | grep my_timer (to see if it installed)
sudo chmod 777 /proc/timer
cat /proc/timer

### Removing: 
sudo rmmod my_timer
Optional: lsmod | grep my_timer (to see if it installed)
rm Module* *.ko *.o *.mod* modules*

--------------------------------------------------------------------------------------------------

