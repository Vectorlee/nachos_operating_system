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
#include "openfile.h"
#include "thread.h"

//---------------------------------------------------------------------
//forkFunction
//     The function we use to fork a new thread, through a inner function
//
//---------------------------------------------------------------------

void 
forkFunction(int address)
{
    //interrupt -> DumpState();

    currentThread -> space -> InitRegisters();		     // set the initial register values
    currentThread -> space -> RestoreState();

    machine -> registers[PCReg] = address;   //set the PC
    machine -> registers[NextPCReg] = address + 4; 

    machine->Run();

    ASSERT(FALSE);
}

//---------------------------------------------------------------------
//getMemoryAddr
//     get the real memory address of a variable
//     used in the syscall handler
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


#define C_LS     1
#define C_CAT    2
#define C_MKDIR  3
#define C_RM     4
#define C_CD     5
#define C_TOUCH  6
#define C_MV     7
#define C_RMDIR  8
#define C_PWD    9
#define C_CP     10
#define C_PS     11
#define C_KILL   12
#define C_EXIT   13

char ShellInput[0x100];


//---------------------------------------------------------------------------
//CheckInput
//
//      check the command input, whether it is a shell command or not
//--------------------------------------------------------------------------

#ifdef FILESYS


int
CheckInput(char *input)
{
    char buffer[0x100], *tokenPtr;

    memset(ShellInput, 0, sizeof(ShellInput));
    strncpy(ShellInput, input, 0x100);

    memset(buffer, 0, sizeof(buffer));
    strncpy(buffer, input, 0x100);


    tokenPtr = strtok(buffer, " ");
    if(!strcmp(tokenPtr, "ls"))
    {
        return C_LS;
    }
    if(!strcmp(tokenPtr, "cat"))
    {
        return C_CAT;
    }
    if(!strcmp(tokenPtr, "mkdir"))
    {
        return C_MKDIR;
    }
    if(!strcmp(tokenPtr, "cd"))
    {
        return C_CD;
    }
    if(!strcmp(tokenPtr, "touch"))
    {
        return C_TOUCH;
    }
    if(!strcmp(tokenPtr, "rm"))
    {
        return C_RM;
    }        
    if(!strcmp(tokenPtr, "mv"))
    {
        return C_MV;
    }
    if(!strcmp(tokenPtr, "cp"))
    {
        return C_CP;
    } 
    if(!strcmp(tokenPtr, "pwd"))
    {
        return C_PWD;
    }
    if(!strcmp(tokenPtr, "rmdir"))
    {
        return C_RMDIR;
    }
    if(!strcmp(tokenPtr, "ps"))
    {
        return C_PS;
    }
    if(!strcmp(tokenPtr, "kill"))
    {
        return C_KILL; 
    }
    if(!strcmp(tokenPtr, "exit"))
    {
        return C_EXIT; 
    }
    return 0;
}


//-------------------------------------------------------------------------
//ShellCommand
//      Execute the shell command.
//-------------------------------------------------------------------------

void Shell_CAT()
{
    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "cat"));

    tokenPtr = strtok(NULL, " ");
    if(tokenPtr != NULL)
    {
        int sector = fileSystem -> FindFile(tokenPtr); 
        if(sector != -1)
        {
            OpenFile* openfile = new OpenFile(sector); 
            openfile -> FileContent();
            delete openfile; 
        }
        else
            printf("[-] File does not exist\n");
    }
    else
        printf("[-] 'cat' function lack argument\n");

    return;
}

void
Shell_MKDIR()
{
    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "mkdir"));

    tokenPtr = strtok(NULL, " ");
    if(tokenPtr != NULL)               //to simplify our program, we assume the "mkdir" 
    {                                  //can only create directory under current directory
        int sector = fileSystem -> FindFile(tokenPtr); 
        if(sector == -1)
        {
            fileSystem -> CreateDirectory(tokenPtr);
        }
        else
            printf("[-] File already exists\n");
    }
    else
        printf("[-] 'mkdir' lack directory name\n");

    return;
}


