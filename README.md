# binary exploitation [computer security]


Author: 
- Matteo Briscini - [MatteoBriscini](https://github.com/MatteoBriscini)
- Alessandro Conti - [AlessandroConti11](https://github.com/AlessandroConti11)

Tags: `#buffer_overflow`, `#C`, `#computer_security`, `#format_string_bug`, `#polimi`, `#python`, `#ROP`, `#spectre`, `#software_engineering`.



## University

Politecnico di Milano.

Academic Year: 2023/2024.

055633 - Computer Security - UIC 587 - professor Zanero Stefano - security challenge.



## Specification

This GitHub repository describes how to attack some vulnerable C codes in the form of challenges.
The challenges were carried out as optional activities and lasted for 1 week.



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
