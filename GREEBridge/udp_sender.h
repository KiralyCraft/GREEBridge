/*
 * udp_sender.h
 *
 *  Created on: Apr 6, 2023
 *      Author: kiraly
 */

#ifndef UDP_SENDER_H_
#define UDP_SENDER_H_

#include <stdint.h>
#include <arpa/inet.h>

typedef struct
{
	in_addr_t destHost;
	uint32_t destPort;
} udp_sender_proto;

void* udp_sender_exec(void* __theArguments);

#endif /* UDP_SENDER_H_ */
