//threadmanager.h 
//    a manager of the global threads, implements a list of thread element
//
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include "list.h"
#include "thread.h"

class ThreadManager
{

      private:
              int threadNumber;                    //record the totoal number of the threads
              int threadIndex;          //being used when we product a threadID
              List *threadList;

      public:
              ThreadManager();
              ~ThreadManager(); 

              int generateID();

              int getCurrentNum();      //get the number of the totoal threads
              void threadStatus();      // 'TS' output the status of all the threads 

              void addThread(Thread *newThread);         // add a thread into the list when a thread is forked
              void removeThread(int ID);      // remove a thread from the list when the thread is finished
               
};


#endif
