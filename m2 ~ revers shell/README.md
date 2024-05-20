# mission2
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission2.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.

The executable program is compiled with: gcc -fno-stack-protector -m32 -no-pie -O0 -z execstack -o mission2 mission2.c

## Code review