void
Shell_CD()
{
    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "cd"));

    tokenPtr = strtok(NULL, " ");
    if(tokenPtr != NULL)
    {
        int sector = fileSystem -> FindFile(tokenPtr); 
        if(sector != -1)
        {
            OpenFile* openfile = new OpenFile(sector); 
            if(openfile -> FileType() == 1)
                fileSystem -> ChangeDirectory(sector);
            else
                printf("[-] It isn't a directory\n");

            delete openfile;
        }
        else
            printf("[-] Directory doesn't exist\n");
    }
    else
        printf("[-] 'cd' lack directory name\n");
    
    return;
}


void
Shell_TOUCH()
{
    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "touch"));

    tokenPtr = strtok(NULL, " ");
    if(tokenPtr != NULL)
    {    
        int sector = fileSystem -> FindFile(tokenPtr);
        if(sector != -1)
        {
            OpenFile *openfile = new OpenFile(sector);
            openfile -> Read(NULL, 0);
        }
        else
        {
            char basename[0x10], pathname[0x100];
            BaseName(basename, tokenPtr);
            PathName(pathname, tokenPtr);

            if(pathname[0] != NULL && fileSystem -> FindFile(pathname) == -1)
                printf("[-] The path is invalid\n");
            else
            {
                if(pathname[0] != NULL)
                    fileSystem -> Create(basename, 0, pathname);
                else
                    fileSystem -> Create(basename, 0);
            }
        }
    }
    else
        printf("[-] 'touch' lack file name\n");

    return;
}

void
Shell_RM()
{
    char basename[0x10], pathname[0x100];

    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "rm"));

    tokenPtr = strtok(NULL, " ");
    if(tokenPtr != NULL)
    {
        int sector = fileSystem -> FindFile(tokenPtr); 
        if(sector != -1)
        {
            OpenFile *openfile = new OpenFile(sector);
            if(openfile -> FileType() == 0)
            {
                BaseName(basename, tokenPtr);
                PathName(pathname, tokenPtr);

                if(pathname[0] != NULL)
                    fileSystem -> Remove(basename, pathname);
                else
                    fileSystem -> Remove(basename); 
            }
            else 
                printf("[-] Can't delete a directory\n");

            delete openfile;
        }
        else
            printf("[-] file doesn't exist\n");
    }
    else
        printf("[-] 'rm' lack file name\n");

    return;
}

//This 'mv' function, can only move one file from current 
//location, to current location(change name), or move to another 
//place 
//
void 
Shell_MV()
{
    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "mv"));

    char *file1 = strtok(NULL, " ");
    char *file2 = strtok(NULL, " "); 

    if(file1 != NULL && file2 != NULL) 
    {
        int sector1 = fileSystem -> FindFile(file1);
        if(sector1 == -1)
        {
            printf("[-] File '%s' doesn't exist\n", file1);
            return;
        }

        int sector2 = fileSystem -> FindFile(file2);
        if(sector2 == -1)       //current location, with a different name.
            fileSystem -> MoveFile(".", ".", file1, file2); 

        else                    //different location, with same name
        {
            OpenFile *openfile = new OpenFile(sector2);
            if(openfile -> FileType() == 1) 
            {
                 fileSystem -> MoveFile(".", file2, file1, file1);    
            }
            else
                 printf("[-] File already exist\n"); 

            delete openfile;
        }
    }
    else
        printf("[-] 'mv' needs more arguments\n");

    return;
}

void
Shell_CP()
{
    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "cp"));

    char *file1 = strtok(NULL, " ");
    char *file2 = strtok(NULL, " "); 

    if(file1 != NULL && file2 != NULL) 
    {
        int sector1 = fileSystem -> FindFile(file1);
        if(sector1 == -1)
        {
            printf("[-] File '%s' doesn't exist\n", file1);
            return;
        }

        int sector2 = fileSystem -> FindFile(file2);
        if(sector2 == -1)       //current location, with a different name.
            fileSystem -> CopyFile(".", ".", file1, file2); 

        else                    //different location, with same name
        {
            OpenFile *openfile = new OpenFile(sector2);
            if(openfile -> FileType() == 1) 
            {
                 fileSystem -> CopyFile(".", file2, file1, file1);    
            }
            else
                 printf("[-] File already exist\n"); 

            delete openfile;
        }
    }
    else
        printf("[-] 'mv' needs more arguments\n");

    return;

}


