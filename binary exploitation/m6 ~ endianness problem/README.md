# Mission 6 ~ endianness problem


## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission6.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.


## Compiled options
The executable program is compiled in 64bit, the stack isn't executable and the canary is inactive.
```
gcc -no-pie -g --static -fno-stack-protector -o mission6 mission6.c
```


## Code overview
As suggested this mission is solved using [ROP (Return Oriented Programming)](https://en.wikipedia.org/wiki/Return-oriented_programming) exploitation.
You can find some information by watching this [video](https://www.youtube.com/watch?v=8zRoMAkGYQE).

The idea of this exploitation method is to use assembly instructions already present in the binary to set the registers needed to call the execve(“/bin/sh”, 0, 0) function.
```
puts("This challenge is meant to teach you another exploitation techniques called ROP.");
puts("You can find some info here: https://www.youtube.com/watch?v=8zRoMAkGYQE");
puts("The goal of this challenge is to execute the syscall: execve(\"/bin/sh\\0\", 0, 0).");
puts("Or the syscalls: (1) open(\"flag\\0\", 0), (2) read(3, some_buff, 20) and (3) write(1, same_buff, 20).");
puts("If you want to learn more advanced techniques, you can attend the course of Offensive and Defensive Cybersecurity :)");
```


## Solution
You have to call the function execve(“/bin/sh”, 0, 0); the registers to be set with their respective values are as follows.

	RAX = 59
	RDI = /bin/sh 
	RSI = 0
	RDX = 0

1. Finding all the **gadget** (machine instruction sequences that are already present in the machine's memory) in the binary.
	```
	ROPgadget --binary mission6 --depth 12 > writable/gadget.txt
	```
2. Finding the **useful gadget**
	- 0x0000000000479136 : pop rax ; pop rdx ; pop rbx ; ret
	- reading the code we find the get_present function that allows you to create a gadget and returns the memory address where the newly created gadget is located. Using this function we get the last gadget which allows us to set the missing registers (RDI and RSI) correctly.
		```
		void get_present()
		{
		    char choice;
		    void *addr;
		    int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
		    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
		    off_t offset = 0;
		    int len;
		    int i;

		    // Allocate a memory page
		    addr = (unsigned char *) mmap(NULL, PAGE_SIZE, prot, flags, -1, offset);
		    if (addr == MAP_FAILED) 
		    {
			puts("mmap failed");
			exit(-1);
		    }
		    // Choosing instructions
		    puts("You can chain 4 instructions, each of which can only be used once!");
		    for (i = 0; i < 4; i++)
		    {
			puts("Choose an instruction: ");
			puts("1) RET");
			puts("2) POP RDI");
			puts("3) POP RSI");
			puts("4) POP RDX");
			puts("5) POP RBX");
			puts("6) SYSCALL");
			puts("7) POP RAX");
			puts("8) MOV ?,EAX");
			puts("9) MOV EAX,?");
			puts(" > ");
			choice = get_unsigned_int();
			if (choice < 1 || choice > sizeof(gadget_list))
			{
			    puts("Invalid choice");
			    i--;
			}
			else
			{
			    if (gadget_list[choice-1] != 0)
			    {
				    len = strlen(addr);
				    strcpy(addr+len, gadget_list[choice-1]);
				    gadget_list[choice-1][0] = 0;
				    puts("Instruction inserted!");
			    }
			    else
			    {
				puts("You have already chosen this instruction!");  
				i--;
			    }
			}
		    }
		    // Change the page permissions to read and execute only
		    if (mprotect(addr, PAGE_SIZE, PROT_READ | PROT_EXEC) == -1) {
			puts("mprotect failed!");
			exit(-1);
		    }
		    printf("Gadget created at %p\n", addr);
		}
		```
3. write a python script that allows you to run the exploit using the pwn library
4. execute the exploitation
	```
 	python EXPLOITATION.py ; cat - | env -i PWD=/home/m259542/mission1 SHLVL=0 ./mission3
 	```
