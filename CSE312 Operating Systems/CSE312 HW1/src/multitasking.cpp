
#include <multitasking.h>
#include <extra.h>

using namespace myos;
using namespace myos::common;

Task::Task() {
    InitTask(0, 0, -1, -1, TaskPriority::Normal);
}

Task::Task(GlobalDescriptorTable *gdt, void entrypoint(), common::uint32_t ppid, TaskPriority priority)
{
    InitTask(gdt, entrypoint, pid, ppid, priority);
}

Task::~Task()
{
}

void Task::InitTask(GlobalDescriptorTable *gdt, void entrypoint(), common::uint32_t pid, common::uint32_t ppid, TaskPriority priority) {
    // Initialize task parameters
    this->ppid = ppid;         // Set parent process ID
    this->pid = pid;           // Set process ID
    this->priority = priority; // Set task priority
    state = Ready;             // Set initial state to Ready
    waitparent = false;        // Initialize waitparent flag to false

    // Initialize CPU state pointer to the top of the stack minus the size of CPUState
    cpustate = (CPUState*)(stack + STACK_SIZE - sizeof(CPUState));

    // Initialize general-purpose registers to 0
    cpustate->eax = 0;
    cpustate->ebx = 0;
    cpustate->ecx = 0;
    cpustate->edx = 0;

    // Initialize additional general-purpose registers to 0
    cpustate->esi = 0;
    cpustate->edi = 0;
    cpustate->ebp = 0;

    // Set the instruction pointer to the entry point of the task
    cpustate->eip = (uint32_t)entrypoint;

    // Set the code segment selector using the GDT
    cpustate->cs = gdt->CodeSegmentSelector();

    // Set the initial flags register value
    cpustate->eflags = 0x202;  // Enable interrupts (bit 9) and set reserved bits
}


TaskManager::TaskManager(GlobalDescriptorTable *gdt)
{
    this->gdt = gdt;
    Task_counter = 0;
    Active_Task_counter = 0;
    currentTask = -1;
    next_pid = 1;
}

TaskManager::~TaskManager()
{
}

void TaskManager::ControlTask() {
    // If the current task is running, mark it as ready to be scheduled again
    if (tasks[currentTask].state == Running)
        tasks[currentTask].state = Ready;

    do { 
        // Move to the next task in the list
        // Apply Round Robin scheduling: increment currentTask
        // If currentTask exceeds the number of tasks, wrap around using modulo
        if (++currentTask >= Task_counter)
            currentTask %= Task_counter;
        
        // Loop until a task in the ready state is found
        // This ensures we skip any tasks that are not ready to run (blocked or terminated)
    } while (tasks[currentTask].state != Ready);
    
    // Once a ready task is found, mark it as running
    tasks[currentTask].state = Running;
}

void TaskManager::CopyTask(Task* source, Task* destination) {
    // Copy basic task attributes from source to destination
    destination->state = source->state;
    destination->pid = source->pid;
    destination->ppid = source->ppid;
    destination->priority = source->priority;
    destination->waitparent = source->waitparent;

    // Copy the entire stack from source to destination
    for (int i = 0; i < Task::STACK_SIZE; ++i) {
        destination->stack[i] = source->stack[i];
    }

    // Copy the CPU state from source to destination
    *(destination->cpustate) = *(source->cpustate);
}

bool TaskManager::AddTask(Task* task) {
    const int MAX_NUM_TASKS = 256 ;
    // Check if the task limit has been reached
    if (Task_counter >= MAX_NUM_TASKS) {
        return false; // Cannot add more tasks if the limit is reached
    }

    // Assign the next available PID to the new task
    task->pid = next_pid;

    // Copy the task into the tasks array at the position indicated by Task_counter
    CopyTask(task, &tasks[Task_counter]);

    // Update the task manager's counters and PID
    IncrementPIDS_TASKS();

    return true; // Task added successfully
}

