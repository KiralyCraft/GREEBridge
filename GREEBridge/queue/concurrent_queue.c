/*
 * ll.c
 *
 *  Created on: Jan 7, 2023
 *      Author: kiraly
 */

#include "concurrent_queue.h"

#include <stdlib.h>

void conc_queue_init(conc_queue** __theQueue)
{
	*__theQueue = (conc_queue*)malloc(sizeof(conc_queue));
	(*__theQueue)->queueCount = 0;
	(*__theQueue)->rootItem = NULL;
	(*__theQueue)->lastItem = NULL;

	pthread_mutex_init(&((*__theQueue)->queueMutex),NULL);
	pthread_cond_init(&((*__theQueue)->queuePopCondvar),NULL);
	pthread_cond_init(&((*__theQueue)->queuePushCondvar),NULL);
}
void conc_queue_push(conc_queue* __theQueue, void* __theItemToPush)
{
	pthread_mutex_lock (&__theQueue->queueMutex);
	conc_queue_item* _theItem = malloc(sizeof(conc_queue_item));
	_theItem->next = NULL;
	_theItem->theItem = __theItemToPush;

	if (__theQueue->rootItem == NULL)
	{
		__theQueue->rootItem = _theItem;
		__theQueue->lastItem = _theItem;
	}
	else
	{
		__theQueue->lastItem->next = _theItem;
		__theQueue->lastItem = _theItem;
	}

	__theQueue->queueCount++;
	pthread_cond_signal (&__theQueue->queuePopCondvar);
	pthread_mutex_unlock(&__theQueue->queueMutex);
}
void* conc_queue_pop(conc_queue* __theQueue)
{
	pthread_mutex_lock (&__theQueue->queueMutex);

	while (__theQueue->queueCount == 0)
	{
		pthread_cond_wait (&__theQueue->queuePopCondvar, &__theQueue->queueMutex);
	}

	conc_queue_item* _lastItem = __theQueue->rootItem;
	void* _toReturn = _lastItem->theItem;

	if (_lastItem -> next == NULL)
	{
		__theQueue->rootItem = NULL;
		__theQueue->lastItem = NULL;
	}
	else
	{
		__theQueue->rootItem = __theQueue->rootItem->next;
	}
	__theQueue->queueCount--;
	free(_lastItem);

	//TODO never used because there is no upper limit for this queue
//	pthread_cond_signal (&__theQueue->queuePushCondvar);
	pthread_mutex_unlock(&__theQueue->queueMutex);
	return _toReturn;
}

inline uint32_t conc_queue_count(conc_queue* __theQueue)
{
	return __theQueue -> queueCount;
}

void conc_queue_destroy(conc_queue* __theQueue)
{
	; //TODO this please, clean the queue and destroy the mutexes and conditional variables
}
