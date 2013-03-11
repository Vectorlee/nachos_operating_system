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

// testnum is set in main.cc
int testnum = 2;

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
    
    for (num = 0; num < 2; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void 
Buffer_Overflow(int num)
{
    int large[5016];
    memset(large, 0, sizeof(large));

    printf("*** 5016 the forked thread ***\n");
    //currentThread -> Yield();
}

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");
    //t->Fork(SimpleThread, t->getID());
    t-> Fork(Buffer_Overflow, 1); 
    SimpleThread(0);
}

//======================================================
void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");
    
    Thread *t[20];
    for(int i = 0; i < 20; i++)   
    {
        t[i] = new Thread("forked thread");
        
        t[i] -> setPriority((i * i) % 10 + 1);
        t[i] -> Fork(SimpleThread, t[i] -> getID());
    }
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
	ThreadTest1();
	break;
    case 2:
        ThreadTest2();
        break;
    default:
	printf("No test specified.\n");
	break;
    }
}

