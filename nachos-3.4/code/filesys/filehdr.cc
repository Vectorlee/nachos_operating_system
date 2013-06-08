// filehdr.cc 
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the 
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the 
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect 
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector, 
//
//      Unlike in a real system, we do not keep track of file permissions, 
//	ownership, last modification date, etc., in the file header. 
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "filehdr.h"

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------

bool
FileHeader::Allocate(BitMap *freeMap, int fileSize)
{ 
    numBytes = fileSize;
    numSectors  = divRoundUp(fileSize, SectorSize);
    if (freeMap->NumClear() < numSectors + 1)        // here we plus one, since the indirect index sector
	return FALSE;		// not enough space

    if(numSectors <= NumDirect )          // if the sector number of that file smaller than direct index slot
    {
        for (int i = 0; i < numSectors; i++)          // then we only need direct index 
            dataSectors[i] = freeMap->Find();

        indirect = -1;
    }
    else
    {
        int indirectNum = numSectors - NumDirect;
        ASSERT(indirectNum < NumInDirect);        

        for (int i = 0; i < NumDirect; i++)
            dataSectors[i] = freeMap->Find(); 

        indirect = freeMap->Find();

        IndirectIndex *indexptr = new IndirectIndex();        
        for (int i = 0; i < indirectNum; i++ ) 
            indexptr -> dataSectors[i] = freeMap -> Find();

        synchDisk->WriteSector(indirect, (char *)indexptr);   //write back
        
        delete indexptr; 
    }
    return TRUE;
}

//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

void 
FileHeader::Deallocate(BitMap *freeMap)
{
    if(numSectors <= NumDirect)
    { 
        for(int i = 0; i < numSectors; i++){       
            ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
	    freeMap->Clear((int) dataSectors[i]);
        }
    }
    else
    {
        int indirectNum = numSectors - NumDirect;
        ASSERT(indirectNum <= NumInDirect);        

        for (int i = 0; i < NumDirect; i++){
            ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
	    freeMap->Clear((int) dataSectors[i]); 
        }

        IndirectIndex* indexptr = new IndirectIndex();
        //here we read the index sector from the disk
        synchDisk->ReadSector(indirect, (char *)indexptr); 


        for (int i = 0; i < indirectNum; i++ ){ 
            ASSERT(freeMap->Test( (int)(indexptr -> dataSectors[i]) ));  // ought to be marked!
	    freeMap->Clear( (int)(indexptr -> dataSectors[i]) ); 
        }
        
        ASSERT(freeMap->Test(indirect));  // ought to be marked!
	freeMap->Clear(indirect);         // clean the index sector  
        
        indirect = -1;

        delete indexptr; 
    }

}

//------------------------------------------------------------
//FileHeader::InitialSet
//     Set the initial value of the file 
//
//-------------------------------------------------------------

void 
FileHeader::InitialSet(char *name, int type, int sector)
{
    //strncpy(filename, name, MaxFileName);
    //printf("%s\n", filename);

    this -> type        = type;                    // 0 is for file, 1 is for directory
    this -> createTime  = stats -> totalTicks;     //set the create time
    this -> path        = sector;
}


