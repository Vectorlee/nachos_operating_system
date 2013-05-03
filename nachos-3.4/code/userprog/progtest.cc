// progtest.cc 
//	Test routines for demonstrating that Nachos can load
//	a user program and execute it.  
//
//	Also, routines for testing the Console hardware device.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "synch.h"

//----------------------------------------------------------------------
// CreateTempFile
//        Create a temp file for a program to store 
//        the run-time information
//
//----------------------------------------------------------------------

char Buffer[0x10000];  //create a large buffer


void
CreateTempFile(OpenFile *executable, char *tempfile, int filesize)
{
    NoffHeader noffH;

    executable -> ReadAt((char *)&noffH, sizeof(noffH), 0);
    
    if ((noffH.noffMagic != NOFFMAGIC) && (WordToHost(noffH.noffMagic) == NOFFMAGIC))
    	SwapHeader(&noffH);
 
    ASSERT(noffH.noffMagic == NOFFMAGIC);



    //read the original executable file to the buffer
    memset(Buffer, 0, sizeof(Buffer));

    if (noffH.code.size > 0) {

        executable -> ReadAt(Buffer, noffH.code.size, noffH.code.inFileAddr);
    }
    if (noffH.initData.size > 0) {

        executable -> ReadAt(Buffer + noffH.code.size, noffH.initData.size, noffH.initData.inFileAddr);
    }


    //create a new file    
    fileSystem -> Create(tempfile, filesize);
    

    //write the data to the new file
    OpenFile *tempexe = fileSystem -> Open(tempfile);
    tempexe -> WriteAt(Buffer, filesize, 0);

    delete tempexe;  //close temp file
}

//---------------------------------------------------------------------
// IntToStr
//          Transfer the int value to a char string
//
//---------------------------------------------------------------------
void 
IntToStr(int value, char *buffer)
{
     int temp, left, i;

     temp = value;
     i = 0;

     while(temp > 0)
     {
         left = temp % 10;
         temp = temp / 10;  

         buffer[i] = (char)(left + '0');
         i++;
     } 
     buffer[i] = '\0';
 
     return;
}


//----------------------------------------------------------------------
// StartProcess
// 	Run a user program.  Open the executable, load it into
//	memory, and jump to it.
//----------------------------------------------------------------------

void
StartProcess(char *filename)
{

    // create user space
    OpenFile *executable = fileSystem->Open(filename);
    AddrSpace *space;

    if (executable == NULL) {
	printf("Unable to open file %s\n", filename);
	return;
    }
    space = new AddrSpace(executable);          //one page table and number of pages    
    


    //we create a temp file for the executable file to store the data
    char* tempfile = new char [ strlen(filename) + 12 ];     
    char IDBuffer[8];

    IntToStr(currentThread -> getID(), IDBuffer);

    strcpy(tempfile, filename);
    strcat(tempfile, IDBuffer);                // use the thread ID to distinguish each temp file

    int filesize = (space -> getNumPage()) * PageSize ;

    CreateTempFile(executable, tempfile, filesize);  // create the tempfile 



    // allocate the thread attribute    
    currentThread -> space = space;
    currentThread -> filename = tempfile;

    delete executable;			// close file

    space->InitRegisters();		// set the initial register values
    space->RestoreState();		// load page table register


    machine->Run();			// jump to the user progam
                                        // machine::Run defined in the mipssim.cc 

                                        // we read the data from the memory by calling
                                        // machine -> ReadMem();
                                        // the translation between virtual and physics occurs here.


    ASSERT(FALSE);			// machine->Run never returns;
					// the address space exits
					// by doing the syscall "exit"
}

// Data structures needed for the console test.  Threads making
// I/O requests wait on a Semaphore to delay until the I/O completes.

static Console *console;
static Semaphore *readAvail;
static Semaphore *writeDone;

//----------------------------------------------------------------------
// ConsoleInterruptHandlers
// 	Wake up the thread that requested the I/O.
//----------------------------------------------------------------------

static void ReadAvail(int arg) { readAvail->V(); }
static void WriteDone(int arg) { writeDone->V(); }

//----------------------------------------------------------------------
// ConsoleTest
// 	Test the console by echoing characters typed at the input onto
//	the output.  Stop when the user types a 'q'.
//----------------------------------------------------------------------

void 
ConsoleTest (char *in, char *out)
{
    char ch;

    console = new Console(in, out, ReadAvail, WriteDone, 0);
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    
    for (;;) {
	readAvail->P();		// wait for character to arrive
	ch = console->GetChar();
	console->PutChar(ch);	// echo it!
	writeDone->P() ;        // wait for write to finish
	if (ch == 'q') return;  // if q, quit
    }
}
