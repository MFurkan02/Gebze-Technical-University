#ifndef __TEST_H
#define __TEST_H

#include <gdt.h>
#include <multitasking.h>

namespace myos
{
    
    void lifecyleA(GlobalDescriptorTable *gdt, TaskManager *taskManager);

    void lifecyle1(GlobalDescriptorTable *gdt, TaskManager *taskManager);
    
    void lifecyle2(GlobalDescriptorTable *gdt, TaskManager *taskManager);
    
    void lifecyle3(GlobalDescriptorTable *gdt, TaskManager *taskManager);

   
    int LinearSearch(int * array, int size, int target);
    
    void LinearSearchTesting();

    
    int BinarySearch(int *arr, int target, int lo, int hi);
    
    void BinarySearchTesting(); 


    void printSeq(int *seq); 
   
    void CollatzSequence(int n, int *buff); 
   
    void CollatzSequenceTesting(); 

    
    void testWaitpid(GlobalDescriptorTable *gdt, TaskManager *taskManager); 
    
   
    void Fork(GlobalDescriptorTable *gdt, TaskManager *taskManager); 
   
    void ForkTesting();
    
    
    void testExecve(GlobalDescriptorTable *gdt, TaskManager *taskManager); 
   
    void epTestExecve();
    
   
    void testForkAndExecve(GlobalDescriptorTable *gdt, TaskManager *taskManager); 
   
    void epTestForkAndExecve();
    
   
    void testScanf(GlobalDescriptorTable *gdt, TaskManager *taskManager);
   
    //void epTestScanf();
}
#endif