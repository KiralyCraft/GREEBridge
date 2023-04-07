/*
 * udp_receiver.h
 *
 *  Created on: Apr 6, 2023
 *      Author: kiraly
 */

#ifndef UDP_RECEIVER_H_
#define UDP_RECEIVER_H_

#include <stdint.h>
#include <arpa/inet.h>

typedef struct
{
	in_addr_t expectedSenderIP;
	uint32_t bufferSize;
} udp_receiver_proto;

void* udp_receiver_exec(void* theArguments);

#endif /* UDP_RECEIVER_H_ */
