/* sort.c 
 *    Test program to sort a large number of integers.
 *
 *    Intention is to stress virtual memory system.
 *
 *    Ideally, we could read the unsorted array off of the file system,
 *	and store the result back to the file system!
 */

#include "syscall.h"

char A[256];	/* size of physical memory; with code, we'll run out of space!*/
char file[] = "temp";


int
main()
{
    int i, id;

    /* first initialize the array, in reverse sorted order */
    for (i = 0; i < 256; i++)		
    {
          A[i] = 'A';
    }

    Create(file);
    id = Open(file);

    Write(A, 256, id);
    Close(id);

    Exit(A[0]);		/* and then we're done -- should be 0! */
}
