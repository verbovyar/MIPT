#include <unistd.h>
#include <sys/syscall.h>

const int STRING_SIZE = 14;

long syscall(long number, ...);

void _start()
{
    static const char string[] = "Hello, World!";

    syscall(SYS_write, 1, string, sizeof(string) - 1);

    syscall(SYS_exit, 0);
}