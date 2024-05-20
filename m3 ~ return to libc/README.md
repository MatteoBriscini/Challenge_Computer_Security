# mission 3
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission3.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.

The executable program is compiled with: gcc -m32 -no-pie -fno-pie -fno-stack-protector -O0 -o mission3 mission3.c