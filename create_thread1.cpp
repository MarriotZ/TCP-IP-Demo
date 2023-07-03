#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thread_func1(void *arg);

int main(int argc, char *argv[])
{
    pthread_t t_id; // 线程的id，类比与进程号
    int thread_cycle = 10;

    if (pthread_create(&t_id, NULL, thread_func1, (void *)&thread_cycle /*注意要强转*/) != 0)
    {
        puts("pthread creation error");
        return -1;
    };
    sleep(11); //让Main函数停留 11 s，一般而言，等待事件设置的都比理论上的多1-2s，以保证能够执行完整
    puts("thread execution end");
    return 0;
}

void *thread_func1(void *arg)
{
    int i;
    int cnt = *((int *)arg); // 将指向void类型的指针类型转换为指向int类型的指针，然后再对其进行取值
    for (i = 0; i < cnt; i++)
    {
        sleep(1);
        printf("cycle: %d \n", i);
    }
    return NULL;
}