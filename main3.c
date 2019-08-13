/*************************************************************************
    > File Name: main3.c
    > Author: tym
    > Mail: 
    > Created Time: Tue 13 Aug 2019 10:31:17 AM CST
 ************************************************************************/
#include"common.h"
#include"threadpool.h"

void* myTask(void* arg)
{
	printf("thread %d is working on task %d\n", (int)pthread_self(), *(int*)arg);
	sleep(1);
	free(arg);
	return NULL;
}

int main()
{
	threadpoolT pool;
	threadpoolInit(&pool, 4);
	for(int i = 0; i < 10; i++)
	{
		int* arg = (int*)malloc(sizeof(int));
		*arg = i;
		threadpoolAddTask(&pool, myTask, arg);
	}
	threadpoolDestory(&pool);
	return 0;
}
