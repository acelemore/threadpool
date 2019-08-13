/*************************************************************************
    > File Name: condition.h
    > Author: tym
    > Mail: 
    > Created Time: Sat 10 Aug 2019 05:11:16 PM CST
 ************************************************************************/

#ifndef _CONDITION_H
#define _CONDITION_H

#include<pthread.h>
#include<stdio.h>

typedef struct condition
{
	pthread_mutex_t pMutex;
	pthread_cond_t pCond;
}conditionT;

int conditionInit(conditionT* cond);
int conditionLock(conditionT* cond);
int conditionUnlock(conditionT* cond);
int conditionWait(conditionT* cond);
int conditionTimeWait(conditionT* cond, const struct timespec* absTime);
int conditionSignal(conditionT* cond);
int conditionBroadcast(conditionT* cond);
int conditionDestory(conditionT* cond);


#endif
