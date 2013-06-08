#include "syscall.h"

void Thread1()
{
    int j;
    for(j = 0; j < 10; j ++)
    {
        Print(1, "In the Thread 1\n");

        Yield();
    }

    Exit(0);
}

int main()
{
    
    Fork(Thread1);

    Yield();

    Print(1, "In the main\n");

    Exit(0);
}
