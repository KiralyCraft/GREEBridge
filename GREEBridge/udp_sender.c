/*
 * udp_sender.c
 *
 *  Created on: Apr 6, 2023
 *      Author: kiraly
 */
#include "udp_sender.h"
#include "queue/concurrent_queue.h"
#include "udp_bridge_internal.h"
#include <string.h>
#include <stdlib.h>

void* udp_sender_exec(void* __theArguments)
{
	void** _theActualArguments = __theArguments;
	udp_sender_proto* _theProto = _theActualArguments[0];
	conc_queue* _theQueue = _theActualArguments[1];
	int _socketDescriptor = *(int*)_theActualArguments[2];
	int (*_getReceivedPortFunction)() = _theActualArguments[3];

	free(__theArguments); //Can clean up the temporary argument transporter

//	printf("DEBUG - Starting up the sender on port %d UDP, sending broadcasts",_theProto->port);



	struct sockaddr_in _targetAddr;
	_targetAddr.sin_family = AF_INET;
	_targetAddr.sin_addr.s_addr = _theProto->destHost;
	_targetAddr.sin_port = htons(_theProto->destPort);

	while(1)
	{
		udp_bridge_comm* _receivedComm = conc_queue_pop(_theQueue);

		if (_getReceivedPortFunction != NULL)
		{
			_targetAddr.sin_port = htons(_getReceivedPortFunction());
		}

		printf("Popped length: %d\n",_receivedComm->passedBufferLength);
		int _sentResult = sendto(_socketDescriptor, _receivedComm->passedBuffer, _receivedComm->passedBufferLength, 0, (struct sockaddr*) &_targetAddr, sizeof(_targetAddr));
		free(_receivedComm->passedBuffer);
		free(_receivedComm);

		if (_sentResult < 0)
		{
			return 4;
		}
		else
		{
			printf("Sent %d bytes to %s:%d\n",_sentResult,inet_ntoa(_targetAddr.sin_addr),ntohs(_targetAddr.sin_port));
		}
	}
}
