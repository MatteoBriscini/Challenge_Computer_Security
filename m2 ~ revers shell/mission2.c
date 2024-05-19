#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


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

void muda() {
    puts("Welcome to this muda program.");
    puts("This program does nothing. And it's more muda against dirty hacker tricks!");
    puts("Provide me a string and it will do nothing");
    char buf[180];
    // Read a buffer until the newline character (\x0a in hex)
    gets(buf);
    check(buf);
    return;
}

void main() {
    alarm(120);
    muda();
}