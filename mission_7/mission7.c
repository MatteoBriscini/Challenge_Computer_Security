#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <x86intrin.h>
#include <sys/mman.h>
#include <fcntl.h>


#define STRING_LEN          0x1000
#define FLAG_SIZE           STRING_LEN
#define MAX_SIZE            STRING_LEN
#define ALIGNMENT           STRING_LEN
#define BOX_SIZE            STRING_LEN
#define BOXES_SIZE          STRING_LEN * 256


typedef struct _perm {
    unsigned int key[MAX_SIZE];
    char cypertext[MAX_SIZE];
    char plaintext[MAX_SIZE];
    char flag[FLAG_SIZE];
} Permutation;

size_t __attribute__((aligned(ALIGNMENT))) plain_len;
char __attribute__((aligned(ALIGNMENT))) check_boxes[BOXES_SIZE];


void init()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    alarm(10);
}

void read_file(char* dest, char* filepath, size_t max_len)
{
    FILE *file;
    size_t bytes_read;
    
    file = fopen(filepath, "rb");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file");
        exit(-1);
    }
    bytes_read = fread(dest, sizeof(unsigned char), max_len, file);
    if (bytes_read == 0 && ferror(file)) {
        fprintf(stderr, "Failed to read file");
        exit(-1);
    }
}

unsigned long measure_access_time(void *addr) {
    unsigned long start, end;
    _mm_mfence();
    start = __rdtsc();
    _mm_mfence();
    *(volatile unsigned char *)addr;
    _mm_mfence();
    end = __rdtsc();
    _mm_mfence();
    return end - start;
}

void welcome()
{
    puts("                                  .-=+***###****+=:.                                                ");
    puts("                              .-*#####+=-::::-=+#####*-.                                            ");
    puts("                           .=*##*=.               .:=*##*-.                                         ");
    puts("                        .:+##+-.                      .:+##+:.                                      ");
    puts("                       .*##+..                          ..=##*.                                     ");
    puts("                     .=##=.                                .+#*-.                                 ..");
    puts("                   .:+#*:                                    :*#+.                           .. .:*=");
    puts("                   :##=.                                      .+##.                          *+..*+.");
    puts("                  :##=.        ....                ....        .=#*:                         :*++*: ");
    puts("                 :*#=.      .-*###*+=-:.      .:=+*###*=.       .+#+.                         :*#-  ");
    puts("                .+#+.      .+##########*-.  :*###########:       .+#=                         -*=.  ");
    puts("                -##.       +############*: .+#############.       :*#:                       .+*....");
    puts("                *#=       .##############- .*#############-       .=#+                       :#++#*-");
    puts("               :#*:        +############*: .+#############:        :*#:               .-    :*#=:.. ");
    puts("               =#+.        .*###########-.  :*###########-         .*#-               -*-..+#+.     ");
    puts("               +#=.         .=*#######+:     .=*#######+:.         .+#=               :*+-*#-       ");
    puts("               *#=.           ..::-:..         ..:--:..             +#+.              .+##+:        ");
    puts("               +#=.                                                 =#+.             .=##=.         ");
    puts("               +#=.                                                 =#+.            :*#*.           ");
    puts("               +#=.          .==..                  .-=.            =#+.          .+##=.            ");
    puts("               +#=.          .=**###*=-::....::=+*##**=.            =#+. ..:-=-:.-###:              ");
    puts("               +#=.              ..:-==+++**+++==-:.                =#+. .+#+=+*###+.               ");
    puts("               +#=.                                                 =#+. =##=  .-#*:.               ");
    puts("               +#=.                                                 =#+.-#+..    :**.               ");
    puts("               +#=.                                                 =#+.:*#+.    .+#.               ");
    puts("     .-=+=-.   +#=.                                                 =#+.=#*:.    :#*.               ");
    puts("  .:##*+++*#*-.+#=.                                                 =#+..=##*==+##+.                ");
    puts(" .+#+.     .**:+#=.                                                 =#+.-*#+==+=-.                  ");
    puts(".=#=.       **:+#=.                                                 =#+. ...                        ");
    puts("-#*##=....+##=.+#=.                                                 =#+.                            ");
    puts("*#*+#**#**#-.  +#=.                                                 =#+.                            ");
    puts("....##=+##-.   +#=.                                                 =#+.                            ");
    puts("        -=.    +#=.                                                 =#+.                            ");
    puts("               +#=.                                                 =#+.                            ");
    puts("               +#=.                                                 =#+.                            ");
    puts("               *#=.                                                 -#*.                            ");
    puts("             .-#*-                                                  .*#=.                           ");
    puts("            .=##=.                                                   .*#*-                          ");
    puts("         .:*##*.                                           .+#-       .-*##*=:                      ");
    puts("      .*###*+:.      .-=.        .=#+.        .=#+.       .-*##+:        .+##*:                     ");
    puts("       :*#*-.      .-*##*:.     .+###+.      .=###*-.    :+#*-+##*=-:::-+*#*-.                      ");
    puts("        .-*##########*--###*++*###+.+##*=::-*##+.-#########=.  .-*#######+:                         ");
    puts("           .-=++++=-.   .:+****+-.   :=**##**=.    .:===-.                                          ");
    puts("");
    puts("The idea of this challenge is to exploit a spectre v1 vulnerability.");
    puts("You can find the paper of the attack here: https://spectreattack.com/spectre.pdf");
    puts("But it's easier to learn about it from different sources (https://www.youtube.com/watch?v=q3-xCvzBjGs).");
    puts("Moreover, since it is not deterministic, you might need to run the exploit several times.");
    puts("Hence, we provided you with a pwntools (https://docs.pwntools.com/en/stable/) example script that you can use to program your exploit :)");
    puts("Good luck!");
}

