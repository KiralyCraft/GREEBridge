/*
 * main.c
 *
 *  Created on: Apr 6, 2023
 *      Author: kiraly
 */

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include "udp_receiver.h"
#include "udp_sender.h"
#include "udp_bridge.h"
#include "queue/concurrent_queue.h"


int thePortF;
void setReceivedPort(int thePort)
{
	printf("Called yay with %d\n",thePort);
	thePortF = thePort;
}

int getReceivedPort()
{
	printf("Returned %d\n",thePortF);
	return thePortF;
}

int main()
{

	udp_receiver_proto* receiverConfigA = malloc(sizeof(udp_receiver_proto));
	receiverConfigA -> bufferSize = 1024;
	receiverConfigA -> expectedSenderIP = 0;

	udp_sender_proto* senderConfigA = malloc(sizeof(udp_sender_proto));
	senderConfigA -> destHost = inet_addr("192.168.0.156");
	senderConfigA -> destPort = 7000;

	udp_sender_proto* senderConfigB = malloc(sizeof(udp_sender_proto));
	senderConfigB -> destHost = inet_addr("192.168.2.2");

	int _localSocketDescriptor = udp_bridge_createSocket(7000,INADDR_ANY);
	int _remoteSocketDescriptor = udp_bridge_createSocket(7001,INADDR_ANY);

	printf("%d %d\n",_localSocketDescriptor,_remoteSocketDescriptor);

	printf("Starting up the tunnel receiver thread\n");

	conc_queue* _theQueueA;
	conc_queue* _theQueueB;
	conc_queue_init(&_theQueueA);
	conc_queue_init(&_theQueueB);

	void** receiveSideArgumentsA = malloc(sizeof(void*)*4);
	receiveSideArgumentsA[0] = receiverConfigA;
	receiveSideArgumentsA[1] = _theQueueA;
	receiveSideArgumentsA[2] = &_localSocketDescriptor;
	receiveSideArgumentsA[3] = &setReceivedPort;

	void** sendSideArgumentsA = malloc(sizeof(void*)*4);
	sendSideArgumentsA[0] = senderConfigA;
	sendSideArgumentsA[1] = _theQueueA;
	sendSideArgumentsA[2] = &_remoteSocketDescriptor;
	sendSideArgumentsA[3] = NULL;

	void** receiveSideArgumentsB = malloc(sizeof(void*)*4);
	receiveSideArgumentsB[0] = receiverConfigA;
	receiveSideArgumentsB[1] = _theQueueB;
	receiveSideArgumentsB[2] = &_remoteSocketDescriptor;
	receiveSideArgumentsB[3] = NULL;

	void** sendSideArgumentsB = malloc(sizeof(void*)*4);
	sendSideArgumentsB[0] = senderConfigB;
	sendSideArgumentsB[1] = _theQueueB;
	sendSideArgumentsB[2] = &_localSocketDescriptor;
	sendSideArgumentsB[3] = &getReceivedPort;


	pthread_t AR,AS,BR,BS;

	pthread_create(&AR,NULL,udp_receiver_exec,receiveSideArgumentsA);
	pthread_create(&AS,NULL,udp_sender_exec,sendSideArgumentsA);
	pthread_create(&BR,NULL,udp_receiver_exec,receiveSideArgumentsB);
	pthread_create(&BS,NULL,udp_sender_exec,sendSideArgumentsB);

	pthread_join(AR,NULL);
}

