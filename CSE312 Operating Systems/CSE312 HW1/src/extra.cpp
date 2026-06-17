#include <common/types.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <hardwarecommunication/interrupts.h>
#include <extra.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

namespace myos {
    void printf(char* str)
    {
        static common::uint16_t* VideoMemory = (common::uint16_t*)0xb8000;

        static common::uint8_t x=0,y=0;

        for(int i = 0; str[i] != '\0'; ++i)
        {
            switch(str[i])
            {
                case '\n':
                    x = 0;
                    y++;
                    break;
                default:
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                    x++;
                    break;
            }

            if(x >= 80)
            {
                x = 0;
                y++;
            }

            if(y >= 25)
            {
                for(y = 0; y < 25; y++)
                    for(x = 0; x < 80; x++)
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                x = 0;
                y = 0;
            }
        }
    }

void printfHex(common::uint8_t key)
{
        char* foo = "00";
        char* hex = "0123456789ABCDEF";
        foo[0] = hex[(key >> 4) & 0xF];
        foo[1] = hex[key & 0xF];
        printf(foo);
}

int32_t rand(int min, int max) {
    uint64_t counter;
    int32_t num;

    // Read the clock counter using the rdtsc instruction
    asm("rdtsc" : "=A"(counter));

    // Use the clock counter as a source of randomness
    // Apply a linear congruential generator (LCG) formula
    counter = counter * 1103515245 + 12345;

    // Scale down the large number to the desired range  0 to max - min
    num = (int)(counter / 65536) % (max - min);

    // Ensure the number is non-negative
    if (num < 0)
        num += max;

    // Adjust the number to the range  min to max
    return num + min;
}


void sleep(int seconds) {
    long int delay = 100000000; // Define the delay for 1 second

    // Loop to create a delay equivalent to the number of seconds
    for (long int i = 0; i < seconds * delay; ++i)
        i++; // Increment to create a delay (NOP-like operation)
}


int scanf(char *out) {
    char *str;
    int i;

    // Lock I/O operations to prevent interrupts during input
    InterruptManager::setIOLock(true);

    // Read the whole line until a newline character is encountered
    while ((str = InterruptManager::stdin.read('\n')) == nullptr);

    // Copy the input string to the output buffer until a newline or null character
    for (i = 0; str[i] != '\0' && str[i] != '\n'; ++i)
        out[i] = str[i];

    // Null-terminate the output string
    out[i] = '\0';

    // Unlock I/O operations
    InterruptManager::setIOLock(false);

    // Return the number of characters read
    return i;
}


int scanfPrompt(char *prompt, int *out) {
    int bf;
    char buff[128];

    // Print the prompt message
    printf(prompt);

    // Read the input into the buffer
    bf = scanf(buff);

    // Convert the input to an integer, retry if the format is invalid
    while (atoi(buff, out) == -1) {
        printf("Invalid format error\n");
        printf(prompt);
        bf = scanf(buff);
    }

    // Return the number of characters read
    return bf;
}


    

int atoi(char *str, int *out){
        char *start = str;
        int sign;
        int num = 0;
        
        // sign check
        if ((sign = (str[0] == '-' || str[0] == '+')))
            ++str;

        while (*str != '\0') {
            if (*str < '0' || '9' < *str)
                return -1;
            num =  num * 10 + (*str - '0');
            ++str; 
        }
        *out = sign ? (((start[0] == '-') ? -1 : 1) * num) : (num);
        return 0;
}

void printDigit(int digit){
        char buff[256];
        int n; 
        int i;
        
        // check if the digit is positive or negative
        if (digit < 0) {
            digit *= -1;
            buff[0] = '-';
            i = n = 1;
        }
        else {
            i = n = 0;
        }

        do {
            buff[n] = '0' + (digit % 10);
            digit /= 10;
            ++n;
        } while (digit > 0);

        buff[n] = '\0';
        
        while (i < n / 2) {
            int temp = buff[i];
            buff[i] = buff[n - i - 1];
            buff[n - i - 1] = temp;
            ++i;        
        }
        printf((char *) buff);
    }

void printArr(int *arr, int size){
    for (int i = 0; i < size; ++i) {
            printDigit(arr[i]); 
            printf(" ");
        }
        printf("\n");
}

void swap(int* a, int* b) {
        int temp = *a;
        *a = *b;
        *b = temp;
}

void SortForBinary(int * array, int size){
        for(int i=0;i<size-1;i++){
            for(int j=i+1;j<size;j++){
                if(array[j] < array[i])
                    swap(&array[j],&array[i]);
            }
        }
}

}