int main(int argc, char* argv[])
{
    Permutation perm;
    unsigned int key_len, n_chars;

    init();
    welcome();
    memset(&perm, 0, sizeof(Permutation));
    memset(&check_boxes, 0, sizeof(check_boxes));
    read_file(perm.flag, "./flag", FLAG_SIZE);
    // Reading plaintext
    printf("Plaintext: ");
    n_chars = read(STDIN_FILENO, perm.plaintext, MAX_SIZE-1);
    if (n_chars <= 0) {
        fprintf(stderr, "Read failed!");
        exit(-1);
    }
    if (perm.plaintext[n_chars - 1] == '\n')
        perm.plaintext[n_chars - 1] = '\0';
    plain_len = strlen(perm.plaintext);
    // Reading key
    key_len = plain_len;
    for (int i = 0; i < key_len; i++) {
        printf("key[%d]: ", i);
        if (scanf("%u", &perm.key[i]) != 1) {
            fprintf(stderr, "Scanf error!");
            exit(-1);
        }
    }
    if (getenv("DEBUG")) {
        // Flushing checkboxes
        for (int i = 0; i < 256; i++)
            _mm_clflush(&check_boxes[i * BOX_SIZE]);
    }
    // Permutating
    for (int i = 0; i < key_len; i++) {
        _mm_clflush(&plain_len);
        _mm_mfence();
        unsigned int plain_pos = perm.key[i];
        if (plain_pos < plain_len) {
            size_t check_len = strlen(&check_boxes[perm.plaintext[plain_pos] * BOX_SIZE]);
            check_boxes[(perm.plaintext[plain_pos] * BOX_SIZE) + check_len] = perm.plaintext[plain_pos];
            perm.cypertext[i] = perm.plaintext[plain_pos];
        }
    }
    if (getenv("DEBUG")) {
        // Measuring times
        for (int i = 0; i < 256; i++) {
            unsigned long long time_taken = measure_access_time(&check_boxes[i * BOX_SIZE]);
            printf("Access time for index %d: %llu cycles\n", i, time_taken);
        }
    }
    // Checking the correctness of the ciphertext
    for (int i = 0; i < plain_len; i++)
    {
        char c = perm.plaintext[i];
        size_t check_len = strlen(&check_boxes[c * BOX_SIZE]);
        if (check_len == 0) {
            fprintf(stderr, "Encryption failed!");
            exit(-1);            
        } else {
            check_boxes[(c * BOX_SIZE)  + (check_len - 1)] = '\0';
        }
    }
    printf("Ciphertext: %s\n", perm.cypertext);
    printf("Have a good day!");
}