# COP 4610 Project 2 Kernel Module Programming <br>

Project 2 of our Operating Systems Class<br>
Group Members: Diego Castro and Liz Parra (Git: lizparra)<br>
Instructor: Professor An-I Wang<br>

## Known bugs:
No known bugs.

## Completed:
## Part 1: System-call Tracing- Completed by Diego
This part is about tracing system calls. Empty.c is an empty C program and part1.c is a program that has exactly 7 system-calls more than empty.c. No makefile required for this part.
        
    Files:
        empty.c
        empty.trace
        part1.c
        part1.trace
	Usage: 
	1. First get both empty.c and part1.c on the same directory of the Virtual Machiene
	2. gcc -o empty.x empty.c
	3. strace -o empty.trace ./empty.x
	4. gcc -o part1.x part1.c
	5. strace -o part1.trace ./part1.x
	6. Compare the two .trace files and see that the difference are the 7 extra system calls at the end of the file

 ## Part 2: Kernel Module - Completed by DiegoThis part is about creating a kernel module that displays the current time (in seconds) after the Unix Epoch (January 1st, 1970). The makefile compiles the kernel objects which are then installed by using the insmod command.

    Files:
        Makefile
        my_timer.c
	Usage: 
	- Installing & Running
		1. Assuming you are using a Virtual Machine, you should get the my_timer.c and Makefiles onto the Virtual Machine.
		2. Go to /usr/src and create a directory called my_timer (might have to sudo mkdir my_timer)
		3. Move my_timer.c and Makefile into /usr/src/my_timer (might have to sudo chmod on the directory)
		4. sudo make
		5. sudo insmod my_timer.ko
		6. Optional: lsmod | grep my_timer (to see if it installed)
		7. sudo chmod 777 /proc/timer
		8. cat /proc/timer
	- Removing
		1. sudo rmmod my_timer
		2. Optional: lsmod | grep my_timer (to see if it installed)
		3. rm Module* *.ko .o .mod modules

## Part 3: Elevator Scheduler: Humans v. Zombies - Completed by Liz & Diego
Kernel Module with an Elevator - Liz & Diego
Add System Calls - Liz & Diego
/Proc - Liz
Start elevator - Liz
Stop elevator - Liz
Issue request - Liz
Testing and debugging - Liz & Diego

    Files:
        Makefile
        elevator.c
	Usage: 
    - terminal 1: 
		1. /usr/src/ElevatorModule
		2. sudo make
		3. sudo make insert

    - terminal 2:
		1. ./part3_code
		2. sudo make
		3. sudo make start
		4. ./producer.x 
		5. sudo make issue
		6. sudo make stop 

    - terminal 3:
		1. ./part3_code
		2. sudo make watch_proc
