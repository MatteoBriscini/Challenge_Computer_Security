#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


// ANSI color codes for style
#define BLUE "\033[0;34m"
#define YELLOW "\033[0;33m"
#define NORMAL  "\033[0m"
#define CLEAR "\033[H\033[J"


// Global variables
int choice = -1;
int first_swap = -1;
int second_swap = -1;


// Frames from the 90s
const char *frames[] = {
    BLUE"\n\n\n"
    "           ⠀⠀⠀⠀⠀⢀⣀⣤⣤⣄⣀⠀⠀⠀           \n"
    "       ⠀⠀⠀⠀⠀⠀⢠⣶⠟⣉⣤⣢⣄⡪⢝⢦⡀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⢰⡿⢁⣾⠟⠉⠉⠉⠹⣧⣃⢳⡀⠀⠀⠀      \n"
    "       ⠀⠀⠀⢀⣀⣼⡏⣼⠃⠀⠀⠀⠀⠀⢹⣏⣸⡅⠀⠀⠀      \n"
    "       ⠀⢀⣴⡿⠿⣿⠃⣿⠀⠀⠀⠀⠀⠀⣸⣷⣿⣶⣄⠀⠀      \n"
    "       ⠠⠞⠁⠀⢠⣿⠌⣿⠀⠀⠀⠀⠀⠀⣿⡇⣿⠛⠛⠿⣄      \n"
    NORMAL"       ⠀⠀⢀⣠⠾⠿⠾⣷⡀⠀⠀⠀⡠⢶⠛⠹⠿⢶⠀      \n"
    "       ⠀⢠⠋⠀⢀⣁⡀⠘⠙⣦⡀⠘⠈⠀⣠⣤⡀⠀⠻⣦⠀      \n"
    "       ⠀⢀⠀⠀⢾⣿⣿⠀⠀⢘⣧⠇⡀⠘⢿⣿⠏⠀⠀⡿⠀      \n"
    "       ⠀⠈⢧⡀⠈⣉⡁⠀⣤⡞⠀⠘⢢⣀⡄⠀⢠⣠⠾⠃⠀      \n"
    "       ⠀⠀⠀⠉⣷⡖⣶⡛⠉⠀⠀⠀⠀⣿⡏⣿⠋⠁⠀⠀⠀      \n"
    BLUE"       ⠀⠀⠀⠀⢻⡇⣽⢺⣱⡄⠀⠀⠀⣿⢇⡏⠀⠀⣰⡖⣦      \n"
    "       ⠀⠀⠀⠀⣿⡇⣿⢻⠸⡇⠀⠀⠀⣿⢰⡏⢀⣾⢳⡾⠉      \n"
    "       ⠀⠀⠀⠀⣿⡄⡿⣿⠘⡁⠀⠀⠐⣿⢸⡇⣾⢇⡿⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠐⣟⣧⢰⠀⠀⠀⢸⣿⢺⠆⣿⢸⡇⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣟⣿⢸⡇⠀⠀⢸⣇⢿⠆⣿⢸⡅⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣏⣿⡸⡅⠀⠀⣼⢏⣼⠆⣿⢸⠃⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠰⣿⠹⣶⣭⣖⣪⣵⡾⠏⢠⣿⢸⡁⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⢂⡷⠀⠈⠉⠘⠉⠉⠀⠀⠸⣿⢼⡀⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⡍⢿⡀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣼⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠹⣯⡎⡻⢦⣀⣀⣀⣀⡤⠞⣉⣼⠃⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠈⠻⢷⣦⣢⣬⣤⣤⣶⠾⠋⠀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀      \n",
    "\n\n\n"
    "           ⠀⠀⠀⠀⠀⢀⣀⣤⣤⣄⣀⠀⠀⠀           \n"
    "       ⠀⠀⠀⠀⠀⠀⢠⣶⠟⣉⣤⣢⣄⡪⢝⢦⡀⠀⠀⠀⠀       \n"
    "       ⠀⠀⠀⠀⠀⢰⡿⢁⣾⠟⠉⠉⠉⠹⣧⣃⢳⡀⠀⠀⠀      "YELLOW"| \n"BLUE
    "       ⠀⠀⠀⢀⣀⣼⡏⣼⠃⠀⠀⠀⠀⠀⢹⣏⣸⡅⠀⠀⠀      "YELLOW"| \n"BLUE
    "       ⠀⢀⣴⡿⠿⣿⠃⣿⠀⠀⠀⠀⠀⠀⣸⣷⣿⣶⣄⠀⠀      "YELLOW"| \n"BLUE
    "       ⠠⠞⠁⠀⢠⣿⠌⣿⠀⠀⠀⠀⠀⠀⣿⡇⣿⠛⠛⠿⣄      "YELLOW"| \n"BLUE
    NORMAL"       ⠀⠀⢀⣠⠾⠿⠾⣷⡀⠀⠀⠀⡠⢶⠛⠹⠿⢶⠀  "YELLOW"      /\n"NORMAL
    "       ⠀⢠⠋⠀⢀⣁⡀⠘⠙⣦⡀⠘⠈⠀⣠⣤⡀⠀⠻⣦⠀     "YELLOW"/ /\n"NORMAL
    "       ⠀⢀⠀⠀⢾⣿⣿⠀⠀⢘⣧⠇⡀⠘⢿⣿⠏⠀⠀⡿⠀     "YELLOW"\\/\n"NORMAL
    "       ⠀⠈⢧⡀⠈⣉⡁⠀⣤⡞⠀⠘⢢⣀⡄⠀⢠⣠⠾⠃⠀      \n"
    "       ⠀⠀⠀⠉⣷⡖⣶⡛⠉⠀⠀⠀⠀⣿⡏⣿⠋⠁⠀⠀⠀      \n"
    BLUE"       ⠀⠀⠀⠀⢻⡇⣽⢺⣱⡄⠀⠀⠀⣿⢇⡏⠀⠀⣰⡖⣦      \n"
    "       ⠀⠀⠀⠀⣿⡇⣿⢻⠸⡇⠀⠀⠀⣿⢰⡏⢀⣾⢳⡾⠉      \n"
    "       ⠀⠀⠀⠀⣿⡄⡿⣿⠘⡁⠀⠀⠐⣿⢸⡇⣾⢇⡿⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠐⣟⣧⢰⠀⠀⠀⢸⣿⢺⠆⣿⢸⡇⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣟⣿⢸⡇⠀⠀⢸⣇⢿⠆⣿⢸⡅⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣏⣿⡸⡅⠀⠀⣼⢏⣼⠆⣿⢸⠃⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠰⣿⠹⣶⣭⣖⣪⣵⡾⠏⢠⣿⢸⡁⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⢂⡷⠀⠈⠉⠘⠉⠉⠀⠀⠸⣿⢼⡀⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⡍⢿⡀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣼⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠹⣯⡎⡻⢦⣀⣀⣀⣀⡤⠞⣉⣼⠃⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠈⠻⢷⣦⣢⣬⣤⣤⣶⠾⠋⠀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀      \n",
    "\n\n\n"
    "           ⠀⠀⠀⠀⠀⢀⣀⣤⣤⣄⣀⠀⠀⠀           \n"
    "       ⠀⠀⠀⠀⠀⠀⢠⣶⠟⣉⣤⣢⣄⡪⢝⢦⡀⠀⠀⠀⠀      "YELLOW" _______\n"BLUE
    "       ⠀⠀⠀⠀⠀⢰⡿⢁⣾⠟⠉⠉⠉⠹⣧⣃⢳⡀⠀⠀⠀      "YELLOW"|       \n"BLUE
    "       ⠀⠀⠀⢀⣀⣼⡏⣼⠃⠀⠀⠀⠀⠀⢹⣏⣸⡅⠀⠀⠀      "YELLOW"|       \n"BLUE
    "       ⠀⢀⣴⡿⠿⣿⠃⣿⠀⠀⠀⠀⠀⠀⣸⣷⣿⣶⣄⠀⠀      "YELLOW"|       \n"BLUE
    "       ⠠⠞⠁⠀⢠⣿⠌⣿⠀⠀⠀⠀⠀⠀⣿⡇⣿⠛⠛⠿⣄      "YELLOW"|       \n"BLUE
    NORMAL"       ⠀⠀⢀⣠⠾⠿⠾⣷⡀⠀⠀⠀⡠⢶⠛⠹⠿⢶⠀  "YELLOW"      / ______\n"NORMAL
    "       ⠀⢠⠋⠀⢀⣁⡀⠘⠙⣦⡀⠘⠈⠀⣠⣤⡀⠀⠻⣦⠀     "YELLOW"/ /                \n"NORMAL
    "       ⠀⢀⠀⠀⢾⣿⣿⠀⠀⢘⣧⠇⡀⠘⢿⣿⠏⠀⠀⡿⠀     "YELLOW"\\/  \n"NORMAL
    "       ⠀⠈⢧⡀⠈⣉⡁⠀⣤⡞⠀⠘⢢⣀⡄⠀⢠⣠⠾⠃⠀      \n"
    "       ⠀⠀⠀⠉⣷⡖⣶⡛⠉⠀⠀⠀⠀⣿⡏⣿⠋⠁⠀⠀⠀      \n"
    BLUE"       ⠀⠀⠀⠀⢻⡇⣽⢺⣱⡄⠀⠀⠀⣿⢇⡏⠀⠀⣰⡖⣦      \n"
    "       ⠀⠀⠀⠀⣿⡇⣿⢻⠸⡇⠀⠀⠀⣿⢰⡏⢀⣾⢳⡾⠉      \n"
    "       ⠀⠀⠀⠀⣿⡄⡿⣿⠘⡁⠀⠀⠐⣿⢸⡇⣾⢇⡿⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠐⣟⣧⢰⠀⠀⠀⢸⣿⢺⠆⣿⢸⡇⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣟⣿⢸⡇⠀⠀⢸⣇⢿⠆⣿⢸⡅⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣏⣿⡸⡅⠀⠀⣼⢏⣼⠆⣿⢸⠃⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠰⣿⠹⣶⣭⣖⣪⣵⡾⠏⢠⣿⢸⡁⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⢂⡷⠀⠈⠉⠘⠉⠉⠀⠀⠸⣿⢼⡀⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⡍⢿⡀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣼⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠹⣯⡎⡻⢦⣀⣀⣀⣀⡤⠞⣉⣼⠃⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠈⠻⢷⣦⣢⣬⣤⣤⣶⠾⠋⠀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀      \n",
    "\n\n\n"
    "           ⠀⠀⠀⠀⠀⢀⣀⣤⣤⣄⣀⠀⠀⠀           \n"
    "       ⠀⠀⠀⠀⠀⠀⢠⣶⠟⣉⣤⣢⣄⡪⢝⢦⡀⠀⠀⠀⠀      "YELLOW" ________________ \n"BLUE
    "       ⠀⠀⠀⠀⠀⢰⡿⢁⣾⠟⠉⠉⠉⠹⣧⣃⢳⡀⠀⠀⠀      "YELLOW"|                |\n"BLUE
    "       ⠀⠀⠀⢀⣀⣼⡏⣼⠃⠀⠀⠀⠀⠀⢹⣏⣸⡅⠀⠀⠀      "YELLOW"|                |\n"BLUE
    "       ⠀⢀⣴⡿⠿⣿⠃⣿⠀⠀⠀⠀⠀⠀⣸⣷⣿⣶⣄⠀⠀      "YELLOW"|                |\n"BLUE
    "       ⠠⠞⠁⠀⢠⣿⠌⣿⠀⠀⠀⠀⠀⠀⣿⡇⣿⠛⠛⠿⣄      "YELLOW"|                |\n"BLUE
    NORMAL"       ⠀⠀⢀⣠⠾⠿⠾⣷⡀⠀⠀⠀⡠⢶⠛⠹⠿⢶⠀  "YELLOW"      / _______________|\n"NORMAL
    "       ⠀⢠⠋⠀⢀⣁⡀⠘⠙⣦⡀⠘⠈⠀⣠⣤⡀⠀⠻⣦⠀     "YELLOW"/ /                \n"NORMAL
    "       ⠀⢀⠀⠀⢾⣿⣿⠀⠀⢘⣧⠇⡀⠘⢿⣿⠏⠀⠀⡿⠀     "YELLOW"\\/  \n"NORMAL
    "       ⠀⠈⢧⡀⠈⣉⡁⠀⣤⡞⠀⠘⢢⣀⡄⠀⢠⣠⠾⠃⠀      \n"
    "       ⠀⠀⠀⠉⣷⡖⣶⡛⠉⠀⠀⠀⠀⣿⡏⣿⠋⠁⠀⠀⠀      \n"
    BLUE"       ⠀⠀⠀⠀⢻⡇⣽⢺⣱⡄⠀⠀⠀⣿⢇⡏⠀⠀⣰⡖⣦      \n"
    "       ⠀⠀⠀⠀⣿⡇⣿⢻⠸⡇⠀⠀⠀⣿⢰⡏⢀⣾⢳⡾⠉      \n"
    "       ⠀⠀⠀⠀⣿⡄⡿⣿⠘⡁⠀⠀⠐⣿⢸⡇⣾⢇⡿⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠐⣟⣧⢰⠀⠀⠀⢸⣿⢺⠆⣿⢸⡇⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣟⣿⢸⡇⠀⠀⢸⣇⢿⠆⣿⢸⡅⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣏⣿⡸⡅⠀⠀⣼⢏⣼⠆⣿⢸⠃⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠰⣿⠹⣶⣭⣖⣪⣵⡾⠏⢠⣿⢸⡁⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⢂⡷⠀⠈⠉⠘⠉⠉⠀⠀⠸⣿⢼⡀⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⡍⢿⡀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣼⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠹⣯⡎⡻⢦⣀⣀⣀⣀⡤⠞⣉⣼⠃⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠈⠻⢷⣦⣢⣬⣤⣤⣶⠾⠋⠀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀      \n",
     "\n\n\n"
    "           ⠀⠀⠀⠀⠀⢀⣀⣤⣤⣄⣀⠀⠀⠀           \n"
    "       ⠀⠀⠀⠀⠀⠀⢠⣶⠟⣉⣤⣢⣄⡪⢝⢦⡀⠀⠀⠀⠀      "YELLOW" ________________ \n"BLUE
    "       ⠀⠀⠀⠀⠀⢰⡿⢁⣾⠟⠉⠉⠉⠹⣧⣃⢳⡀⠀⠀⠀      "YELLOW"|                |\n"BLUE
    "       ⠀⠀⠀⢀⣀⣼⡏⣼⠃⠀⠀⠀⠀⠀⢹⣏⣸⡅⠀⠀⠀      "YELLOW"|   Welcome to   |\n"BLUE
    "       ⠀⢀⣴⡿⠿⣿⠃⣿⠀⠀⠀⠀⠀⠀⣸⣷⣿⣶⣄⠀⠀      "YELLOW"|                |\n"BLUE
    "       ⠠⠞⠁⠀⢠⣿⠌⣿⠀⠀⠀⠀⠀⠀⣿⡇⣿⠛⠛⠿⣄      "YELLOW"|                |\n"BLUE
    NORMAL"       ⠀⠀⢀⣠⠾⠿⠾⣷⡀⠀⠀⠀⡠⢶⠛⠹⠿⢶⠀  "YELLOW"      / _______________|\n"NORMAL
    "       ⠀⢠⠋⠀⢀⣁⡀⠘⠙⣦⡀⠘⠈⠀⣠⣤⡀⠀⠻⣦⠀     "YELLOW"/ /                \n"NORMAL
    "       ⠀⢀⠀⠀⢾⣿⣿⠀⠀⢘⣧⠇⡀⠘⢿⣿⠏⠀⠀⡿⠀     "YELLOW"\\/  \n"NORMAL
    "       ⠀⠈⢧⡀⠈⣉⡁⠀⣤⡞⠀⠘⢢⣀⡄⠀⢠⣠⠾⠃⠀      \n"
    "       ⠀⠀⠀⠉⣷⡖⣶⡛⠉⠀⠀⠀⠀⣿⡏⣿⠋⠁⠀⠀⠀      \n"
    BLUE"       ⠀⠀⠀⠀⢻⡇⣽⢺⣱⡄⠀⠀⠀⣿⢇⡏⠀⠀⣰⡖⣦      \n"
    "       ⠀⠀⠀⠀⣿⡇⣿⢻⠸⡇⠀⠀⠀⣿⢰⡏⢀⣾⢳⡾⠉      \n"
    "       ⠀⠀⠀⠀⣿⡄⡿⣿⠘⡁⠀⠀⠐⣿⢸⡇⣾⢇⡿⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠐⣟⣧⢰⠀⠀⠀⢸⣿⢺⠆⣿⢸⡇⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣟⣿⢸⡇⠀⠀⢸⣇⢿⠆⣿⢸⡅⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣏⣿⡸⡅⠀⠀⣼⢏⣼⠆⣿⢸⠃⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠰⣿⠹⣶⣭⣖⣪⣵⡾⠏⢠⣿⢸⡁⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⢂⡷⠀⠈⠉⠘⠉⠉⠀⠀⠸⣿⢼⡀⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⡍⢿⡀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣼⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠹⣯⡎⡻⢦⣀⣀⣀⣀⡤⠞⣉⣼⠃⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠈⠻⢷⣦⣢⣬⣤⣤⣶⠾⠋⠀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀      \n",
     "\n\n\n"
    "           ⠀⠀⠀⠀⠀⢀⣀⣤⣤⣄⣀⠀⠀⠀           \n"
    "       ⠀⠀⠀⠀⠀⠀⢠⣶⠟⣉⣤⣢⣄⡪⢝⢦⡀⠀⠀⠀⠀      "YELLOW" ________________ \n"BLUE
    "       ⠀⠀⠀⠀⠀⢰⡿⢁⣾⠟⠉⠉⠉⠹⣧⣃⢳⡀⠀⠀⠀      "YELLOW"|                |\n"BLUE
    "       ⠀⠀⠀⢀⣀⣼⡏⣼⠃⠀⠀⠀⠀⠀⢹⣏⣸⡅⠀⠀⠀      "YELLOW"|   Welcome to   |\n"BLUE
    "       ⠀⢀⣴⡿⠿⣿⠃⣿⠀⠀⠀⠀⠀⠀⣸⣷⣿⣶⣄⠀⠀      "YELLOW"|      the       |\n"BLUE
    "       ⠠⠞⠁⠀⢠⣿⠌⣿⠀⠀⠀⠀⠀⠀⣿⡇⣿⠛⠛⠿⣄      "YELLOW"|                |\n"BLUE
    NORMAL"       ⠀⠀⢀⣠⠾⠿⠾⣷⡀⠀⠀⠀⡠⢶⠛⠹⠿⢶⠀  "YELLOW"      / _______________|\n"NORMAL
    "       ⠀⢠⠋⠀⢀⣁⡀⠘⠙⣦⡀⠘⠈⠀⣠⣤⡀⠀⠻⣦⠀     "YELLOW"/ /                \n"NORMAL
    "       ⠀⢀⠀⠀⢾⣿⣿⠀⠀⢘⣧⠇⡀⠘⢿⣿⠏⠀⠀⡿⠀     "YELLOW"\\/  \n"NORMAL
    "       ⠀⠈⢧⡀⠈⣉⡁⠀⣤⡞⠀⠘⢢⣀⡄⠀⢠⣠⠾⠃⠀      \n"
    "       ⠀⠀⠀⠉⣷⡖⣶⡛⠉⠀⠀⠀⠀⣿⡏⣿⠋⠁⠀⠀⠀      \n"
    BLUE"       ⠀⠀⠀⠀⢻⡇⣽⢺⣱⡄⠀⠀⠀⣿⢇⡏⠀⠀⣰⡖⣦      \n"
    "       ⠀⠀⠀⠀⣿⡇⣿⢻⠸⡇⠀⠀⠀⣿⢰⡏⢀⣾⢳⡾⠉      \n"
    "       ⠀⠀⠀⠀⣿⡄⡿⣿⠘⡁⠀⠀⠐⣿⢸⡇⣾⢇⡿⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠐⣟⣧⢰⠀⠀⠀⢸⣿⢺⠆⣿⢸⡇⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣟⣿⢸⡇⠀⠀⢸⣇⢿⠆⣿⢸⡅⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣏⣿⡸⡅⠀⠀⣼⢏⣼⠆⣿⢸⠃⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠰⣿⠹⣶⣭⣖⣪⣵⡾⠏⢠⣿⢸⡁⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⢂⡷⠀⠈⠉⠘⠉⠉⠀⠀⠸⣿⢼⡀⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⡍⢿⡀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣼⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠹⣯⡎⡻⢦⣀⣀⣀⣀⡤⠞⣉⣼⠃⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠈⠻⢷⣦⣢⣬⣤⣤⣶⠾⠋⠀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀      \n",
    "\n\n\n"
    "           ⠀⠀⠀⠀⠀⢀⣀⣤⣤⣄⣀⠀⠀⠀           \n"
    "       ⠀⠀⠀⠀⠀⠀⢠⣶⠟⣉⣤⣢⣄⡪⢝⢦⡀⠀⠀⠀⠀      "YELLOW" ________________ \n"BLUE
    "       ⠀⠀⠀⠀⠀⢰⡿⢁⣾⠟⠉⠉⠉⠹⣧⣃⢳⡀⠀⠀⠀      "YELLOW"|                |\n"BLUE
    "       ⠀⠀⠀⢀⣀⣼⡏⣼⠃⠀⠀⠀⠀⠀⢹⣏⣸⡅⠀⠀⠀      "YELLOW"|   Welcome to   |\n"BLUE
    "       ⠀⢀⣴⡿⠿⣿⠃⣿⠀⠀⠀⠀⠀⠀⣸⣷⣿⣶⣄⠀⠀      "YELLOW"|      the       |\n"BLUE
    "       ⠠⠞⠁⠀⢠⣿⠌⣿⠀⠀⠀⠀⠀⠀⣿⡇⣿⠛⠛⠿⣄      "YELLOW"|     90s!!      |\n"BLUE
    NORMAL"       ⠀⠀⢀⣠⠾⠿⠾⣷⡀⠀⠀⠀⡠⢶⠛⠹⠿⢶⠀  "YELLOW"      / _______________|\n"NORMAL
    "       ⠀⢠⠋⠀⢀⣁⡀⠘⠙⣦⡀⠘⠈⠀⣠⣤⡀⠀⠻⣦⠀     "YELLOW"/ /                \n"NORMAL
    "       ⠀⢀⠀⠀⢾⣿⣿⠀⠀⢘⣧⠇⡀⠘⢿⣿⠏⠀⠀⡿⠀     "YELLOW"\\/  \n"NORMAL
    "       ⠀⠈⢧⡀⠈⣉⡁⠀⣤⡞⠀⠘⢢⣀⡄⠀⢠⣠⠾⠃⠀      \n"
    "       ⠀⠀⠀⠉⣷⡖⣶⡛⠉⠀⠀⠀⠀⣿⡏⣿⠋⠁⠀⠀⠀      \n"
    BLUE"       ⠀⠀⠀⠀⢻⡇⣽⢺⣱⡄⠀⠀⠀⣿⢇⡏⠀⠀⣰⡖⣦      \n"
    "       ⠀⠀⠀⠀⣿⡇⣿⢻⠸⡇⠀⠀⠀⣿⢰⡏⢀⣾⢳⡾⠉      \n"
    "       ⠀⠀⠀⠀⣿⡄⡿⣿⠘⡁⠀⠀⠐⣿⢸⡇⣾⢇⡿⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠐⣟⣧⢰⠀⠀⠀⢸⣿⢺⠆⣿⢸⡇⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣟⣿⢸⡇⠀⠀⢸⣇⢿⠆⣿⢸⡅⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠡⣏⣿⡸⡅⠀⠀⣼⢏⣼⠆⣿⢸⠃⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⠰⣿⠹⣶⣭⣖⣪⣵⡾⠏⢠⣿⢸⡁⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⢂⡷⠀⠈⠉⠘⠉⠉⠀⠀⠸⣿⢼⡀⠀⠀      \n"
    "       ⠀⠀⠀⠀⣿⡍⢿⡀⠀⠀⠀⠀⠀⠀⠀⣸⠇⣼⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠹⣯⡎⡻⢦⣀⣀⣀⣀⡤⠞⣉⣼⠃⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠈⠻⢷⣦⣢⣬⣤⣤⣶⠾⠋⠀⠀⠀⠀⠀      \n"
    "       ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀      \n"
    };


