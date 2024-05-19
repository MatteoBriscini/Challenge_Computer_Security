#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>
#include<unistd.h>

void print_car(){
    printf("      __---~~~~--__                      __--~~~~---__    \n"); 
    printf(" '\\---~~~~~~~~\\\\                    //~~~~~~~~---/'       \n");
    printf("   \\/~~~~~~~~~\\||                  ||/~~~~~~~~~\\/         \n");
    printf("               '\\\\                //'                     \n");
    printf("                 '\\\\            //'                       \n");
    printf("                   ||          ||                 \n");
    printf("         ______--~~~~~~~~~~~~~~~~~~--______               \n");
    printf("    ___ // _-~                        ~-_ \\\\ ___          \n");
    printf("   '\\__)\\/~                              ~\\/(__/'         \n");
    printf("    _--'-___                            ___-'--_          \n");
    printf("  /~     '\\ ~~~~~~~~------------~~~~~~~~ /'     ~\\        \n");
    printf(" /|        '\\                          /'        |\\       \n");
    printf("| '\\   ______'\\_         DMC        _/'______   /' |      \n");
    printf("|   '\\_~-_____\\ ~-________________-~ /_____-~_/'   |      \n");
    printf("'.     ~-__________________________________-~     .'      \n");
    printf(" '.      [_______/------|~~|------\\_______]      .'       \n");
    printf("  '\\--___((____)(________\\/________)(____))___--/'        \n");
    printf("   |>>>>>>||                            ||<<<<<<|         \n");
    printf("   '\\<<<<</'                            '\\>>>>>/'         \n\n");    
}

void print_catchphrase(){
    printf("\nNow, if my calculations are correct, when this baby hits 88 miles an hour you're going to see some serious <CENSORED>!\n\n");
}

void print_banner(){

 printf("          _   ___  _   _           __       _                         \n");
 printf("         | | |__ \\| | | |         / _|     | |                        \n");
 printf(" _ __ ___| |_   ) | |_| |__   ___| |_ _   _| |_ _   _ _ __ ___        \n");
 printf("| '__/ _ \\ __| / /| __| '_ \\ / _ \\  _| | | | __| | | | '__/ _ \\    \n");
 printf("| | |  __/ |_ / /_| |_| | | |  __/ | | |_| | |_| |_| | | |  __/       \n");
 printf("|_|  \\___|\\__|____|\\__|_| |_|\\___|_|  \\__,_|\\__|\\__,_|_|  \\___|\n\n");
                                                                                                                           
}

void to_88(){
    int i = 0;
    for (i = 0; i < 89; i++){
        printf("%d.. ", i);
        usleep(100*1000);
    }
    printf("\n");
}

void init(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

int main(){

    uint8_t day;
    uint8_t month;
    uint16_t year;
    char message[100];

    init();

    print_banner();
    print_car();
    
    printf("Marty, we need to go! Turn the DeLorean on!\n");
    printf("Great God, Marty, give me a date!\n");
    printf("> Year: ");
    scanf("%hu", &year);
    printf("> Month: ");
    scanf("%hhu", &month);
    printf("> Day: ");
    scanf("%hhu", &day);

    printf("\nMarty, you got any last words? ");
    scanf("%99s", message);

    print_catchphrase();
    to_88();

    printf("\n%hhu/%hhu/%hu, here we come!\n", day, month, year);

    uint32_t addr = (day << 24) | (month << 16) | year;
    uint32_t mess_addr = &message;

    asm(
        "movl %0, %%eax\n\t"
        "push %%eax\n\t"
        "call *%1"
        :
        : "r" (mess_addr), "r" (addr)
    );

    return 0;
}