//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk. 
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector)
{
    synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk. 
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector)
{
    synchDisk->WriteSector(sector, (char *)this); 
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

int
FileHeader::ByteToSector(int offset)
{
    ASSERT(offset < numBytes);

    if( offset / SectorSize < NumDirect )
       return(dataSectors[offset / SectorSize]);
    else 
    {
       IndirectIndex *indexptr = new IndirectIndex();
       synchDisk->ReadSector(indirect, (char *)indexptr);

       int indexoffset = offset - NumDirect * SectorSize;
       int sector = indexptr -> dataSectors[indexoffset / SectorSize];
       delete indexptr;
       
       return sector;   
    } 

}

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength()
{
    return numBytes;
}

int 
FileHeader::FileSectorNum()
{
    return numSectors;
}


//----------------------------------------------------------------------
// FileHeader::ChangeLength
// 	
//     Set the length of the file, this function is used when we
//     increase or decrease the size of the file
//
//     This file should only be used by OpenFile
//----------------------------------------------------------------------

bool
FileHeader::ChangeLength(BitMap *freeMap, int newSize)
{
     int newSectorNum;
     newSectorNum = divRoundUp(newSize, SectorSize);

     ASSERT(newSize >= 0);

     if(newSize > MaxFileSize || (freeMap -> NumClear()) < (newSectorNum - numSectors))
        return FALSE;

     if(numSectors == newSectorNum)      //if the accural disk sector remain same, 
     {                                   //we don't need to any thing more.
         numBytes = newSize;
         return TRUE;
     }     
     else if(newSectorNum > numSectors)  // we increase the size of the file
     {

         IncreaseFile(freeMap, newSectorNum - numSectors);
         numSectors = newSectorNum;
         numBytes = newSize;
     } 
     else                // we decrease the size
     {
         DecreaseFile(freeMap, numSectors - newSectorNum); 
         numSectors = newSectorNum;
         numBytes = newSize;
     }

     return TRUE;
}

//----------------------------------------------------------------------
// FileHeader::IncreaseFile/DecreaseFile
// 	
//     Increase or decrease the length of the file, more precisely, 
//     increase or decrease the number of disk sectors allocated for 
//     that files, we will allocate new disk sectors for existing files,
//     or recycle the sectors which have been used
//
//          int sectors: the disk sectors we need to add or delete
//
//----------------------------------------------------------------------

void
FileHeader::IncreaseFile(BitMap *freeMap, int sectors)
{
     int i, pointer = 0;

     if(numSectors < NumDirect)    //no indirect index is used
     {
          for(i = numSectors; i < NumDirect && pointer < sectors; i++, pointer++)
               dataSectors[i] = freeMap->Find();
          
          if(pointer < sectors)
          {
               indirect = freeMap -> Find();
               
               IndirectIndex *indexptr = new IndirectIndex();        
               for (i = 0; i < NumInDirect && pointer < sectors; i++, pointer++ ) 
                   indexptr -> dataSectors[i] = freeMap -> Find();

               synchDisk->WriteSector(indirect, (char *)indexptr);   //write back
        
               delete indexptr;  
          }
     }
     else
     {    
          if(indirect == -1)
          {
               indirect = freeMap -> Find();
               
               IndirectIndex *indexptr = new IndirectIndex();        
               for (i = 0; i < NumInDirect && pointer < sectors; i++, pointer++ ) 
                   indexptr -> dataSectors[i] = freeMap -> Find();

               synchDisk->WriteSector(indirect, (char *)indexptr);   //write back
        
               delete indexptr;  
          }
          else
          {
               IndirectIndex *indexptr = new IndirectIndex();
               synchDisk->ReadSector(indirect, (char *)indexptr);

               for (i = numSectors - NumDirect; i < NumInDirect && pointer < sectors; i++, pointer++ ) 
                   indexptr -> dataSectors[i] = freeMap -> Find();

               synchDisk->WriteSector(indirect, (char *)indexptr);   //write back
               delete indexptr;

          }
     }
}


void
FileHeader::DecreaseFile(BitMap *freeMap, int sectors)
{
     int i, pointer = sectors - 1;

     if(numSectors <= NumDirect)                     //no indirect index is used
     {
          for(i = numSectors - 1; i >= 0 && pointer >= 0; i--, pointer--)
          {    
               ASSERT(freeMap->Test((int) dataSectors[i]));  // ought to be marked!
	       freeMap->Clear((int) dataSectors[i]);
          }
     }
     else
     {   
          IndirectIndex* indexptr = new IndirectIndex();
          synchDisk->ReadSector(indirect, (char *)indexptr); 

          for(i = numSectors - NumDirect - 1; i >= 0 && pointer >= 0; i--, pointer--)
          { 
              ASSERT(freeMap->Test( (int)(indexptr -> dataSectors[i]) ));  // ought to be marked!
	      freeMap->Clear( (int)(indexptr -> dataSectors[i]) ); 
          }
          //Here we don't need to write back

          if(pointer >= 0)
          {
              ASSERT(freeMap->Test(indirect));  // ought to be marked!
	      freeMap->Clear(indirect);         // clean the index sector  
        
              indirect = -1;
              delete indexptr; 

              for(i = NumDirect - 1; i >= 0 && pointer >= 0; i--, pointer--)
              {
                  ASSERT(freeMap->Test((int)dataSectors[i]));  // ought to be marked!
	          freeMap->Clear((int)dataSectors[i]); 
              } 
          } 
     }
}


//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void
FileHeader::Print()
{
    int i, j, k;
    char *data = new char[SectorSize];

    //printf("FileHeader contents.  File size: %d.  File blocks:\n", numBytes); 
    //for (i = 0; i < numSectors; i++)
    //	printf("%d ", dataSectors[i]);
    
    //printf("\nFile contents:\n");
    
    for (i = k = 0; i < numSectors; i++) {
	synchDisk->ReadSector(dataSectors[i], data);
        for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++) {
	    if ('\040' <= data[j] && data[j] <= '\176')   // isprint(data[j])
		printf("%c", data[j]);
            else
		printf("\\%x", (unsigned char)data[j]);
	}
        printf("\n"); 
    }
     
    delete [] data;

}
