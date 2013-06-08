
#include "filemgr.h"


int sectorCheck;
Element *TempItem;

bool Find;


void CheckFile(int ptr)
{
    Element *element = (Element *)ptr;
    if(element -> sector == sectorCheck)
    {
        Find = TRUE;
        TempItem = element;
    }
}


FileManager::FileManager()
{
    fileList = new SynchList();

}


FileManager::~FileManager()
{
    delete fileList;
}

bool
FileManager::Append(int sector)
{
    sectorCheck = sector;
    Find = FALSE;
   
    fileList -> Mapcar(CheckFile); 

    if(Find)
    {   
        TempItem -> threadcount++;
        return FALSE;    // find the file, we don't append it 
    }
    else 
    {
        Element *element = new Element();
        element -> sector = sector;
        element -> mutex = new Lock("mutex");
        element -> threadcount = 1;

        fileList -> SortedInsert(element, sector);
        return TRUE;
    }
}

bool
FileManager::Remove(int sector)
{
    sectorCheck = sector;
    Find = FALSE;

    fileList -> Mapcar(CheckFile);

    ASSERT(Find);
    
    TempItem -> threadcount--;
    if(TempItem -> threadcount == 0)    // if the thread count equal 0, means no threads hold the file
    {                                   // we delete it
        Element *element = (Element*)fileList -> SortedRemove(sector);
  
        ASSERT(element != NULL);
        delete element;
    }

    return TRUE;
}

bool 
FileManager::LockFile(int sector)
{
    sectorCheck = sector;
    Find = FALSE;
    TempItem = NULL;

    fileList -> Mapcar(CheckFile);
    
    ASSERT(Find);

    ASSERT(TempItem != NULL);
    TempItem -> mutex -> Acquire();

    return TRUE;
}


bool 
FileManager::ReleaseFile(int sector)
{
    sectorCheck = sector;
    Find = FALSE;
    TempItem = NULL;

    fileList -> Mapcar(CheckFile);
    
    ASSERT(Find);

    ASSERT(TempItem != NULL);
    TempItem -> mutex -> Release();

    return TRUE;
}

int 
FileManager::CheckCount(int sector)
{

    sectorCheck = sector;
    Find = FALSE;
    TempItem = NULL;

    fileList -> Mapcar(CheckFile);
    
    //ASSERT(Find);          //we must find the file
    
    if(Find)
    {
        ASSERT(TempItem != NULL);
        return TempItem -> threadcount;    
    }
    else
        return 0; 

}

