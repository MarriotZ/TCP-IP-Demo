#include <stdio.h>
#include <unistd.h>

int gval = 10;

int main(int argc, char *argv[])
{
    pid_t pid;
    int lval = 20;
    gval++;    // gval -> 11
    lval += 5; // lval -> 25

    pid = fork();
    if (pid == 0) // 子进程
    {
        gval += 1; // gval -> 12
        lval += 2; // lval -> 27
    }
    else // 父进程
    {
        gval -= 1; // gval -> 10
        lval -= 2; // lval -> 23
    }

    pid == 0 ? printf("Child Process: gval = %d, lval = %d \n", gval, lval)
             : printf("Parent Process: gval = %d, lval = %d \n", gval, lval);
    return 0;
}