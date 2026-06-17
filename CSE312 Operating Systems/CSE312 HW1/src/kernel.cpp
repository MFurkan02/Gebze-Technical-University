#include <common/types.h>
#include <extra.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <syscalls.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <multitasking.h>
#include <main.h>

using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

// Defining a constructor function pointer type
typedef void (*constructor)();

// Declare external constructor symbols provided by the linker script
extern "C" constructor start_ctors;

extern "C" constructor end_ctors;

// Function to call all global constructors
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

void initProcess(GlobalDescriptorTable *gdt, TaskManager *taskManager);

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/) {
    // Create a Global Descriptor Table (GDT)
    GlobalDescriptorTable gdt;

    // Create a Task Manager instance using the GDT
    TaskManager taskManager(&gdt);

    // Create an Interrupt Manager with IRQ offset 0x20, using the GDT and Task Manager
    InterruptManager interrupts(0x20, &gdt, &taskManager);

    // Create a Syscall Handler for handling system calls, using the Task Manager and Interrupt Manager
    SyscallHandler syscalls(&taskManager, &interrupts, 0x80);

    // Create a Process Execution Handler for managing process execution, using the Task Manager and Interrupt Manager
    ProcessExecutionHandler execs(&taskManager, &interrupts, 0x06);

    // Create a driver manager for managing keyboard and mouse drivers
    DriverManager mouse_keyboard_drivers;

    // Initialize a keyboard event handler and driver
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    mouse_keyboard_drivers.AddDriver(&keyboard);

    // Initialize a mouse event handler and driver
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    mouse_keyboard_drivers.AddDriver(&mouse);

    // Activate all drivers for keyboard and mouse
    mouse_keyboard_drivers.ActivateAll();

    // Initialize the init process
    initProcess(&gdt, &taskManager);

    // Activate interrupts
    interrupts.Activate();

    // Infinite loop to keep the kernel running
    while(true);
}

void infinite_loop() 
{
    while (true);
} 

void initProcess(GlobalDescriptorTable *gdt, TaskManager *taskManager) 
{
    printf("Welcome to FurkanOS\n");

    Task taskInit(gdt, infinite_loop, 0, High);
    taskManager->AddTask(&taskInit);


    printf("LIFECYCLE OF PART A\n");

    lifecyleA(gdt, taskManager);


    // I commented the Part B lifecycles so that Part A lifecycle can be seen much clearly.
    
    
    /* should be umcommented to be tried.

    printf("LIFECYCLE 1 OF PART B\n");

    lifecyle1(gdt, taskManager);
    
    */
    
    /* should be umcommented to be tried.

    printf("LIFECYCLE 2 OF PART B\n");

    lifecyle2(gdt, taskManager);

    */

    /* I couldn't do the lifecycle 3 as I stated in my report.

    printf("LIFECYCLE 3 OF PART B\n");

    lifecyle3(gdt, taskManager);
    
    */

}