/*************************************************************************
    > File Name: condition.c
    > Author: tym
    > Mail: 
    > Created Time: Sat 10 Aug 2019 05:39:59 PM CST
 ************************************************************************/

#include"condition.h"

int conditionInit(conditionT* cond)
{
	int status;
	status = pthread_mutex_init(&cond->pMutex, NULL);
	if(status)
	{
		perror("mutex init failed: ");
		return status;
	}
	status = pthread_cond_init(&cond->pCond, NULL);
	if(status)
	{
		perror("cond init failed: ");
		return status;
	}
	return 0;
}

int conditionSignal(conditionT* cond)
{
	pthread_cond_signal(&cond->pCond);
}


int conditionLock(conditionT* cond)
{
	return pthread_mutex_lock(&cond->pMutex);
}

int conditionUnlock(conditionT* cond)
{
	return pthread_mutex_unlock(&cond->pMutex);
}

int conditionWait(conditionT* cond)
{
	return pthread_cond_wait(&cond->pCond, &cond->pMutex);
}

int conditionTimeWait(conditionT* cond, const struct timespec* absTime)
{
	return pthread_cond_timedwait(&cond->pCond, &cond->pMutex, absTime);
}

int conditionBroadcast(conditionT* cond)
{
	return pthread_cond_broadcast(&cond->pCond);
}

int conditionDestory(conditionT* cond)
{
	int status;
	status = pthread_mutex_destroy(&cond->pMutex);
	if(status)
	{
		perror("destory mutex failed:");
		return status;
	}
	status = pthread_cond_destroy(&cond->pCond);
	if(status)
	{
		perror("destory cond failed:");
		return status;
	}
	return 0;
}


