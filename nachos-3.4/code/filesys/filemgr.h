

#ifndef FILEMGR_H
#define FILEMGR_H

#include "synchlist.h"

class Element{
   public:
      int sector;
      int threadcount;
      Lock *mutex;
};
//use the sector number as the list key

class FileManager
{
     private:
         SynchList *fileList; 

     public:
         FileManager();  // {  filelist = new SyncList(); }

         ~FileManager();
 
         bool Append(int sector);  // the disk sector of the file header

         bool Remove(int sector);
         
         bool LockFile(int sector);

         bool ReleaseFile(int sector);

         int CheckCount(int sector);

};


#endif
