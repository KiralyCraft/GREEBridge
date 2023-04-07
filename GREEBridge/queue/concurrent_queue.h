/*
 * ll.h
 *
 *  Created on: Jan 7, 2023
 *      Author: kiraly
 */

#ifndef UTILS_QUEUE_QUEUE_H_
#define UTILS_QUEUE_QUEUE_H_

#include <stdint.h>
#include <pthread.h>

typedef struct
{
	void* theItem;
	void* next;
} conc_queue_item;

typedef struct
{
	conc_queue_item* lastItem;
	conc_queue_item* rootItem;
	uint32_t queueCount;
	pthread_mutex_t queueMutex;
	pthread_cond_t queuePopCondvar;
	pthread_cond_t queuePushCondvar;
} conc_queue;

void conc_queue_init(conc_queue** __theQueue);
void conc_queue_push(conc_queue* __theQueue, void* __theItem);
void* conc_queue_pop(conc_queue* __theQueue);
uint32_t conc_queue_count(conc_queue* __theQueue);
void conc_queue_destroy(conc_queue* __theQueue);


#endif /* UTILS_QUEUE_QUEUE_H_ */
