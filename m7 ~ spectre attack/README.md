# mission7
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission7.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.

The executable program is compiled with: gcc -std=c99 -O3 -Wall -g -o mission7 mission7.c