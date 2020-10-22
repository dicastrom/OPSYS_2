# COP 4610 Project 2 Kernel Module Programming<br>


Project 2 of our Operating Systems Class<br>
Group Members: Diego Castro and Liz Parra (Git: lizparra)<br>
Instructor: Professor An-I Wang<br>

--------------------------------------------------------------------------------------------------<br>

## Part 1-System-call Tracing<br>

### Deliverables: empty.c and part1.c<br>

This part is about tracing system calls. Empty.c is an empty C program and part1.c is a program that has exactly 7 system-calls more than empty.c.<br>

### Usage: <br>
First get both empty.c and part1.c on the same directory of the Virtual Machiene<br>
gcc -o empty.x empty.c<br> 
strace -o empty.trace ./empty.x <br>
gcc -o part1.x part1.c<br>
strace -o part1.trace ./part1.x<br>
Compare the two .trace files and see that the difference are the 7 extra system calls at the end of the file!<br>

--------------------------------------------------------------------------------------------------<br>
## Part 2- Kernel Module<br>

### Deliverables: my_timer.c Makefile<br>

This part is about creating a kernel module that displays the current time (in seconds) after the Unix Epoch (January 1st, 1970).<br>



### Installing and Running: <br>
Assuming you are using a Virtual Machine, you should get the my_timer.c and Makefiles onto the Virtual Machiene.<br>
Go to /usr/src and create a directory called my_timer (might have to sudo mkdir my_timer)<br>
Move my_timer.c and Makefile into /usr/src/my_timer (might have to sudo chmod on the directory)<br>
sudo make<br>
sudo insmod my_timer.ko<br>
Optional: lsmod | grep my_timer (to see if it installed)<br>
sudo chmod 777 /proc/timer<br>
cat /proc/timer<br>

### Removing: <br>
sudo rmmod my_timer<br>
Optional: lsmod | grep my_timer (to see if it installed)<br>
rm Module* *.ko *.o *.mod* modules*<br>

--------------------------------------------------------------------------------------------------<br>