void
Shell_RMDIR()
{
    char basename[0x10], pathname[0x100];

    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "rmdir"));

    tokenPtr = strtok(NULL, " ");
    if(tokenPtr != NULL)
    {
        int sector = fileSystem -> FindFile(tokenPtr); 
        if(sector != -1)
        {
            OpenFile *openfile = new OpenFile(sector);
            if(openfile -> FileType() == 1)
            {
                BaseName(basename, tokenPtr);
                PathName(pathname, tokenPtr);
 
                fileSystem -> RemoveDirectory(tokenPtr);
                if(pathname[0] != NULL)
                    fileSystem -> Remove(basename, pathname);
                else
                    fileSystem -> Remove(basename);
            }
            else 
                printf("[-] Can't delete a file\n");

            delete openfile;
        }
        else
            printf("[-] file doesn't exist\n");
    }
    else
        printf("[-] 'rmdir' lack file name\n");

    return;
}


void
Shell_KILL()
{
    char *tokenPtr = strtok(ShellInput, " ");
    ASSERT(!strcmp(tokenPtr, "kill"));

    tokenPtr = strtok(NULL, " ");
    if(tokenPtr != NULL)
    {
        int id = atoi(tokenPtr);
        Thread* killthread = threadmanager -> returnThread(id);

        if(killthread != NULL)
        {
            if(killthread == currentThread){
                 printf("[-] You can't kill your self\n");
                 return;
            }

            killthread -> killed = TRUE;
            threadmanager -> removeThread(id);

            //if(killthread -> filename != NULL)    //delete the temporary file.
            //{
            //     if(fileSystem -> Remove(filename))
            //         printf("[+] Removed the temp file: %s\n", filename);
            //}

            printf("[+] Removed thread: %d\n", id);
        } 
        else
            printf("[-] Please Input a valid thread id\n");     
    }
    else
        printf("[-] 'kill' lack thread id\n");

    return;
}

void 
ShellCommand(int input)
{
    //char *command = (char*)input;
    //char *tokenPtr = strtok(command, " ");
    //char pathname[0x100], basename[0x10];

    switch(input)
    {
       case C_LS:    fileSystem -> List(); break;
       case C_CAT:   Shell_CAT();          break;  
       case C_CD:    Shell_CD();           break;
       case C_MKDIR: Shell_MKDIR();        break;
       case C_TOUCH: Shell_TOUCH();        break;
       case C_RM:    Shell_RM();           break;
       case C_MV:    Shell_MV();           break;
       case C_CP:    Shell_CP();           break;
       case C_RMDIR: Shell_RMDIR();        break;
       case C_PS:    threadmanager -> threadStatus();  break;
       case C_PWD:   fileSystem -> WorkingDirectory(); break;
       case C_KILL:  Shell_KILL();         break;
       //case C_EXIT:  currentThread -> Finish();   break;
       
       default: ASSERT(FALSE); break;
    }
}