// Shellcode. Are you ready to go back to the '90s?
const char* shellcode = "\x48\x31\xC0\x48\x31\xFF\x48\x31\xD2\x66\xB8\x01\x00\x48\xB9\x6B\x65\x72\x21\x0A\x00\x00\x00\x51\x48\xB9\x27\x39\x30\x73\x20\x68\x61\x63\x51\x48\xB9\x20\x61\x20\x74\x72\x75\x65\x20\x51\x48\xB9\x0A\x0A\x59\x6F\x75\x27\x72\x65\x51\x48\xFF\xC7\x48\x89\xE6\xB2\x1D\x0F\x05\x66\xB8\x01\x00\x48\xC7\xC1\x6C\x3A\x0A\x00\x51\x48\xB9\x6F\x75\x72\x20\x73\x68\x65\x6C\x51\x48\xB9\x48\x65\x72\x65\x27\x73\x20\x79\x51\x48\x89\xE6\xB2\x13\x0F\x05\x48\xC7\xC0\x3B\x00\x00\x00\x48\xB9\x2F\x62\x69\x6E\x2F\x73\x68\x00\x51\x48\x89\xE7\x48\x31\xD2\x48\x31\xF6\x0F\x05";


// Signatures from the '90s
char signatures[40][100] = {
    "From 1990!!",
    "From 1991!!",
    "From 1992!!",
    "From 1993!!",
    "From 1994!!",
    "From 1995!!",
    "From 1996!!",
    "From 1997!!",
    "From 1998!!",
    "From 1999!!",
    "90s!!",
    "Written while using this new software miracle called Photoshop 1.0 (19 Feb 1990)",
    "Sent from the new Macintosh LC (15 Oct 1990)",
    "Sent from Windows 3.0 (22 May 1990)",
    "Trying this new language called Python (20 Feb 1991)",
    "Written using Linux Kernel 0.1 (17 Sept 1991)",
    "Sent from one of the new Apple PowerBook (21 Oct 1991)",
    "Sent from Windows 3.1 (6 Apr 1992)",
    "Reading this new specification called JPEG (18 Sept 1992)",
    "Powered by Intel Pentium (22 Mar 1993)",
    "Watching Jurassic Park (11 Jun 1993)",
    "Using for the first time this new thing called PDF (15 Jun 1993)",
    "Sent from Windows NT 3.1 (27 Jul 1993)",
    "Sent from my Apple Newton MessagePad (3 Aug 1993)",
    "Playing to this new game called Doom (10 Dec 1993)",
    "Written using FreeBSD 1.0 (1 Nov 1993)",
    "Warching this new movie called Pulp Fiction (14 Oct 1994)",
    "Playing with this new thing called PlayStation (3 Dec 1994)",
    "Trying this new thing called Java (23 May 1995)",
    "Trying this new thing called JavaScript (4 Dec 1995)",
    "Sent from Windows 95 (24 Aug 1995)",
    "Watching this new movie called Toy Story (22 Nov 1995)",
    "Suspiciously looking at this new thing USB (15 Jan 1996)",
    "Sent from Windows NT 4.0 (24 Aug 1996)",
    "Using this new thing called DVD (1 Nov 1996)",
    "Trying this magic thing called Wi-Fi (21 Sep 1997)",
    "Sent from Windows 98 (25 Jun 1998)",
    "Observing this new thing called Google (4 Sep 1998)",
    "Written using the iMac G3 (15 Aug 1998)",
    "Watching this new movie called The Matrix (31 Mar 1999)",
    };
    

