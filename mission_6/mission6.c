#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define BUFFER_DIM 2
#define TMP_BUFFER_DIM 32
#define PAGE_SIZE 0x1000
#define TIMEOUT 60 * 10

typedef struct {
    unsigned long long id;
    unsigned long long value;
    bool is_little;
} endian_value;

// Global variables
bool present_available = true;
char tmp_buff[TMP_BUFFER_DIM];
unsigned int choice;
unsigned int n = 0;
int fd_rand;
char gadget_list[9][3] = {"\xC3","\x5F","\x5E","\x5A","\x5B","\x0F\x05","\x58","\x8E\xF0","\x8C\xF0"};

// Prototypes
void init();
void ROP();
void print_menu();
void add_value(endian_value*);
void print_values(endian_value*);
void get_present();
bool check_duplicated_ID(unsigned long long, endian_value*);
unsigned int get_unsigned_int();
unsigned long long get_unsigned_long_long();
unsigned long long invert_endianness(unsigned long long);
unsigned char get_random_byte();

// Functions
void init()
{
    // Alarm
    alarm(TIMEOUT);
    // Buffers
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
    // Opening /dev/urandom
    fd_rand = open("/dev/urandom", O_RDONLY);
    if (fd_rand < 0)
    {
        puts("Error opening /dev/random");
        exit(-1);
    }
}

unsigned char get_random_byte()
{
    unsigned char c;

    memset(tmp_buff, 0, TMP_BUFFER_DIM);
    read(fd_rand, tmp_buff, 1);
    c = *((unsigned char *) tmp_buff);
    return c;
}

unsigned int get_unsigned_int()
{
    unsigned int num;

    memset(tmp_buff, 0, TMP_BUFFER_DIM);
    read(STDIN_FILENO, tmp_buff, TMP_BUFFER_DIM - 1);
    num = strtoul(tmp_buff, NULL, 10);
    return num;
}

unsigned long long get_unsigned_long_long()
{
    unsigned long long num;

    memset(tmp_buff, 0, TMP_BUFFER_DIM);
    read(STDIN_FILENO, tmp_buff, TMP_BUFFER_DIM - 1);
    num = strtoull(tmp_buff, NULL, 10);
    return num;
}

unsigned long long invert_endianness(unsigned long long val) 
{
    unsigned long long new_val = 0;
    int i;

    for (i = 0; i < 8; i++) {
        new_val = (new_val << 8) | (val & 0xFF);
        val >>= 8;
    }
    return new_val;
}

bool check_duplicated_ID(unsigned long long new_id, endian_value* buff)
{
    int i;

    for (i = 0; i < n; i++)
        if (buff[i].id == new_id)
            return true;
    return false;
}

void print_menu()
{
    puts("Options:");
    puts("1) Add value");
    puts("2) Print values");
    puts("3) Get present");
    puts("4) Quit");
    printf(" > ");
}

void add_value(endian_value *buff)
{
    unsigned char c;

    memset(&buff[n], 0, sizeof(endian_value));
    printf("ID: ");
    do
        buff[n].id = get_unsigned_long_long();
    while (check_duplicated_ID(buff[n].id, buff));
    c = get_random_byte();
    if (c & 1)
    {
        printf("Little endian: ");
        buff[n].is_little = true;
        buff[n].value = get_unsigned_long_long();
    }
    else
    {
        printf("Big endian: ");
        buff[n].is_little = false;
        buff[n].value = invert_endianness(get_unsigned_long_long());
    }
    n++;
}

void print_values(endian_value *buff)
{
    int i;

    for (i = 0; i < n; i++)
        if (buff[i].is_little)
            printf("%.2d) ID: %llu -> Value: %llu\n", i, buff[i].id, buff[i].value);
        else
            printf("%.2d) ID: %llu -> Value: %llu\n", i, buff[i].id, invert_endianness(buff[i].value));
}

void get_present()
{
    char choice;
    void *addr;
    int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    off_t offset = 0;
    int len;
    int i;

    // Allocate a memory page
    addr = (unsigned char *) mmap(NULL, PAGE_SIZE, prot, flags, -1, offset);
    if (addr == MAP_FAILED) 
    {
        puts("mmap failed");
        exit(-1);
    }
    // Choosing instructions
    puts("You can chain 4 instructions, each of which can only be used once!");
    for (i = 0; i < 4; i++)
    {
        puts("Choose an instruction: ");
        puts("1) RET");
        puts("2) POP RDI");
        puts("3) POP RSI");
        puts("4) POP RDX");
        puts("5) POP RBX");
        puts("6) SYSCALL");
        puts("7) POP RAX");
        puts("8) MOV ?,EAX");
        puts("9) MOV EAX,?");
        puts(" > ");
        choice = get_unsigned_int();
        if (choice < 1 || choice > sizeof(gadget_list))
        {
            puts("Invalid choice");
            i--;
        }
        else
        {
            if (gadget_list[choice-1] != 0)
            {
                    len = strlen(addr);
                    strcpy(addr+len, gadget_list[choice-1]);
                    gadget_list[choice-1][0] = 0;
                    puts("Instruction inserted!");
            }
            else
            {
                puts("You have already chosen this instruction!");  
                i--;
            }
        }
    }
    // Change the page permissions to read and execute only
    if (mprotect(addr, PAGE_SIZE, PROT_READ | PROT_EXEC) == -1) {
        puts("mprotect failed!");
        exit(-1);
    }
    printf("Gadget created at %p\n", addr);
}


void ROP()
{
    endian_value buff[BUFFER_DIM];

    while (1)
    {
        print_menu();
        choice = get_unsigned_int();
        switch (choice)
        {
            case 1:
                add_value(buff);
                break;
            case 2:
                print_values(buff);
                break;
            case 3:
                if (present_available)
                {
                    get_present();
                    present_available = false;
                }
                else
                    puts("You have already unwrapped your present!");
                break;
            case 4:
                puts("Bye!");
                break;
            default:
                puts("Invalid option!");
        }
        if (choice == 4)
            break;
    }
}

int main(int argc, char *argv[])
{
    init();
    puts("This challenge is meant to teach you another exploitation techniques called ROP.");
    puts("You can find some info here: https://www.youtube.com/watch?v=8zRoMAkGYQE");
    puts("The goal of this challenge is to execute the syscall: execve(\"/bin/sh\\0\", 0, 0).");
    puts("Or the syscalls: (1) open(\"flag\\0\", 0), (2) read(3, some_buff, 20) and (3) write(1, same_buff, 20).");
    puts("If you want to learn more advanced techniques, you can attend the course of Offensive and Defensive Cybersecurity :)");
    ROP();
}