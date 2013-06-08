//threadmanager.cc 
//    a manager of the global threads, implements a list of thread element
//
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "threadmanager.h"
//
//      private:
//              int threadNumber;         //record the totoal number of the threads
//              int threadIndex;          //being used when we product a threadID
//              List *threadList;
//

void 
printStatus(int num) 
{ 
     Thread *currentThread =  (Thread*) num;
     int status = currentThread -> getStatus();  
     
     printf("name: %s\t\tTID: %d\t\tstatus: ", currentThread -> getName(), currentThread -> getID());
     //enum ThreadStatus { JUST_CREATED, RUNNING, READY, BLOCKED };
     switch(status)
     {
         case 0: printf("JUST_CREATED\n"); break;
         case 1: printf("RUNNING\n"); break;
         case 2: printf("READY\n"); break;
         case 3: printf("BLOCKED\n"); break;
         default: printf("error\n"); break; 
     }
}


ThreadManager::ThreadManager()
{
       threadNumber = 0;
       threadIndex = 0;
       
       threadList = new List();
       mutex = new Lock("mutex");
}


ThreadManager::~ThreadManager()
{
       if(threadList != NULL)
          delete threadList;

       delete mutex;
} 

int 
ThreadManager::generateID()
{   
     int id = threadIndex;
     threadIndex++;

     return id;
}


int 
ThreadManager::getCurrentNum()      //get the number of the totoal threads
{
      return threadNumber;
}


void 
ThreadManager::threadStatus()      // 'TS' output the status of all the threads
{
    threadList -> Mapcar(printStatus);
   //void Mapcar(VoidFunctionPtr func);	// Apply "func" to every element 
					// on the list      
} 

Thread* 
ThreadManager::returnThread(int ID)
{
    return (Thread*)(threadList -> returnItem(ID));
}


void 
ThreadManager::addThread(Thread *newThread)         // add a thread into the list when a thread is forked
{
    mutex -> Acquire();

    threadList -> SortedInsert((void*)newThread, newThread -> getID());
    threadNumber++;

    mutex -> Release();
}

void 
ThreadManager::removeThread(int ID)      // remove a thread from the list when the thread is finished 
{ 
  
    mutex -> Acquire();

    threadList -> removeItem(ID);
    threadNumber--;

    mutex -> Release();
}

