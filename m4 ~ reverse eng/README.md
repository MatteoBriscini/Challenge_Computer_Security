# mission4
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission4.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.
## compiled options
The executable program is compiled in 64bit, the stack is not executable and the canary is active.
```
gcc -m32 -no-pie -fno-pie -fno-stack-protector -O0 -o mission3 mission3.c
```
## Code overview



## Solution