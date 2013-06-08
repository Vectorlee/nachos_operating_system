// filesys.cc 
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on dist--just like i-node in UNIX
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them 
//	on bootup. This is quite crucial.
//
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "bitmap.h"
#include "directory.h"
#include "filehdr.h"
#include "filesys.h"
#include "system.h"


// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known 
// sectors, so that they can be located on boot-up.
#define FreeMapSector 		0
#define DirectorySector 	1

// Initial file sizes for the bitmap and directory; until the file system
// supports extensible files, the directory size sets the maximum number 
// of files that can be loaded onto the disk.
#define FreeMapFileSize 	(NumSectors / BitsInByte)
#define NumDirEntries 		12                           //first is ".", the second is ".."
#define DirectoryFileSize 	(sizeof(DirectoryEntry) * NumDirEntries)


//----------------------------------------------------------------------
//RecursiveCopy
//
//----------------------------------------------------------------------

void
RecursiveCopy(BitMap *freeMap, int dir_sector, int newdir_sector)   //the file header location of the directory
{

    OpenFile *dirFile1 = new OpenFile(dir_sector);         // old directory file, new directory file.
    Directory *dir1 = new Directory(NumDirEntries);        // the directory entry of the new directory 
    dir1 -> FetchFrom(dirFile1);                           // haven't been set yet. 
    
    OpenFile *dirFile2 = new OpenFile(newdir_sector);
    Directory *dir2 = new Directory(NumDirEntries);
    dir2 -> FetchFrom(dirFile2);                           // new directory file.
    

    FileHeader *dir_hdr = new FileHeader();
    dir_hdr -> FetchFrom(newdir_sector);

    dir2 -> Change(".", newdir_sector);
    dir2 -> Change("..", dir_hdr -> path);
    delete dir_hdr;


    for(int i = 2; i < NumDirEntries; i++)  //we need a registeration. start from number 2.
    {
        int old_sector = dir1 -> GetEntry(i);     //the sector number of the correspond file.
        if(old_sector == -1)
           continue;

        FileHeader *hdr = new FileHeader();
        hdr -> FetchFrom(old_sector);  


        //first, create a file header for that file.
        int new_sector = freeMap -> Find();
        int sectorNum = hdr -> FileSectorNum();
        int type = hdr -> type;

        FileHeader *new_hdr = new FileHeader();
        new_hdr -> InitialSet(NULL, hdr -> type, newdir_sector);   //set the initial value of the header.
        new_hdr -> Allocate(freeMap, hdr -> FileLength());
        new_hdr -> WriteBack(new_sector);


        dir2 -> SetEntry(i, new_sector);   //set the directory entry for that file.
        //then, copy the content of the file to the new file.
        OpenFile *new_file = new OpenFile(new_sector);
        OpenFile *old_file = new OpenFile(old_sector);

        int  index = 0;
        char Buffer[SectorSize];

        while(index < sectorNum)  // copy the whole file.
        {
            old_file -> Read(Buffer, SectorSize);
            new_file -> Write(Buffer, SectorSize);
                
            index++;
        }
        delete old_file;
        delete new_file;
        delete hdr; 
        delete new_hdr;

        if(type == 1)   // if the file is a directory
        {
            RecursiveCopy(freeMap, old_sector, new_sector);
        }
        
    }
    dir2 -> WriteBack(dirFile2);

    delete dirFile1;
    delete dirFile2;
    delete dir1;
    delete dir2;
}

//-----------------------------------------------------------------
//RecursiveDelete
//
//-----------------------------------------------------------------

