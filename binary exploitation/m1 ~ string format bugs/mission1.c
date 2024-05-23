#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ART_LENGTH 128

typedef enum { false, true } bool;

// Example art
const char EXAMPLE_ART_1[] = " _._     _,-\'\"\"`-._\n(,-.`._,\'(       |\\`-/|\n    `-.-' \\ )-`( , o o)\n          `-    \\`_`\"\'-  \n";
const char EXAMPLE_ART_2[] = " ,_     _\n |\\\\_,-~/\n / _  _ |    ,--.\n(  @  @ )   / ,-\'\n \\  _T_/-._( (\n /         `. \\\n|         _  \\ |\n \\ \\ ,  /      |\n  || |-_\\__   /\n ((_/`(____,-\'\n";

// Cat Features
const char CAT_FEATURES[][8] = {"/\\_/\\", "( o.o )", "> ^ <"};

// Function prototypes
void view_example_art();
void create_art(char* buffer);
void view_saved_art(char* buffer);
void win();

int main(int argc, char** argv)
{
    // Saved art
    char saved_art[MAX_ART_LENGTH];
    memset(saved_art, 0, MAX_ART_LENGTH);

    // Ignore this, just know it's for your own good
    setvbuf(stdout, NULL, _IONBF, 0);

    bool still_running = true;

    // Print welcome message
    printf("Welcome to baby_ascii_art!\n");\
    printf("Here you can save and view your favourite pieces! :)\n");

    while (still_running)
    {
        // Menu
        printf("1. View example art\n");
        printf("2. Save your own art\n");
        printf("3. View your saved art\n");
        printf("4. Exit\n\n");

        // Get user input
        int choice;
        printf("> ");
        scanf("%d", &choice);
        getchar(); // Clear the newline from the buffer

        switch (choice)
        {
            case 1:
                view_example_art();
                break;
            case 2:
                create_art(saved_art);
                break;
            case 3:
                view_saved_art(saved_art);
                break;
            case 4:
                still_running = false;
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }

    return 0;
}

void view_example_art()
{
    printf("Here are some example pieces of art:\n");

    printf("1.\n%s\n", EXAMPLE_ART_1);
    printf("2.\n%s\n", EXAMPLE_ART_2);

}

void create_art(char* buffer)
{
    if(strlen(buffer) != 0)
    {
        printf("You have already saved some beautiful art, please restart the program if you want to change it.\n");
        return;
    }
    else
    {
        printf("Enter your ASCII art (MAX %d characters):\n", MAX_ART_LENGTH);
        fgets(buffer, MAX_ART_LENGTH, stdin);

        // Check if the ASCII Art corresponds to a cat (only cats are allowed)
        if (strstr(buffer, CAT_FEATURES[0]) == NULL && strstr(buffer, CAT_FEATURES[1]) == NULL && strstr(buffer, CAT_FEATURES[2]) == NULL)
        {
            printf("Sorry, only cat ASCII art is allowed!\n");
            memset(buffer, 0, MAX_ART_LENGTH);
            return;
        }

        printf("Art saved!\n");
    }

}

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

// I like the name of this command, as it is related to cats
// I will include it in the application as an easter egg
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
