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
 
int main(int argc, char *argv[])
{
    //声明sigaction结构体变量act
    struct sigaction act;
    //将信号处理函数存入结构体中
    act.sa_handler = alarmEvent;
    //将sa_mask成员的所有位初始化为0
    sigemptyset(&act.sa_mask);
    //将sa_flags成员同样初始化为0
    act.sa_flags = 0;
    //注册：触发条件为SIGALARM，调用act信号处理器中的信息
    sigaction(SIGALRM, &act, 0);
 
    alarm(2);
 
    for (size_t i = 0; i < 5; i++)
    {
        printf("Wait for an event to occur...\n");
        sleep(50);
    }
 
    return 0;
}