void RecursiveDelete(char *path)
{
    char buffer[0x100];
    int sector = fileSystem -> FindFile(path);
    
    ASSERT(sector != -1);

    OpenFile *dirFile = new OpenFile(sector);
    Directory *directory = new Directory(NumDirEntries);
    directory -> FetchFrom(dirFile);

    for(int i = 2; i < NumDirEntries; i++)
    {
        int old_sector = directory -> GetEntry(i);     //the sector number of the correspond file.
        if(old_sector == -1)
           continue;

        char *filename = directory -> GetName(i);
        ASSERT(filename != NULL);

        FileHeader *hdr = new FileHeader();
        hdr -> FetchFrom(old_sector);

        if(hdr -> type == 0)  
        {
            fileSystem -> Remove(filename, path);
        }
        else
        {
            strncpy(buffer, path, 0x100);
            strcat(buffer, "/");
            strcat(buffer, filename);

            RecursiveDelete(buffer);
            fileSystem -> Remove(filename, path);
        }

        delete hdr;
    }

    delete dirFile;
    delete directory;
}


//----------------------------------------------------------------------
// FileSystem::FileSystem
// 	Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).  
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{ 
    DEBUG('f', "Initializing the file system.\n");
    if (format) {
        BitMap *freeMap = new BitMap(NumSectors);
        Directory *directory = new Directory(NumDirEntries);
	FileHeader *mapHdr = new FileHeader;
	FileHeader *dirHdr = new FileHeader;

        DEBUG('f', "Formatting the file system.\n");

    // First, allocate space for FileHeaders for the directory and bitmap
    // (make sure no one else grabs these!)
	freeMap->Mark(FreeMapSector);	    
	freeMap->Mark(DirectorySector);

    // Second, allocate space for the data blocks containing the contents
    // of the directory and bitmap files.  There better be enough space!

	ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));
	ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize));   // we should set it as 12

    // Flush the bitmap and directory FileHeaders back to disk
    // We need to do this before we can "Open" the file, since open
    // reads the file header off of disk (and currently the disk has garbage
    // on it!).

        DEBUG('f', "Writing headers back to disk.\n");
	mapHdr->WriteBack(FreeMapSector);

        dirHdr->InitialSet(NULL, 1, DirectorySector);
	dirHdr->WriteBack(DirectorySector);

    // OK to open the bitmap and directory files now
    // The file system operations assume these two files are left open
    // while Nachos is running.

        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
    
    // Here we add two directory entry for the root directory. the ".", ".."

        directory -> Add(".", DirectorySector);
        directory -> Add("..", DirectorySector);

    // Once we have the files "open", we can write the initial version
    // of each file back to disk.  The directory at this point is completely
    // empty; but the bitmap has been changed to reflect the fact that
    // sectors on the disk have been allocated for the file headers and
    // to hold the file data for the directory and bitmap.

        DEBUG('f', "Writing bitmap and directory back to disk.\n");
	freeMap->WriteBack(freeMapFile);	 // flush changes to disk
	directory->WriteBack(directoryFile);

	if (DebugIsEnabled('f')) {
	    freeMap->Print();
	    directory->Print();

        delete freeMap; 
	delete directory; 
	delete mapHdr; 
	delete dirHdr;
	}
    } else {
    // if we are not formatting the disk, just open the files representing
    // the bitmap and directory; these are left open while Nachos is running
        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
    }

}

//----------------------------------------------------------------------
// FileSystem::Create
// 	Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//	  Make sure the file doesn't already exist
//        Allocate a sector for the file header
// 	  Allocate space on disk for the data blocks for the file
//	  Add the name to the directory
//	  Store the new file header on disk 
//	  Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
// 	Create fails if:
//   		file is already in directory
//	 	no free space for file header
//	 	no free entry for file in directory
//	 	no free space for data blocks for the file 
//
// 	Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

