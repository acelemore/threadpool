/*************************************************************************
    > File Name: threadpool.h
    > Author: tym
    > Mail: 
    > Created Time: Mon 12 Aug 2019 02:17:43 PM CST
 ************************************************************************/

#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include "condition.h"
typedef struct task
{
	void* (*run)(void* args);//执行函数函数指针
	void* arg;//参数
	struct task* next;//任务队列中下一个任务
}taskT;

typedef struct threadpool
{
	conditionT ready;
	taskT* first;
	taskT* last;
	int counter;
	int idle;
	int maxThreads;
	int quit;
}threadpoolT;

//线程池初始化
void threadpoolInit(threadpoolT* pool, int threads);

//线程池加入任务
void threadpoolAddTask(threadpoolT* pool, void* (*run)(void* arg), void* arg);

//销毁线程池
void threadpoolDestory(threadpoolT* pool);




#endif
