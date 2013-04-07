//messagequeue.h
//
//    This messagequeue.h defined a class about the Message queue
//
//    This data structure serve for the intercommunication between
//    different threads. One thread send message to the queue and 
//    another thread, who have shared the same key with the foregoing
//    thread, retreive the message from the queue.
//
//
#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H


#include <map>
#include "synchlist.h"
#include "stdio.h"
#include "thread.h"


#define IPC_CREATE    0x00000001
#define IPC_PRIVATE   0x00000010

#define NO_WAIT       0x00000001

#define QUEUE_MAX      100

typedef int key_t;

class MessageQueue
{

     private:
             std::map<int, SynchList*> systemQueue;

     public:
             MessageQueue();

             ~MessageQueue();

             //get a unique key of the message queue
             key_t ftok(const char* fname, int id);
             
             //get the message queue id with the key, if there isn't it, we create one
             int msgget(key_t key, int msgflg);

             //send a message to the message queue defined by msqid 
             int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

             //receive a message from the message queue defined by msqid 
             int msgrcv(int msqid, void* msgp, size_t msgsz, long msgtyp, int msgflg);            

};


#endif
