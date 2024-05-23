#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void win()
{
    puts("Please take your pizza");
    system("cat flag");
    exit(0);
}

void pizza_chall() {

    char buf[168];

    puts("You have been caught. If you don't answer correctly, you can't return back.");
    puts("Welcome to the Pizza Pizza challenge!");
    puts("Complete the phrase");
    puts("What a splendid pie! Pizza pizza ____");

    gets(buf);

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

}

int main()
{
    pizza_chall();
    return 0;
}
