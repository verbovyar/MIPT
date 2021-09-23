#include <unistd.h>
#include <sys/syscall.h>

long syscall(long number, ...);

const int MAX_SIZE = 1e6;

void _start()
{
    char string[MAX_SIZE];

    int strlen = 0;

    while((strlen = syscall(SYS_read, 0, string, MAX_SIZE)) > 0)
    {
        syscall(SYS_write, 1, string, strlen);

        // ++strlen;
    }

    syscall(SYS_exit, 0);
}