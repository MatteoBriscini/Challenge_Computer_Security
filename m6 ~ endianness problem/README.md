# mission6 ~ 
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission6.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.

The executable program is compiled with: gcc -no-pie -g --static -fno-stack-protector -o mission6 mission6.c