// Avoid problems with buffers during exploitation
void init()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    alarm(60);
}


// Welcome message
void welcome()
{
    for (int i = 0; i <= 6; i++)
    {
        puts(CLEAR);
        puts(frames[i]);
        usleep(300000);
    }
    puts(YELLOW"\nSomeone told me that the '90s were too far away since they're from the last millennium, but I think they feel closer than ever.");
    puts("\nI will let you write a message to tell me what you think about the 90s.\n\n");
}


// Choice of the signature
void choice_signature(){
    puts("First, choose the signature you want to use!");
    printf("> ");
    scanf("%d", &choice);

    // Clear the input buffer after reading the integer
    while (getchar() != '\n');  // This will consume characters until a newline is found

    if (choice < 0 || choice >= 40)
    {
        puts("Invalid choice");
        exit(1);
    }
}


// Choice of the two bytes to swap
void choice_swap(){
    puts("Now, choose the first of the two bytes you want to swap!");
    printf("> ");
    scanf("%d", &first_swap);

    // Clear the input buffer after reading the integer
    while (getchar() != '\n');  // This will consume characters until a newline is found

    if (first_swap < 0 || first_swap >= 40 + 8)
    {
        puts("Invalid choice");
        exit(1);
    }

    puts("Now, choose the second of the two bytes you want to swap!");
    printf("> ");
    scanf("%d", &second_swap);

    // Clear the input buffer after reading the integer
    while (getchar() != '\n');  // This will consume characters until a newline is found

    if (second_swap < 0 || second_swap >= 40 + 8)
    {
        puts("Invalid choice");
        exit(1);
    }
}


