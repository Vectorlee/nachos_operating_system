// filehdr.h 
//	Data structures for managing a disk file header.  
//
//	A file header describes where on disk to find the data in a file,
//	along with other information about the file (for instance, its
//	length, owner, etc.)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#ifndef FILEHDR_H
#define FILEHDR_H

#include "disk.h"
#include "bitmap.h"

#define NumDirect 	((SectorSize - 32) / sizeof(int))
#define NumInDirect     SectorSize / sizeof(int)

//#define MaxFileName     31
#define MaxFileSize 	( NumDirect + 1 * NumInDirect ) * SectorSize

#define TYPE_FILE       0
#define TYPE_DIRECTORY  1


// This class defines the indirect index of a file, here we only 
// implement a single indirect file index, to extand the max size of 
// one file. 
//

class IndirectIndex
{
    public:
       int dataSectors[NumInDirect];   //just enough to fill a whole disk sector
};


// The following class defines the Nachos "file header" (in UNIX terms,  
// the "i-node"), describing where on disk to find all of the data in the file.
// The file header is organized as a simple table of pointers to
// data blocks. 
//
// The file header data structure can be stored in memory or on disk.
// When it is on disk, it is stored in a single sector -- this means
// that we assume the size of this data structure to be the same
// as one disk sector.  Without indirect addressing, this
// limits the maximum file length to just under 4K bytes.
//
// There is no constructor; rather the file header can be initialized
// by allocating blocks for the file (if it is a new file), or by
// reading it from disk.


class FileHeader {
  public:
    bool Allocate(BitMap *bitMap, int fileSize);//  Initialize a file header, 
						//  including allocating space 
						//  on disk for the file data
    void Deallocate(BitMap *bitMap);  		//  De-allocate this file's 
						//  data blocks

    void InitialSet(char *name, int type, int sector);

    void FetchFrom(int sectorNumber); 	// Initialize file header from disk
    void WriteBack(int sectorNumber); 	// Write modifications to file header
					// back to disk

    int ByteToSector(int offset);	// Convert a byte offset into the file
					// to the disk sector containing
					// the byte

    int FileLength();			// Return the length of the file 
					// in bytes
    int FileSectorNum();                // Return the sector number of the file.

    bool ChangeLength(BitMap *bitMap, int newSize);      // change the size of file

    void Print();			// Print the contents of the file.

  private:

    void IncreaseFile(BitMap *bitMap, int sectors);   // those two functions are private.
    void DecreaseFile(BitMap *bitMap, int sectors);   // these two functions should only be called by ChangeLength
  
  public: 
                                                   
    int type;                             // 4 byte   // 0 is for file, 1 is for directory

    int createTime;
    int accessTime;
    int modifiedTime;                     // 12 btye
    
    // the path of the file, here we only provide 
    // the father directory of the file, the file header place
    // of the father directory. sector
    
    int path;                             // 4 byte    20btye
  
  private:

    int numBytes;			// Number of bytes in the file
    int numSectors;			// Number of data sectors in the file
                                        // 28 byte 

    int dataSectors[NumDirect];		// Disk sector numbers for each data 
					// block in the file

    int indirect;                       // 32 the sector number of the indirect index

};


#endif // FILEHDR_H