bool
FileSystem::Create(char *name, int initialSize, const char *path)
{
    Directory *directory;
    BitMap *freeMap;
    FileHeader *hdr;
    int sector;
    bool success;
    OpenFile *dirFile;

    DEBUG('f', "Creating file %s, size %d\n", name, initialSize);

    directory = new Directory(NumDirEntries);   
    if(path == NULL)
        directory->FetchFrom(directoryFile);
    else
    {
        sector = this -> FindFile(path);
        if(sector == -1)
            return FALSE;

        dirFile = new OpenFile(sector);
        directory -> FetchFrom(dirFile);
    }    


    if (directory->Find(name) != -1)
      success = FALSE;			// file is already in directory
    else {	
        freeMap = new BitMap(NumSectors);
        freeMap->FetchFrom(freeMapFile);
        sector = freeMap->Find();	// find a sector to hold the file header
    	if (sector == -1) 		
            success = FALSE;		// no free block for file header 
        else if (!directory->Add(name, sector))
            success = FALSE;	// no space in directory
	else {
    	    hdr = new FileHeader;
	    if (!hdr->Allocate(freeMap, initialSize))
            	success = FALSE;	// no space on disk for data
	    else {	
	    	success = TRUE;
		// everthing worked, flush all changes back to disk
                hdr->InitialSet(name, 0, directoryFile -> FileSector());   // 0 is file, 1 is directory

                hdr->WriteBack(sector);
    	    	freeMap->WriteBack(freeMapFile);
                
                if(path == NULL)
                    directory -> WriteBack(directoryFile);        // flush to disk
                else
                {
                    directory -> WriteBack(dirFile);
                    delete dirFile;
                }
	    }
            delete hdr;
	}
        delete freeMap;
    }
    delete directory;
    return success;
}


//----------------------------------------------------------------------
// FileSystem::CreateDirectory
// 	Create a directory in the Nachos file system.
//
//	"name" -- name of file to be created
//      the initial size of the directory is two
//          one for "."
//          one for ".." 
//----------------------------------------------------------------------


bool
FileSystem::CreateDirectory(char *name, const char *path)
{
    Directory *directory;
    BitMap *freeMap;
    FileHeader *hdr;
    int sector;
    bool success;
    OpenFile *dirFile;

    DEBUG('f', "Creating file %s, size %d\n", name, DirectoryFileSize);

    directory = new Directory(NumDirEntries);
    if(path == NULL)
        directory->FetchFrom(directoryFile);
    else
    {
        sector = this -> FindFile(path);
        if(sector == -1)
            return FALSE;

        dirFile = new OpenFile(sector);
        directory -> FetchFrom(dirFile);
    } 


    if (directory->Find(name) != -1)
      success = FALSE;			// file is already in directory
    else {	
        freeMap = new BitMap(NumSectors);
        freeMap->FetchFrom(freeMapFile);
        sector = freeMap->Find();	// find a sector to hold the file header
    	if (sector == -1) 		
            success = FALSE;		// no free block for file header 
        else if (!directory->Add(name, sector))
            success = FALSE;	// no space in directory
	else {
    	    hdr = new FileHeader;
	    if (!hdr->Allocate(freeMap, DirectoryFileSize))
            	success = FALSE;	// no space on disk for data
	    else {	
	    	success = TRUE;
//=================================================================================

                hdr -> InitialSet(name, 1, directoryFile -> FileSector());   // 0 is file, 1 is directory                
                hdr -> WriteBack(sector);

                OpenFile  *newdirfile = new OpenFile(sector);
                Directory *newdir = new Directory(NumDirEntries);
                //newdir -> FetchFrom(newdirfile);

                newdir -> Add(".", sector);
                newdir -> Add("..", directoryFile -> FileSector() );
         
                newdir -> WriteBack(newdirfile);

                delete newdirfile;
                delete newdir;
                
//==================================================================================
   
                hdr -> WriteBack(sector);
    	    	freeMap->WriteBack(freeMapFile);

                if(path == NULL)
                    directory -> WriteBack(directoryFile);        // flush to disk
                else
                {
                    directory -> WriteBack(dirFile);
                    delete dirFile;
                }
	    }
            delete hdr;
	}
        delete freeMap;
    }
    delete directory;
    return success;
}


//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading and writing.  
//	To open a file:
//	  Find the location of the file's header, using the directory 
//	  Bring the header into memory
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------

