/*
 * udp_receiver.c
 *
 *  Created on: Apr 6, 2023
 *      Author: kiraly
 */

#include "udp_receiver.h"
#include "queue/concurrent_queue.h"
#include "udp_bridge_internal.h"
#include <string.h>
#include <stdlib.h>

void* udp_receiver_exec(void* __theArguments)
{
	void** _theActualArguments = __theArguments;
	udp_receiver_proto* _theProto = _theActualArguments[0];
	conc_queue* _theQueue = _theActualArguments[1];
	int _socketDescriptor = *(int*)_theActualArguments[2];
	void (*_setReceivedPortFunction)(int) = _theActualArguments[3];

	free(__theArguments); //Can clean up the temporary argument transporter

//	printf("DEBUG - Starting up the receiver on port %d UDP, accepting broadcasts\n",_theProto->port);

	void* _theBuffer = malloc(_theProto->bufferSize);

	struct sockaddr_in _remoteSender;
	unsigned int _remoteSenderLength = sizeof(_remoteSender);

	uint8_t _platformIntSize = sizeof(int);
	uint8_t _platformIntBuffer[_platformIntSize];

	while(1)
	{
		int _receiveResult = recvfrom(_socketDescriptor, _theBuffer, _theProto->bufferSize, 0, (struct sockaddr*) &_remoteSender, &_remoteSenderLength);
		if (_receiveResult < 0)
		{
			return 4;
		}
		else
		{
			uint8_t _allowProcessing = 0;
			if (_theProto->expectedSenderIP != 0)
			{
				if (_remoteSender.sin_addr.s_addr == _theProto->expectedSenderIP)
				{
					_allowProcessing = 1;
				}
			}
			else
			{
				_allowProcessing = 1;
			}

			if (_allowProcessing)
			{
				if (_setReceivedPortFunction != NULL)
				{
					_setReceivedPortFunction(ntohs(_remoteSender.sin_port));
				}

				udp_bridge_comm* _messageToPass = malloc(sizeof(udp_bridge_comm));
				_messageToPass->passedBuffer = malloc(_receiveResult);
				_messageToPass->passedBufferLength = _receiveResult;
				memcpy(_messageToPass->passedBuffer, _theBuffer, _receiveResult);

				conc_queue_push(_theQueue,_messageToPass);
				printf("Pushed message from  %s:%d\n",inet_ntoa(_remoteSender.sin_addr),ntohs(_remoteSender.sin_port));
			}
		}
	}

	return 0;

}
