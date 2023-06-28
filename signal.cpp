#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void alarmEvent(int sig)
{
    if (sig == SIGALRM)
    {
        printf("Time out!\n");
    }

    alarm(2);
}

void keyEvent(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nCTRL+C pressed\n");
    }
}

int main(int argc, char *argv[])
{
    //设置SIGGALRM为触发条件，调用alarmEvent函数
    signal(SIGALRM, alarmEvent); 
    //设置SIGINT为触发条件，调用keyEvent函数
    signal(SIGINT, keyEvent);

    //设置时钟周期为2s
    alarm(2); 

    for (size_t i = 0; i < 5; i++)
    {
        printf("Wait for an event to occur...\n");
        sleep(50);  //实际操作上无法休眠50s
    }
int sigaction(int signo , const struct sigaction * act , struct sigaction *
oldact) ;

    return 0;
}