#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

void* thread_func2(void *arg);

int main(int argc, char *argv[]) 
{
	pthread_t t_id;
	int thread_cycle= 5;
	void * thr_ret;
	
	if(pthread_create(&t_id, NULL, thread_func2, (void*)&thread_cycle)!=0)
	{
		puts("pthread creation error");
		return -1;
	}; 	
    //等待线程t_id的执行完毕。完毕后该线程返回的数据将存放在thr_ret中
	if(pthread_join(t_id, &thr_ret)!=0) 
	{
		puts("pthread_join error");
		return -1;
	};
    //将线程执行完后的返回值输出。执行这条指令时线程已结束。
	printf("Thread return message: %s \n", (char*)thr_ret); 
	free(thr_ret);
	return 0;
}

void* thread_func2(void *arg) 
{
	int i;
	int cnt=*((int*)arg);
	char * msg=(char *)malloc(sizeof(char)*20);
	strcpy(msg, "Hello, I’m thread but now I am dead and buried. \n"); 

	for(i=0; i<cnt; i++)
	{
		sleep(1);  
        printf("cycle: %d \n",i);
	}

	return (void*)msg; //这是
}