#endif


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
    int arg1, arg2, arg3;

    if (which == SyscallException) { 
        
        machine -> registers[PCReg] = machine -> registers[NextPCReg];
        machine -> registers[NextPCReg] += 4;
        
        switch(type)
        {
            case SC_Print:
            {  
                int value;

                arg1 = machine -> registers[4];               
                arg2 = machine -> registers[5];

                if(arg1 == 0)
                    printf("%d", arg2);
                else
                {
                    value = getMemoryAddr(arg2, 4);
                    printf("%s", (char*)((machine -> mainMemory) + value));
                }
                break;
            } 
            case SC_Halt:
            {
                DEBUG('a', "Shutdown, initiated by user program.\n");
   	
                printf("[+] Invoke a halt system call\n");
                interrupt->Halt();

                break;
            }
            case SC_Exit:
            {
                printf("[+] Invoke a exit system call\n");
                currentThread -> Finish();
                break;
            }
#ifdef FILESYS
            case SC_Exec:
            {
                int value;
                char *name; 

                arg1 = machine -> registers[4];
                
                value = getMemoryAddr(arg1, 4);
                name = (char*)((machine -> mainMemory) + value);

                //printf("the File: %s\n", name);
                int ret = CheckInput(name);  
                if(ret != 0)
                {
                    if(ret == C_EXIT) {               //shut down the shell
                        currentThread -> Finish();
                    }

                    Thread *sthread = new Thread("Command");
                    sthread -> Fork(ShellCommand, ret);

                    machine -> registers[2] = sthread -> getID(); 
                }  
                else
                { 
                    Thread *pthread = new Thread(name); 
                    pthread -> Fork(runProgram, int(name));  //create a new thread//run the program

                    machine -> registers[2] = pthread -> getID();   //we return the thread id of the new thread;
                }
                break;
            }
            case SC_Join:   //wait until the program terminate
            {
                arg1 = machine -> registers[4];

                currentThread -> Join(arg1);
                break;
            }
//#define SC_Create
//#define SC_Open
//#define SC_Read 
//#define SC_Write
//#define SC_Close
            case SC_Create:
            {
                arg1 = machine -> registers[4];
                int value = getMemoryAddr(arg1, 4);
                char* name = (char*)((machine -> mainMemory) + value);

                int result = fileSystem -> Create(name, 0);
                
                machine -> registers[2] = result;
                break;
            }
            case SC_Open:
            {
                arg1 = machine -> registers[4];
                int value = getMemoryAddr(arg1, 4);
                char* name = (char*)((machine -> mainMemory) + value);

                OpenFile* openfile = (fileSystem -> Open(name));
                fileManager -> Append(openfile -> FileSector()); //we put the file manager here 

                machine -> registers[2] = (OpenFileId)openfile;  //here we should be careful, we need to give a kernel pointer to user space
                break;
            }
            case SC_Read:
            {
                arg1 = machine -> registers[4];
                arg2 = machine -> registers[5];
                arg3 = machine -> registers[6];

                int value = getMemoryAddr(arg1, arg2);

                char* buffer = (char*)((machine -> mainMemory) + value);

                int size = arg2;

                if(arg3 == ConsoleInput)
                {
                    char temp; 
                    for(int i = 0; i < size; i++)
                    {
                        scanf("%c", &temp);
                        buffer[i] = temp;
                    }
                    machine -> registers[2] = size; 
                }
                else
                {
                    OpenFile* openfile = (OpenFile*)arg3;
                    
                    int sector = openfile -> FileSector();
                    fileManager -> LockFile(sector);

                    int result = openfile -> Read(buffer, size);

                    fileManager -> ReleaseFile(sector);

                    machine -> registers[2] = result;
                }
                break;
            }
            case SC_Write:
            {
                arg1 = machine -> registers[4];
                arg2 = machine -> registers[5];
                arg3 = machine -> registers[6];

                int value = getMemoryAddr(arg1, 4);
                char* buffer = (char*)((machine -> mainMemory) + value);
                int size = arg2;

                if(arg3 == ConsoleOutput)
                {
                    char temp; 
                    for(int i = 0; i < size; i++)
                    {
                        temp = buffer[i];
                        printf("%c", temp);
                    }
                    machine -> registers[2] = size; 
                }
                else  
                {

                    OpenFile* openfile = (OpenFile*)arg3;

                    int sector = openfile -> FileSector();
                    fileManager -> LockFile(sector);

                    int result = openfile -> Write(buffer, size);

                    fileManager -> ReleaseFile(sector);
                }
                break;
            }
            case SC_Close:
            {
                arg1 = machine -> registers[4];
                
                OpenFile* openfile = (OpenFile*)arg1;
                fileManager -> Remove(openfile -> FileSector());
                
                delete openfile;
                break;
            }
            case SC_Fork:
            {
                arg1 = machine -> registers[4];

                int value = arg1;

                Thread *pthread = new Thread("Interanl Thread");       // create a new thread

                pthread -> space = currentThread -> space;
                pthread -> filename = currentThread -> filename;       // use the same address space

                pthread -> Fork(forkFunction, value);
                
                break; 
            }
            case SC_Yield:
            {
                currentThread -> Yield();
                break; 
            }
#endif 
            default:
                break; 
        }
    }

    else if(which == PageFaultException){

        printf("[*] CurrentThread name %s occured a PageFault\n", currentThread -> getName());

        memorymanager -> loadPage();    
    }
    else {
	printf("[-] Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}
