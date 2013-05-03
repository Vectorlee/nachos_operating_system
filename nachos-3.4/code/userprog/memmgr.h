
#ifndef MEMMGR_H
#define MEMMGR_H

#include "bitmap.h"

class MemoryManager
{

     public:
            MemoryManager();
            ~MemoryManager();
 
            int findCleanPage();         // Find a clean page in the memory 
                                         // and allocate to the program, return the start address of the clear page
                                         // 0xffffffff means that there is not a clean page

            int numClean();              // Return the number of clean pages
    
            void setPage(int address);   // Set the page start from the specific address
            
            void cleanPage(int address); // Clean the page start from specific address 

            bool loadPage();             // load page for the current thread
   
            int unloadPage();            // unload page for the current thread 

            int SwapAlgorithm();         // swap a page from the memory to the disk

     private:
            BitMap *manager;
    
};


#endif