void TaskManager::IncrementPIDS_TASKS() {
    // Increment the PID for the next task to be added
    ++next_pid;

    // Increment the total number of tasks
    ++Task_counter;

    // Increment the number of active tasks
    ++Active_Task_counter;
}

void TaskManager::Waitpid(common::uint32_t pid) {
    // Printing waitpid system call is called.
    printf("--- WAITPID SYSCALL IS CALLED ---\n");

    // Get a pointer to the current task (the parent)
    Task *parent = &tasks[currentTask];

    // If the pid is greater than 0, wait for a specific child process to finish
    if (pid > 0) {
        // Find the index of the task with the given pid
        int childIndex = FindTask(pid);
        // Make sure there is such a child process
        if (childIndex != -1 && tasks[childIndex].ppid == parent->pid) {
            // Set the parent task state to Blocked
            parent->state = Blocked;
            // Set the waitparent flag of the child task to true
            // The parent will wake up when the child task is terminated
            tasks[childIndex].waitparent = true;
        }
    }
    else {
        // If pid is -1, set the waitparent flag to true for all child processes
        for (int i = 0; i < Task_counter; ++i) {
            // Check if the task is a child of the parent
            if (tasks[i].ppid == parent->pid) {
                // Set the parent task state to Blocked
                parent->state = Blocked;
                // Set the waitparent flag of the child task to true
                tasks[i].waitparent = true;
            }
        }
    }
}


void TaskManager::Exit() {
    // Print a message indicating that the exit system call has been invoked.
    printf("--- EXIT SYSCALL IS CALLED ---\n");
    
    // Retrieve the current task using the currentTask index.
    Task *current = &tasks[currentTask];

    // Checking if the current task's PID (Process ID) is 1, which typically indicates 
    // the init process. The init process is important and it should 
    // not be terminated. If the current task is the init process, just returning 
    // without performing any further actions.
    if (current->pid == 1)
        return;

    // Check if the current task's parent is waiting for this task to terminate.
    if (current->waitparent) {
        // Find the index of the parent task using the parent's PID (ppid).
        int index_of_parent = FindTask(current->ppid);
        // If the parent task is found (index is valid), set the parent's state to Ready.
        // This indicates that the parent can continue execution after the child terminates.
        if (index_of_parent >= 0)
            tasks[index_of_parent].state = Ready;
    }

    // Decrement the active task counter as this task is about to be terminated.
    --Active_Task_counter;
    
    // Set the current task's state to Terminated to mark it as no longer active.
    current->state = TaskState::Terminated;
}


common::uint32_t TaskManager::Execve(void (*entrypoint)()) {
    // Printing execve system call is called.
    printf("--- EXECVE SYSCALL IS CALLED ---\n");

    // Get a pointer to the current task
    Task *current = &tasks[currentTask];

    // Reinitialize the current task with the new entry point
    // Keep the same PID, PPID, and priority
    current->InitTask(gdt, entrypoint, current->pid, current->ppid, current->priority);

    // Return the CPU state of the current task which will now start executing the new entry point
    return (common::uint32_t) current->cpustate;
}


common::uint32_t TaskManager::Fork(CPUState *cpustate) {
    printf("--- FORK SYSCALL IS CALLED ---\n"); // message to state fork is called.

    if (Task_counter >= 256) // checking if the max num of tasks reached
        return -1; // then returning -1 to indicate.
    
     // Get pointers to the parent task (current task) and the new child task
    Task *parent = &tasks[currentTask];
    Task *child = &tasks[Task_counter];
    
    // Initializing the child task with the parent's entry point, next available PID,
    // parent's PID, and the same priority as the parent
    child->InitTask(gdt, (void(*)()) parent->cpustate->eip, next_pid, parent->pid, parent->priority);

    // Copying the CPU state from the parent to the child
    *(child->cpustate) = *cpustate; 
    
    IncrementPIDS_TASKS(); // Increment the next available PID, the task counter, and the active task counter
    return child->pid; // Returning the PID of the new created child task
}

