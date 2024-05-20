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

The idea of this exploitation method is to use assembly instructions already present in the binary to set the registers needed to call the execve(“/bin/sh”, 0, 0) function.
```
puts("This challenge is meant to teach you another exploitation techniques called ROP.");
puts("You can find some info here: https://www.youtube.com/watch?v=8zRoMAkGYQE");
puts("The goal of this challenge is to execute the syscall: execve(\"/bin/sh\\0\", 0, 0).");
puts("Or the syscalls: (1) open(\"flag\\0\", 0), (2) read(3, some_buff, 20) and (3) write(1, same_buff, 20).");
puts("If you want to learn more advanced techniques, you can attend the course of Offensive and Defensive Cybersecurity :)");
```

## Solution
You have to call the function execve(“/bin/sh”, 0, 0); to call it you must be to set 

1. Finding all the **gadget** (machine instruction sequences that are already present in the machine's memory) in the binary.
	```
	ROPgadget --binary mission6 --depth 12 > writable/gadget.txt
	```
2. Finding the **useful gadget**