OpenFile *
FileSystem::Open(char *name)
{ 
    Directory *directory = new Directory(NumDirEntries);
    OpenFile *openFile = NULL;
    int sector;

    DEBUG('f', "Opening file %s\n", name);
    //sector = FindFile(name);
    directory->FetchFrom(directoryFile);
    sector = directory->Find(name); 
    if (sector >= 0) 		
	openFile = new OpenFile(sector);	// name was found in directory 
    delete directory;
    return openFile;				// return NULL if not found
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//	    Delete the space for its header
//	    Delete the space for its data blocks
//	    Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

bool
FileSystem::Remove(char *name, const char *path)
{
    Directory *directory;
    BitMap *freeMap;
    FileHeader *fileHdr;
    OpenFile *dirFile = NULL;

    int sector;

    directory = new Directory(NumDirEntries);
    if(path != NULL)
    {   
        sector = FindFile(path);
        if(sector == -1)
            return FALSE;

        dirFile = new OpenFile(sector);
        directory->FetchFrom(dirFile);
    }
    else
    {
        directory->FetchFrom(directoryFile);
    }
    sector = directory->Find(name);


    if (sector == -1) {
       delete directory;
       return FALSE;			 // file not found 
    }

    if (fileManager -> CheckCount(sector) > 0)
    {
        delete directory; 
        printf("[-] There is some thread open the file!");
        return FALSE; 
    }

    fileHdr = new FileHeader;
    fileHdr->FetchFrom(sector);

    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);

    fileHdr->Deallocate(freeMap);  		// remove data blocks
    freeMap->Clear(sector);			// remove header block
    directory->Remove(name);

    freeMap->WriteBack(freeMapFile);		// flush to disk

    if(path != NULL)
    {
        directory -> WriteBack(dirFile);
        delete dirFile;
    }
    else
        directory -> WriteBack(directoryFile);        // flush to disk
    

    delete fileHdr;
    delete directory;
    delete freeMap;
    return TRUE;
} 

//---------------------------------------------------------------------
//FileSystem::ChangeFileLength
//    Change the length of a specific file
//        FileHeader hdr    the file we tend to change
//
//    we need to write back the file header after we have change the
//    file size. 
//---------------------------------------------------------------------

bool 
FileSystem::ChangeFileLength(FileHeader *hdr, int newSize)  
{
     BitMap *freeMap;
     freeMap = new BitMap(NumSectors);
     freeMap -> FetchFrom(freeMapFile);

     bool mark = hdr -> ChangeLength(freeMap, newSize);

     freeMap -> WriteBack(freeMapFile);        // flush to disk
     delete freeMap;

     return mark;
}

//-----------------------------------------------------------------------
//FileSystem::ChangeDirectory
//
//     Change the current working directory
//        int sector, the disk sector which holds the directory file header
//        
//     This will implement the function of "cd" 
//     but we don't make it complicated, since that should be the work of shell
//------------------------------------------------------------------------

bool 
FileSystem::ChangeDirectory(int sector)   // the sector can be get through directory::Find
{

   ASSERT(directoryFile != NULL);
   delete directoryFile;

   directoryFile = new OpenFile(sector);

   return TRUE;
}

//---------------------------------------------------------------------
//FileSystem::FindFile(char *path)
//       Give a path, find the file header sector of that path
//---------------------------------------------------------------------

