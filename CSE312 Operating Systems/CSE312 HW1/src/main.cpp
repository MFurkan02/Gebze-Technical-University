#include <extra.h>
#include <gdt.h>
#include <syscalls.h>
#include <multitasking.h>
#include <main.h>
//
namespace myos {
    
// Function to generate the Collatz sequence for a given number n.
// The sequence is stored in the provided buffer `buffer`.
void CollatzSequence(int n, int *buffer) {
    int i = 0;  // Initialize index to store the sequence
    buffer[0] = n;  // Store the initial number in the buffer

    // Continue generating the sequence until n becomes 1
    while (n > 1 ) {  // to ensure we don't exceed buffer size
        if (n % 2 == 0) {
            n = n / 2;  // If n is even, divide it by 2
        } else {
            n = (3 * n) + 1;  // If n is odd, multiply by 3 and add 1
        }
        buffer[++i] = n;  // Store the new value in the buffer
    }
}

void CollatzSequenceTesting() {
        int str[64];
        int buff[256];
        int n;
        printf(" --- COLLATZ SEQUENCE --- \n");
        scanfPrompt("Current Sequence is ->  ", &n);
        printf("Collatz("); printDigit(n); printf("):\n");
        for (int i = n; i > 0;--i) {
            CollatzSequence(i, buff);
            printSeq(buff);
        }
        SyscallHandler::sys_exit();
}

void printSeq(int * seq) {
        printDigit(*seq); printf(": ");
        while (*(++seq) > 1) {
            printDigit(*seq); printf(" ");
        }

        printDigit(1); printf("\n");
}

int LongRunningProgram(int number){
        int result = 0;
        for(int i=0;i<number ; i++){
            for(int j=0; j<number ; j++){
                result += i * j ;
            }
        }
        return result;
}

void LongRunningProgramTesting(){
        printf("--- LONG RUNNING PROGRAM ---\n");
        printf("Result of 100 is -> ");
        int result = LongRunningProgram(100);
        printDigit(result);
        printf("\n");
        sleep(5);
        SyscallHandler::sys_exit();
}



int LinearSearch(int * array, int size, int target) 
{
    for (int i = 0; i < size; i++){
        if (array[i] == target){
            return i;
        }
    }
    return -1;
}

void LinearSearchTesting() {
    int array[] = {10, 20, 80, 30, 60, 50, 110, 100, 130, 170};
    int size = sizeof(array) / sizeof(int); // getting the size
    int target;
    printf(" --- LINEAR SEARCH ---\n");
    printf("OUR ARRAY IS -> "); printArr(array, size);
    scanfPrompt("VALUE TO BE FOUND IS -> ", &target);
    int res = LinearSearch(array, size, target); // getting the result of the search
    printf("LINEAR-SEARCH("); 
    printDigit(target); 
    printf("): "); 
    printDigit(res); 
    printf("\n");
    sleep(5);
    SyscallHandler::sys_exit();
}

int BinarySearch(int *arr, int target, int low, int high){
        if (low > high){
            return -1;
        }
        int mid = (high + low) / 2;
        if (arr[mid] == target){
            return mid;
        }
        else if (target < arr[mid]){
            return BinarySearch(arr, target, low, mid - 1);
        }
        else{
            return BinarySearch(arr, target, mid + 1, high);
        }
    }
void BinarySearchTesting(){
        int array[] = {10, 20, 80, 30, 60, 50, 110, 100, 130, 170};
        int size = sizeof(array) / sizeof(int);
        int target;
        printf("--- BINARY SEARCH ---\n");
        printf("OUR ARRAY IS -> "); printArr(array, size);
        // first sort and print the array
        SortForBinary(array,size);
        printf("SORTING THE ARRAY...\nSORTED ->"); printArr(array, size);
        scanfPrompt("OUR TARGET IS -> ", &target);
        int res = BinarySearch(array, target, 0, size - 1);
        printf("Result of binary search for target '");
        printDigit(target);
        printf("' is -> ");
        printDigit(res);
        printf("\n");
        sleep(5);
        SyscallHandler::sys_exit();
}


void lifecyleA(GlobalDescriptorTable *gdt, TaskManager *tm){
        // Loading each of the 2 program(collatz and long running) 3 times.
        Task taskCollatz(gdt, CollatzSequenceTesting, 2, Normal);
        Task taskLongRunningProgram(gdt, LongRunningProgramTesting , 1, Normal);
        for (int i = 0; i < 3; i++){
                tm->AddTask(&taskCollatz);
                tm->AddTask(&taskLongRunningProgram);
        }
}


void lifecyle1(GlobalDescriptorTable *gdt, TaskManager *tm){
        // Loading one of the selected program 10 times. 
        int program_num = 4; // number of program types.
        void (*prog[program_num]) (void) = {LongRunningProgramTesting ,LinearSearchTesting, BinarySearchTesting, CollatzSequenceTesting};
        void (*selection)(void) = prog[rand(0, program_num)]; // selecting one of them.
        Task task;
        // loading the selected program 10 times.
        for (int i = 0; i < 10; ++i) {
            task.InitTask(gdt, selection, 1, 1, Normal);
            tm->AddTask(&task);
        }
}


void lifecyle2(GlobalDescriptorTable *gdt, TaskManager *tm){
        // Loading 2 of the selected program 3 times each. 
        int program_num = 4;
        // to select between the 4 of the programs.
        void (*prog[program_num]) (void) = {LongRunningProgramTesting ,LinearSearchTesting , BinarySearchTesting, CollatzSequenceTesting};

        int selection1 = rand(0, program_num); // selecting the 1st program.
        int selection2 = rand(0, program_num); // selecting the 2nd program.
        while (selection2 == selection1)       // if selection2 is the same numner as selection1
            selection2 = rand(0, program_num); // selecting a new number until it is a different number than selection1.
        Task task;
        for (int i = 0; i < 3; ++i) { // loading the 2 of the selected programs 3 times each. 
            task.InitTask(gdt, prog[selection1], 1, 1, Normal); // 1st one.
            tm->AddTask(&task);
            task.InitTask(gdt, prog[selection2], 1, 1, Normal); // 2nd one.
            tm->AddTask(&task);
        }
}

void lifecyle3(GlobalDescriptorTable *gdt, TaskManager *tm){
        /*
        */
}

}