// Efficient implementation of swap between two elements using XOR
void swap(char *a, char *b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}


// This is a gift for you, an already placed shellcode in the binary
// Yes, I know, I'm too generous
// What? You cannot jump to the shellcode? Are you really sure you are a hacker from the '90s?
void gift()
{
    void* addr = (void*)0x2121733000;
    size_t shellcode_len = strlen(shellcode);

    // Allocate executable memory
    void* shellcode_mem = mmap(addr, shellcode_len, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, -1, 0);

    if (shellcode_mem == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    // Add 0x40 bytes of nop sled, just in case
    memset(shellcode_mem, 0x90, 0x40);

    // Copy the shellcode into the allocated memory
    memcpy(shellcode_mem + 0x40, shellcode, 137);
}


void start()
{
    init();
    gift();
    welcome();
    choice_signature();
}


int challenge()
{
    char message[40];

    start();
    
    puts("\n\nThe -fstack-protector flag (canary) was first introduced in GCC 4.1.0 on February 28, 2006.");
    puts("Since this is a retcon, I will allow you to overwrite it with your message. Are you brave enough?");
    puts("\nNow, write your message:");
    printf("> ");
    if (read(0, message, 40 + 8) <= 0) {
        fprintf(stderr, "Read failed!");
        exit(-1);
    }

    //Concat message with signature
    strcat(message, "\n\t");
    strcat(message, signatures[choice]);

    puts("\n\nDamn, I've given you the power to overwrite the canary retcon, but I forgot the second retcon: this challenge is a 64-bit executable!");
    puts("\nMmmmmh, let's add a twist for some fun. Now, you must select two bytes of your message + canary, and I will swap them.");
    puts("\nOh, wait. We're in the '90s, so my resources are quite limited. I'll use a well-known, efficient method based on XOR to make the swap more efficient.");
    choice_swap();
    swap(&message[first_swap], &message[second_swap]);

    puts(message);
    return 0;
}


// main function
int main()
{
    challenge();
    return 0;
}