int
FileSystem::FindFile(const char *strpath)
{
    OpenFile *dirFile;
    Directory *directory;
    int sector;
    char *tokenPtr, path[0x100];
 
    if(strpath == NULL)
       return -1;

    strncpy(path, strpath, 0x100);
    if(path[0] == '/')      // absolute address
    {
        dirFile = new OpenFile(DirectorySector);   // we don't need write back,
                                                   // since we will not change it.
        directory = new Directory(NumDirEntries);
        directory -> FetchFrom(dirFile);

        sector = DirectorySector;  // the initial sector

        tokenPtr = strtok(path, "/");
        while(tokenPtr != NULL)
        {
             delete dirFile;
             
             sector = directory -> Find(tokenPtr);
             if(sector == -1)
                return -1;

             tokenPtr = strtok(NULL, "/"); 
             dirFile = new OpenFile(sector);
             
             if(tokenPtr != NULL)
             {
                 if(dirFile -> FileType() == 0)   //this is a file, not a directory
                      return -1;

                 directory -> FetchFrom(dirFile);
             }
        }
        delete dirFile;
        delete directory;

        return sector;  
    }
    else                    // relative address
    {
        sector = (this -> directoryFile) -> FileSector();
        dirFile = new OpenFile(sector);   //the current directory

        directory = new Directory(NumDirEntries);
        directory -> FetchFrom(dirFile);

        tokenPtr = strtok(path, "/");
        while(tokenPtr != NULL)
        {
             delete dirFile;
             
             sector = directory -> Find(tokenPtr);
             if(sector == -1)
                return -1;

             tokenPtr = strtok(NULL, "/"); 
             dirFile = new OpenFile(sector);
             
             if(tokenPtr != NULL)
             {
                 if(dirFile -> FileType() == 0)   //this is a file, not a directory
                      return -1;

                 directory -> FetchFrom(dirFile);
             }
        }
        delete dirFile;
        delete directory;

        return sector; 
    } 
}

//---------------------------------------------------------------------
//FileSystem::MoveFile
//     Move file from one place to another
//---------------------------------------------------------------------

void
FileSystem::MoveFile(char *from, char* go, char *oldname, char* newname)
{

    OpenFile *dirFile1 = new OpenFile(this -> FindFile(from));
    Directory *dir1 = new Directory(NumDirEntries);
    dir1 -> FetchFrom(dirFile1);

    int sector = dir1 -> Find(oldname);
    dir1 -> Remove(oldname);
    dir1 -> WriteBack(dirFile1);

    int dirsector = this -> FindFile(go);
    OpenFile *dirFile2 = new OpenFile(dirsector);
    Directory *dir2 = new Directory(NumDirEntries);
    dir2 -> FetchFrom(dirFile2);

    dir2 -> Add(newname, sector);
    dir2 -> WriteBack(dirFile2);

    FileHeader *hdr = new FileHeader();
    hdr -> FetchFrom(sector);
    hdr -> path = dirsector;
    hdr -> WriteBack(sector);

    if(hdr -> type == 1)
    {
        Directory *dir3 = new Directory(NumDirEntries);
        OpenFile *openfile = new OpenFile(sector);
        dir3 -> FetchFrom(openfile);
        
        dir3 -> Change(".", sector);
        dir3 -> Change("..", dirsector);
        dir3 -> WriteBack(openfile);

        delete openfile;
        delete dir3;
    }
    
    delete hdr;
    delete dirFile1;
    delete dirFile2;
    delete dir1;
    delete dir2;
}

void
FileSystem::CopyFile(char *from, char* go, char *oldname, char* newname)
{
    BitMap *freeMap;
    freeMap = new BitMap(NumSectors);
    freeMap -> FetchFrom(freeMapFile);


    OpenFile *dirFile1 = new OpenFile(this -> FindFile(from));
    Directory *dir1 = new Directory(NumDirEntries);
    dir1 -> FetchFrom(dirFile1);

    int dirsector = this -> FindFile(go);
    OpenFile *dirFile2 = new OpenFile(dirsector);
    Directory *dir2 = new Directory(NumDirEntries);
    dir2 -> FetchFrom(dirFile2);


    int old_sector = dir1 -> Find(oldname);   //sector:: the old sector
    FileHeader *hdr = new FileHeader();
    hdr -> FetchFrom(old_sector);

   
    //first, create a file header for that file.
    int new_sector = freeMap -> Find();
    int sectorNum = hdr -> FileSectorNum();
    int type = hdr -> type;

    FileHeader *new_hdr = new FileHeader();
    new_hdr -> InitialSet(NULL, hdr -> type, dirsector);   //set the initial value of the header.
    new_hdr -> Allocate(freeMap, hdr -> FileLength());
    new_hdr -> WriteBack(new_sector);


    dir2 -> Add(newname, new_sector);  // add the new file to the directory.
    //then, copy the content of the file to the new file.
    OpenFile *new_file = new OpenFile(new_sector);
    OpenFile *old_file = new OpenFile(old_sector);

    int  index = 0;
    char Buffer[SectorSize];

    while(index < sectorNum)  // copy the whole file.
    {
         old_file -> Read(Buffer, SectorSize);
         new_file -> Write(Buffer, SectorSize);
                
         index++;
    }
    delete old_file;
    delete new_file;
    delete new_hdr;
    delete hdr; 


    if(type == 1)   //directory
    {
        RecursiveCopy(freeMap, old_sector, new_sector);
    }
    
    dir2 -> WriteBack(dirFile2);
    freeMap -> WriteBack(freeMapFile);
    
    delete freeMap;
    delete dirFile1;
    delete dirFile2;
    delete dir1;
    delete dir2; 
}

