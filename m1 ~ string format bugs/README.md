# mission1
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission1.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.

The executable program is compiled with: gcc -m32 -no-pie -O0 -o mission1 mission1.c