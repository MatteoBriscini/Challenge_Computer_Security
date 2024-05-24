# Challenge_Computer_Security



Author: 
- Matteo Briscini - [MatteoBriscini](https://github.com/MatteoBriscini)
- Alessandro Conti - [AlessandroConti11](https://github.com/AlessandroConti11)

Tags: `#buffer_overflow`, `#C`, `#computer_security`, `#format_string_bug`, `#polimi`, `#python`, `#ROP`, `#spectre`, `#software_engineering`.



## University

Politecnico di Milano.

Academic Year: 2023/2024.

055633 - Computer Security - UIC 587 - professor Zanero Stefano - security challenge.



## Specification

This GitHub repository describes how to attack some C code and some vulnerable websites in the form of challenges.

The challenges were carried out as optional activities and lasted 2 weeks, 1 for binary exploitation and the other for web exploitation.



## Folder structory

- binary exploitation
  - m0 ~ buffer overflow
  - m1 ~ string format bugs
  - m2 ~ revers shell
  - m3 ~ return to libc
  - m4 ~ reverse eng
  - m5 ~ xor vulnerability
  - m6 ~ endianness problem
  - m7 ~ spectre attack
- web explotation
  - m0 ~ SQL injection
  - m1 ~ cross site scripting (XSS)
  - m2 ~ JWT cookie crafting
  - m3 ~ page walking
  - m4 ~ blind SQL injection
  - m5 ~ cross site scripting (XSS) 2.0
  - m6 ~ blind SQL injection 2.0



## How to run

The command structure to be used to try to exploit all the challenges is as follows:
  ```bash
  python INPUT.py ; cat - | env -i ABSOLUTE_PATH/missionI
  ```
  Where:
  - the **python** command allows you to run scripts to input to exploit challenges
    > **note:** if you do not want to use a script you can use:
      ```bash
      python -c "print 'WHAT_TO_INSERT'"
      ```
  - the **cat -** command waits for further input from the standard input.
  - the **env -i** command unsets the environment variable
  - the **ABSOLUTE_PATH/missionI** command run the executable file


# Final consideration

This GitHub repository was created by the mentioned authors, who did not collaborate directly in carrying out the challenges, which were performed independently. The procedures described in this repository represent the best of the various exploit techniques tested by the authors.

This repository is for academic purposes only, and the authors take no responsibility for uses other than academic ones.
