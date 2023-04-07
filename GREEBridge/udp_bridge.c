/*
 * udp_bridge.c
 *
 *  Created on: Apr 7, 2023
 *      Author: kiraly
 */
#include <arpa/inet.h>
#include <stdint.h>

int udp_bridge_createSocket(uint16_t __thePort,in_addr_t __theHost)
{

	int _socketDescriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (_socketDescriptor < 0)
	{
		return -1;
	}
	else
	{
		int _optionEnabler = 1;
		if (setsockopt(_socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &_optionEnabler, sizeof(_optionEnabler)) < 0)
		{
			return -2;
		}
		else
		{
			struct sockaddr_in _localSocket;
			_localSocket.sin_family = AF_INET;
			_localSocket.sin_port = htons(__thePort);
			_localSocket.sin_addr.s_addr = __theHost;

			if (bind(_socketDescriptor, (struct sockaddr*) &_localSocket, sizeof(_localSocket)) < 0)
			{
				return -3;
			}
			else
			{
				return _socketDescriptor;
			}
		}
	}
}
