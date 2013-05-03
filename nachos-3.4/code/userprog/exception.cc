// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//---------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------

int 
getMemoryAddr(int vaddr, int size)
{
     int physicalAddress;

     ExceptionType exception = machine -> Translate(vaddr, &physicalAddress, size, FALSE);     
  
     if(exception != NoException) {

        machine -> registers[BadVAddrReg] = vaddr;	
        memorymanager -> loadPage(); 
        exception = machine -> Translate(vaddr, &physicalAddress, size, FALSE);

        ASSERT(exception == NoException);
    }

    return physicalAddress; 

}



//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{

    int type = machine->ReadRegister(2);
    int address;

    if ((which == SyscallException) && (type == SC_Print)) {

        machine -> registers[PCReg] = machine -> registers[NextPCReg];
        machine -> registers[NextPCReg] += 4;
        
        if((machine -> registers[4]) == 1)   //string
        {
            address = getMemoryAddr((machine -> registers[5]), 4);
            printf("%s", machine -> mainMemory + address);
        }
        else if((machine -> registers[4]) == 2)   //int 
        {
            printf("%d", (machine -> registers[5]));
        }
        else
            ASSERT(FALSE);

    }
    else if ((which == SyscallException) && (type == SC_Halt)) {

        DEBUG('a', "Shutdown, initiated by user program.\n");
   	
        printf("invoke a halt system call\n");

        machine -> registers[PCReg] = machine -> registers[NextPCReg];
        machine -> registers[NextPCReg] += 4;

        interrupt->Halt();
    }
    else if((which == SyscallException) && (type == SC_Exit)){
        
        printf("invoke a exit system call\n");
 
        machine -> registers[PCReg] = machine -> registers[NextPCReg];
        machine -> registers[NextPCReg] += 4; 

        currentThread -> Finish();

    } 
    else if(which == PageFaultException){

        //printf("occured a PageFault\n");
        memorymanager -> loadPage();
    
    }
    else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}
