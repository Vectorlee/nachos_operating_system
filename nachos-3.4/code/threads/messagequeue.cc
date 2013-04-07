

#include "messagequeue.h"
#include "system.h"


MessageQueue::MessageQueue() {}

MessageQueue::~MessageQueue() {}

//
//get a key with a file name and id;
//        fname:  a file name
//          id:   an arbitrary number 
//                must be smaller than 255
//
key_t 
MessageQueue::ftok(const char* fname, int id)
{
     FILE * fp;
     int key;

     fp = fopen(fname,"r");

     if(fp == NULL)    //we must guarantee that the file exist
     {
         return 0xFFFFFFFF;  //return -1
     }
     
     fclose(fp);

     if(id > 256 || id < 0)      //ASSERT that the id is a small positive number 
         return 0xFFFFFFFF;

     key = 0;
     for(int i = 0; fname[i] != NULL; i++)
     {
         key += (int)fname[i]; 
     }

     id = id << 16;
     key += id;               //use the id and fname to create a unique number 
                              //use this number as message queue id
     return key;
}

//
//  create or get a message queue
//       key_t key : message queue id
//       msgflg :  the flag 
//
//       we now only defined the IPC_CREATE and IPC_PRIVATE
//

int 
MessageQueue::msgget(key_t key, int msgflg)
{

     int crt, priv;     
    
     crt = msgflg & IPC_CREATE;
     priv = msgflg & IPC_PRIVATE;

     if(systemQueue.find(key) != systemQueue.end())
     {
          return (int)systemQueue[key];   //if the msqid exist, return the exist id;
     }
     else
     {                          //if the msqid doesn't exist
          if(crt == 1)          //if the create signal is one
          {
              SynchList *list = new SynchList();       //create a new queue
              systemQueue.insert(std::make_pair<int, SynchList*>(key, list)); 
              
              return (int)list;  
          }  
          else  
              return 0xFFFFFFFF; 

      } 
}


int 
MessageQueue::msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg)
{

     SynchList *list = (SynchList*)msqid;
         
     int flag;

     flag = msgflg & NO_WAIT;
     
     //there should be some check about the length of the list
     //the message queue can not exceed some predefined number
     if(flag == 0)     
     {
          while(list -> totalNumber() == QUEUE_MAX)
          {
               currentThread -> Yield();  //we wait until we get another chance to run
          } 
     }
  
     
     if(list -> totalNumber() == QUEUE_MAX) 
         return 0xFFFFFFFF;          
     else 
     {
         list -> Append((void*)msgp);
         return 0x1;
     } 
}


int
MessageQueue::msgrcv(int msqid, void* msgp, size_t msgsz, long msgtyp, int msgflg)
{

      int flag;
      long type;
      bool find;

      SynchList *list;
      void* temp_msgp;


      list = (SynchList*) msqid;
      flag = msgflg & NO_WAIT;

      find = false;

      while(!list -> isEmpty())  //keep remove the message until we get a proper one
      {
           temp_msgp = list -> Remove();          
           memcpy(&type, temp_msgp, sizeof(long));  //get the type area of the message

           if(type == msgtyp)
           {
                find = true;
                break;
           }
      }

      if(find)
      {
            //we successfully found the message we want, memory copy
            memcpy(msgp, temp_msgp, msgsz);           
            return 0x1;
      }
      else
      {
            ASSERT(list -> isEmpty());               //the list is empty;
            if(flag == 0)   
            {    
                while(temp_msgp = list -> Remove())  //we should block to wait
                {
                    memcpy(&type, temp_msgp, sizeof(long));  //get the type area of the message
                    if(type == msgtyp)
                         break;
                }
                 
                memcpy(msgp, temp_msgp, msgsz);
                return 0x1;
            }
            else
                return 0xFFFFFFFF;  //we don't wait, directly return a false. 
      
      }         

}