//-----------------------------------------------------------------------
//FileSystem::RemoveDirectory
//    Remove the directory and all the files in it
//-----------------------------------------------------------------------


bool
FileSystem::RemoveDirectory(char *name)
{
    int sector = FindFile(name);

    FileHeader *hdr = new FileHeader();
    hdr -> FetchFrom(sector);

    ASSERT(hdr -> type == 1);

 
    RecursiveDelete(name);

    return TRUE;
}


//---------------------------------------------------------------------
//FileSystem::WorkingDirectory
//      Print the current working directory.  'pwd'
//---------------------------------------------------------------------

void
FileSystem::WorkingDirectory()
{
    int i, index, sector, path, entry;
    char tree[0x100][0x10];

    index = 0;
    path = directoryFile -> FilePath();
    sector = directoryFile -> FileSector();
   
    while(sector != DirectorySector)
    {
         FileHeader *hdr = new FileHeader();
         hdr -> FetchFrom(sector);
         ASSERT(hdr -> type == 1);     //assert the file is directory.

         OpenFile *dirFile = new OpenFile(path);
         Directory *dir = new Directory(NumDirEntries);
         dir -> FetchFrom(dirFile);

         for(i = 2; i < NumDirEntries; i++)
         {
             entry = dir -> GetEntry(i);
             if(entry == sector)
             {
                 char *filename = dir -> GetName(i);
                 strcpy(tree[index++], filename); 
 
                 break;
             }
         }
 
         sector = path;   
         path = hdr -> path;
    }

    printf("/");

    index--; 
    while(index >= 0)
    {
        printf("%s/", tree[index]);
        index--;
    } 
    printf("\n");

}


//----------------------------------------------------------------------
// FileSystem::List
// 	List all the files in the file system directory.
//----------------------------------------------------------------------

void
FileSystem::List()
{
    Directory *directory = new Directory(NumDirEntries);
    directory->FetchFrom(directoryFile);

    directory->List();
    delete directory;
}

//----------------------------------------------------------------------
// FileSystem::Print
// 	Print everything about the file system:
//	  the contents of the bitmap
//	  the contents of the directory
//	  for each file in the directory,
//	      the contents of the file header
//	      the data in the file
//----------------------------------------------------------------------

void
FileSystem::Print()
{
    FileHeader *bitHdr = new FileHeader;
    FileHeader *dirHdr = new FileHeader;
    BitMap *freeMap = new BitMap(NumSectors);
    Directory *directory = new Directory(NumDirEntries);

    printf("Bit map file header:\n");
    bitHdr->FetchFrom(FreeMapSector);
    bitHdr->Print();

    printf("Directory file header:\n");
    dirHdr->FetchFrom(DirectorySector);
    dirHdr->Print();

    freeMap->FetchFrom(freeMapFile);
    freeMap->Print();

    directory->FetchFrom(directoryFile);
    directory->Print();

    delete bitHdr;
    delete dirHdr;
    delete freeMap;
    delete directory;
} 
