#include <stdio.h>
#include <unistd.h>

#define CIRCLE 40

int main(int argc, char *argv[])
{
    pid_t pid = fork();

    if (pid == 0) // 子进程
    {
        printf("I'm child process\n");
    }
    else
    {
        printf("Child Process ID: %d \n", pid);
        sleep(CIRCLE); // 休眠40s
    }

    pid == 0 ? printf("Child process end")
             : printf("Parent process end");

    return 0;
}