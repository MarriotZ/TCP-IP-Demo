#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CIRCLE 40

int main(int argc, char *argv[])
{
    int status;

    pid_t pid = fork();

    if (pid == 0) // 子进程进入
    {
        return 111;
    }
    else
    {
        printf("Child process ID: %d \n", pid);
        pid = fork();
        if (pid == 0)
        {
            exit(222);
        }
        else
        {
            printf("Child process ID: %d \n", pid);

            wait(&status);
            if (WIFEXITED(status))
            {
                printf("Child 1 return: %d \n", WEXITSTATUS(status));
            }

            wait(&status);
            if (WIFEXITED(status))
            {
                printf("Child 2 return: %d \n", WEXITSTATUS(status));
            }

            sleep(CIRCLE); // 休眠 40 s.
        }
    }
    return 0;
}