/*************************************************************************
    > File Name: threadpool.c
    > Author: tym
    > Mail: 
    > Created Time: Mon 12 Aug 2019 03:14:56 PM CST
 ************************************************************************/
#include "common.h"
#include "threadpool.h"
#define nil 0

void* worker(void* arg)
{
	struct timespec absTime;
	int timeout;
	printf("thread %d is starting\n", (int)pthread_self());
	threadpoolT* pool = (threadpoolT*)arg;
	while(1){
		
		pool->idle++;
		timeout = 0;
		conditionLock(&pool->ready);
		while(pool->first == NULL && !pool->quit)
		{
			printf("thread %d is waiting\n", (int)pthread_self());
			clock_gettime(CLOCK_REALTIME, &absTime);
			absTime.tv_sec += 2;
			int status;
			status = conditionTimeWait(&pool->ready, &absTime);
			if(status == ETIMEDOUT)
			{
				printf("thread %d time out!\n", (int)pthread_self());
				timeout = 1;
				break;
			}
		}
		pool->idle--;
		if(pool->first != NULL)
		{
			taskT* t = pool->first;
			pool->first = t->next;
			conditionUnlock(&pool->ready);
			t->run(t->arg);
			free(t);
			conditionLock(&pool->ready);
		}
		if(pool->quit && pool->first == NULL)
		{
			pool->counter--;
			if(pool->counter == 0)
			{
				conditionSignal(&pool->ready);
			}
			conditionUnlock(&pool->ready);
			break;
		}
		if(timeout == 1)
		{
			pool->counter--;
			conditionUnlock(&pool->ready);
			break;
		}
		conditionUnlock(&pool->ready);	
	}
	printf("thread %d is exiting\n", (int)pthread_self());
	return NULL;
}

void threadpoolInit(threadpoolT* pool, int threads)
{
	conditionInit(&pool->ready);
	pool->first = NULL;
	pool->last = NULL;
	pool->counter = 0;
	pool->idle = 0;
	pool->maxThreads = threads;
	pool->quit = 0;
}

void threadpoolAddTask(threadpoolT* pool, void* (*run)(void* arg), void* arg)
{
	taskT* newtask = (taskT*)malloc(sizeof(taskT));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next = nil;

	conditionLock(&pool->ready);
	if(pool->first == nil)
	{
		pool->first = newtask;
	}
	else
	{
		pool->last->next = newtask;
	}
	pool->last = newtask;
	if(pool->idle > 0)
	{
		conditionSignal(&pool->ready);
	}
	else if(pool->counter < pool->maxThreads)
	{
		pthread_t pid;
		pthread_create(&pid, nil, worker, pool);
		pool->counter++;
	}
	conditionUnlock(&pool->ready);
}

void threadpoolDestory(threadpoolT* pool)
{
	if(pool->quit)
		return;
	conditionLock(&pool->ready);
	pool->quit = 1;
	if(pool->counter > 0)
	{
		if(pool->idle > 0)
		{
			conditionBroadcast(&pool->ready);
			conditionUnlock(&pool->ready);
		}
		while(pool->counter)
		{
			conditionWait(&pool->ready);
		}
	}
	conditionUnlock(&pool->ready);
	conditionDestory(&pool->ready);
}
