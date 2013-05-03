
#include "memmgr.h"
#include "machine.h"
#include "system.h"
#include "addrspace.h"

MemoryManager::MemoryManager()
{
    manager = new BitMap(MemorySize);

    for(int i = 0; i < 60; i++)
        setPage(i);                 //block the first 100 page
}



MemoryManager::~MemoryManager()
{
    delete manager; 
}

//----------------------------------------------------------------------
// MemoryManager::findCleanPage
// 
//      find a clean page in the memory and return the 
//      physical address of the page
//
//      if we can't find the clean page, we return -1 0xFFFFFFFF
// 
//----------------------------------------------------------------------

int 
MemoryManager::findCleanPage()
{
     int address;

     address = manager -> pureFind();   //find the first byte of the memory which is clean.

     if( address == -1 )      //we haven't find a clean page
        return -1;

     ASSERT(address % PageSize == 0 && !(manager -> Test(address + 1)) && !(manager -> Test(address + PageSize - 1)));    
     //assert that the whole page is clean, assert the start address is a multiple of PageSize

     return address / PageSize;

}


//----------------------------------------------------------------------
// MemoryManager::numClean
// 
//      return how many pages in the memory is clean
// 
//----------------------------------------------------------------------

int 
MemoryManager::numClean()
{
     int num;

     num = manager -> NumClear();   // return how many bytes are clean
    
     ASSERT(num % PageSize == 0);   // the number must be a multiple of PageSize 
     
     return num / PageSize;         // return the page number

}


//----------------------------------------------------------------------
// MemoryManager::setPage
// 
//      set a page in the memory that tell the user it is used
// 
//----------------------------------------------------------------------


void
MemoryManager::setPage(int pagenum)
{
     //we assert that the start address is a multiple of PageSize
     int address = pagenum * PageSize; 

     for(int i = address; i < address + PageSize; i++)
         manager -> Mark(i);     

}

//----------------------------------------------------------------------
// MemoryManager::cleanPage
// 
//      clean a page in the memory
// 
//----------------------------------------------------------------------

void
MemoryManager::cleanPage(int pagenum)
{ 
     //we assert that the start address is a multiple of PageSize
     int address = pagenum * PageSize;

     for(int i = address; i < address + PageSize; i++)
         manager -> Clear(i);     

}


//----------------------------------------------------------------------
// MemoryManager::loadPage
// 
//      page fualt occurred, we find a clean page in the memory
//      and load the wanted page in the memory.      
//
//----------------------------------------------------------------------

bool 
MemoryManager::loadPage()
{

     (stats -> numPageFaults)++;

     char *filename = currentThread -> filename;      // get the executable file name of the thread
     unsigned int vaddr = machine -> registers[BadVAddrReg];    // get the page fault address of the thread
     int phynum;

     //printf("BadVAddr: %d\n", vaddr);


     // caculate the corresponding page number of this address
     unsigned int vpn = vaddr / PageSize;
     ASSERT(!machine -> pageTable[vpn].valid);  // assert the page is not in the memory



     // find a clean page in the memory for the program     
     if( numClean() < 16 )  //if the clean page less than 16, we should invoke the page swap
     {

         //printf("unload occured\n");

         phynum = unloadPage();               
         ASSERT(phynum != -1);
     }
     else 
     {
         phynum = findCleanPage();
         ASSERT(phynum != -1);
     }
     setPage(phynum);     //set the page 


     //printf("virtual page: %d, physical page: %d\n", vpn, phynum); 

     //open file and load the page
     OpenFile *executable = fileSystem -> Open(filename);

     executable -> ReadAt(&(machine -> mainMemory[phynum * PageSize]), PageSize, vpn * PageSize);       

     // set the pagetable
     machine -> pageTable[vpn].physicalPage = phynum;
     machine -> pageTable[vpn].valid = TRUE;
     machine -> pageTable[vpn].use = FALSE;
     machine -> pageTable[vpn].dirty = FALSE;
     machine -> pageTable[vpn].readOnly = FALSE;
     machine -> pageTable[vpn].clockCount = 0;


     //close file
     delete executable;

     return TRUE;
}


//----------------------------------------------------------------------
// MemoryManager::unloadPage
// 
//     when page fualt occurred, and there is no more clean pages in the memory
//     we need to unload severl pages from memory to the disk     
//
//----------------------------------------------------------------------

int
MemoryManager::unloadPage()
{

     (stats -> numPageSwap)++;

     char *filename = currentThread -> filename;
     int pagenum, phynum, vpnum;


     pagenum = SwapAlgorithm();  //choose a page to swap
     ASSERT(pagenum != -1);


     vpnum  = machine -> pageTable[pagenum].virtualPage;
     phynum = machine -> pageTable[pagenum].physicalPage;
     
     machine -> pageTable[pagenum].valid = FALSE;


     //open file and write memory data, if the page is dirty
     if(machine -> pageTable[pagenum].dirty)
     {
         (stats -> numWriteBack)++;

         OpenFile *executable = fileSystem -> Open(filename);     
         executable -> WriteAt(&(machine -> mainMemory[phynum * PageSize]), PageSize, vpnum * PageSize);

         delete executable;
     }

     //clean the page
     cleanPage(phynum);  

     return phynum;
}

//----------------------------------------------------------------------
// MemoryManager::SwapAlgorith
// 
//     when page fualt occurred, and there is no enough clear page     
//     in the memory, we use this function to decide which page in the  
//     memory should we swap it down.
//
//----------------------------------------------------------------------


int 
MemoryManager::SwapAlgorithm()    // NRU
{

     int pagenum, size, i; 
     TranslationEntry *entry; 
     
     pagenum = -1;
     size = machine -> pageTableSize;

     // find the R: 0, M: 0 
     for(i = 0; i < size; i++)
     {
          entry = &(machine -> pageTable[i]); 

          if(entry -> valid && !entry -> use && !entry -> dirty)
          {  
               pagenum = i; 
               break;  
          }
     }
     if(pagenum != -1)
        return pagenum; 


     // find the R:0, M: 1 
     for(i = 0; i < size; i++)
     {
          entry = &(machine -> pageTable[i]); 

          if(entry -> valid && !entry -> use && entry -> dirty)
          {  
               pagenum = i; 
               break;  
          }
     }     

     if(pagenum != -1)
        return pagenum; 

    
     // find the R:1, M: 0     
     for(i = 0; i < size; i++)
     {
          entry = &(machine -> pageTable[i]); 

          if(entry -> valid && entry -> use && !entry -> dirty)
          {  
               pagenum = i; 
               break;  
          }
     }     

     if(pagenum != -1)
        return pagenum; 


     // find the R:1, M: 0
     for(i = 0; i < size; i++)
     {
          entry = &(machine -> pageTable[i]); 

          if(entry -> valid)
          {  
               pagenum = i; 
               break;  
          }
     }     

     if(pagenum != -1)
        return pagenum; 

}