int TaskManager::FindTask(common::uint32_t pid) {
    for (int i = 0; i < Task_counter; ++i)
        if (tasks[i].pid == pid)
            return i;
    return -1;
}




CPUState* TaskManager::Schedule(CPUState* cpustate){
    if (Task_counter <= 0) // if there is no tasks to schedule which means no context switch will occur because there is nothing to switch to.
        // Threfore the current cpu state will be returned.
        return cpustate;
    
    if (currentTask >= 0)   //  If there is a current task being executed save its CPU state. 
        // The CPU state includes registers, stack pointer, program counter, etc. 
        tasks[currentTask].cpustate = cpustate; // The current CPU state (cpustate) is saved to the current task's entry in the task table.

    ControlTask(); 

    // printing the process table
    // it is commented because when it is executed the outputs can not be seen clearly for lifecycle.
    /*if (Active_Task_counter > 1)
        GetTaskStatus();*/

    
    return tasks[currentTask].cpustate; 
    // After selecting the next task to run (via ControlTask()), the function returns the CPU state of this newly selected task. 
}


void TaskManager::PrintStatusOfTask(int index_of_table) {
    Task *task = &tasks[index_of_table];

    printDigit(task->pid);  printf("     "); 
    printDigit(task->ppid); printf("      ");

    switch (task->priority) {
        case Low: 
            printf("LOW PRIORITY"); 
            break;
        case Normal:    
            printf("NORMAL PRIORITY"); 
            break;
        case High:      
            printf("HIGH PRIORITY"); 
            break;
        default:        
            printf("DEFAULT CASE! UNSPECIFIED PRIORITY"); 
            break;
    }
    printf("    ");

    switch (task->state) {
        case Ready:         
            printf("READY"); 
            break;
        case Running:       
            printf("RUNNING"); 
            break;
        case Blocked:       
            printf("BLOCKED"); 
            break;
        case Terminated:    
            printf("TERMINATED");
            break;
        default:            
            printf("DEFAULT CASE! UNSPECIFIED STATE ! "); 
            break;
    }
    printf("\n");
}

void printTableBorder(char * c) {
    const int COL_LENGTH = 80;
    for (int i = 0; i < COL_LENGTH; ++i) 
        printf(c);
    // when the coloumn lenght is exceeded, the cursor is set 
    // implicitly to the next line, so no need to add newline charachter
}

void TaskManager::GetTaskStatus() {
    printf("\n");
    printTableBorder("*");
    printf("PID   PPID   Priority   State\n");
    for (int i = 0; i < Task_counter; ++i)
        PrintStatusOfTask(i);
    printTableBorder("*");
}

void TaskManager::PrintCPUState(Task * task) {
    printf("PID: ");
    printDigit((uint32_t) task->pid);
    printf(", ");
    PrintCPUState(task->cpustate);
}

void TaskManager::PrintCPUState(CPUState * cpustate) {
    printf("eax: ");
    printDigit((uint32_t) cpustate->eax);
    printf(", ebx: ");
    printDigit((uint32_t) cpustate->ebx);
    printf(", ecx: ");
    printDigit((uint32_t) cpustate->ecx);
    printf(", edx: ");
    printDigit((uint32_t) cpustate->edx);
    printf(", esi: ");
    printDigit((uint32_t) cpustate->esi);
    printf(", edi: ");
    printDigit((uint32_t) cpustate->edi);
    printf(", ebp: ");
    printDigit((uint32_t) cpustate->ebp);
    printf(", error: ");
    printDigit((uint32_t) cpustate->error);
    printf(", eip: ");
    printDigit((uint32_t) cpustate->eip);
    printf(", cs: ");
    printDigit((uint32_t) cpustate->cs);
    printf(", eflags: ");
    printDigit((uint32_t) cpustate->eflags);
    printf(", esp: ");
    printDigit((uint32_t) cpustate->esp);
    printf(", ss: ");
    printDigit((uint32_t) cpustate->ss);
    printf("\n");
}