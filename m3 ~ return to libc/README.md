# Mission 3 ~ return to libc
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission3.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.


## Compiled options
The executable program is compiled in 32bit, the stack isn't executable and the canary is inactive.
```
gcc -m32 -no-pie -fno-pie -fno-stack-protector -O0 -o mission3 mission3.c
```


## Code overview
Looking at the source code you can see that there are assembly instructions; these instructions allow you to call a function. Our goal is to call the *system(“/bin/sh”)* function, which allows us to run a shell with the same privileges as the *mission3* binary.
```
asm(
      "movl %0, %%eax\n\t"
      "push %%eax\n\t"
      "call *%1"
      :
      : "r" (mess_addr), "r" (addr)
  );
```

## Solution
Reading the assembly code carefully, we see that the user can enter two parameters: the value to be saved in EAX and the function to be called.

To make the call to *system(“/bin/sh”)*, we need to set *“/bin/sh”* inside EAX and find the address of the *system* function in order to call it.

1. Find the address of the **system** function of the *libc* library
	- open gdb
		```
		gdb mission3
		```
	- put a breakpoint 
		```
		break main
		```
	- run the program 
		```
		run
		```
	- find the system address
		```
		print system
		```
2. Figure out where to save the required parameters
	- **/bin/sh** must be saved in *mess_addr* variable that corresponds to the *message* variable.
	- **system address** must be saved in *addr* variable that corresponds to the *day*, *month* and *year* variable.
		> **note:** the *system address* must be written in decimal 
