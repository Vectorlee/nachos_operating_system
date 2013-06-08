// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "synch.h"
//#include "messagequeue.h"

// testnum is set in main.cc
int testnum = 5;


#define MaxAmount 11

//P,V operation
Semaphore *full, *empty;
int buffer = 0;


//Condition variable
Lock *mutex;
Condition *cond, *cond1, *cond2;


//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 10; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread -> Clock();
        //currentThread -> Yield(); 
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");
    t->Fork(SimpleThread, t->getID());

    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest2
// 	Set up several threads with different pirorities and time periods
//      test the running result
//----------------------------------------------------------------------
void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");
    
    Thread *t[10];
    for(int i = 0; i < 5; i++)   
    {
        t[i] = new Thread("forked thread");
        
        t[i] -> setPriority((i * i * i) % 10 + 1);
        t[i] -> setBaseTimePeriod( (i * i) % 10 + 1  );

        t[i] -> Fork(SimpleThread, t[i] -> getID());
    }
    for(int i = 5; i < 10; i++)   
    {
        t[i] = new Thread("forked thread");
        
        t[i] -> setPriority(8);
        t[i] -> Fork(SimpleThread, t[i] -> getID());
    }
}

void insert_item()
{
     buffer++;
     ASSERT(buffer <= MaxAmount);
     printf("thread %d insert item, items: %d\n", currentThread -> getID(), buffer);
}


void remove_item()
{
     buffer--;
     ASSERT(buffer >= 0);
     printf("thread %d remove item, items: %d\n", currentThread -> getID(), buffer);
}

//----------------------------------------------------
// 
//      Synchronous implemented by P, V operation
//----------------------------------------------------

void producer1(int dummy)
{ 
  while(TRUE) 
  {
    empty -> P();
    
    insert_item();
    
    full -> V();
  }

}

void consumer1(int dummy)
{ 
   while(TRUE) {
      
      full -> P();

      remove_item();
      
      empty -> V();
   }

}

//----------------------------------------------------
// Synchronous implemented by condition variable
//
//----------------------------------------------------

void producer2(int dummy)
{ 
  while(TRUE) 
  {
    mutex -> Acquire();    

    while(buffer == MaxAmount){   
        cond1 -> Wait(mutex);
    }

    insert_item();

    if(buffer == 1)
       cond2 -> Signal(mutex);
    
    mutex -> Release();
    //currentThread -> Yield();
  }

}

void consumer2(int dummy)
{ 
   while(TRUE) {
      
      mutex -> Acquire();
     
      while(buffer == 0){
         cond2 -> Wait(mutex);
      }

      remove_item();
      
      if(buffer == MaxAmount - 1)
         cond1 -> Signal(mutex);

      mutex -> Release();
      //currentThread -> Yield();
   }

}

//----------------------------------------------------
//    Test the result of the synchronous of threads
//      
//----------------------------------------------------

void
ThreadTest3()
{
    DEBUG('t', "Entering ThreadTest2");
    
    //full = new Semaphore("full", 0);
    //empty = new Semaphore("empty", MaxAmount);
    
    mutex = new Lock("mutex");
    cond1 = new Condition("cond1");    
    cond2 = new Condition("cond2");

    Thread *r[7];
    for(int i = 0; i < 7; i++)   
    {
        r[i] = new Thread("produce thread");
        //r[i] -> Fork(producer1, r[i] -> getID());
        
        r[i] -> Fork(producer2, r[i] -> getID());
    }

    Thread *w[4];
    for(int i = 0; i < 4; i++)   
    {
        w[i] = new Thread("consume thread");
        //w[i] -> Fork(consumer1, w[i] -> getID());

        w[i] -> Fork(consumer2, w[i] -> getID());
    }
}


void 
send(int dummy)
{
     key_t ipckey;
     int mq_id, flag;
     
     struct{ 
        long type; 
        char text[30]; 
     } mymsg;

     ipckey = messagequeue -> ftok("MSG_FILE", 42);  //this two argument is predefined

     ASSERT(ipckey != -1);

     mq_id = messagequeue -> msgget(ipckey, IPC_CREATE);

     ASSERT(mq_id != -1);

     printf("the address message queue 1 %d\n", mq_id);
     
     strcpy(mymsg.text, "Hello, world!");

     while(true)
     {
         mymsg.type = Random() % 5;
         flag = Random() % 3;         
 
         int result = messagequeue -> msgsnd(mq_id, &mymsg, sizeof(mymsg), flag); 

         printf("thread: %d send message with type %d, result: %d\n", currentThread -> getID(), mymsg.type, result);

         currentThread -> Yield(); 
     }
}

void 
receive(int dummy)
{
     key_t ipckey;
     int mq_id, flag, type;

     struct{ 
        long type; 
        char text[30]; 
     } mymsg;

     ipckey = messagequeue -> ftok("MSG_FILE", 42);

     ASSERT(ipckey != -1);

     mq_id = messagequeue -> msgget(ipckey, IPC_CREATE);

     ASSERT(mq_id != -1);

     printf("the address message queue 2 %d\n", mq_id);


     while(true)
     {
         type = Random() % 5;
         flag = Random() % 3;         
 
         int result = messagequeue -> msgrcv(mq_id, &mymsg, sizeof(mymsg), type, flag); 

         printf("thread: %d receive message with type %d, result: %d\n", currentThread -> getID(), type, result); 
         
         if(result != -1)
              printf("message: %s\n", mymsg.text);                       

         currentThread -> Yield();
     }
}

void 
ThreadTest4()
{

    Thread *t1 = new Thread("send thread");
    t1 -> Fork(send, t1 -> getID());

    Thread *t2 = new Thread("receive thread");
    t2 -> Fork(receive, t2 -> getID());

}

//======================================================

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:              
	ThreadTest1();          //primary thread
	break;
    case 2:
        ThreadTest2();          //thread with priority
        break;
    case 3:
        ThreadTest3();          //thread with synchronous 
        break;
    case 4:
        ThreadTest4();
        break; 
    default:
	printf("No test specified.\n");
	break;
    }
}

