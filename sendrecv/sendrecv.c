
/* Includes **************************************************************/

#include "sendrecv.h"
#include <stdio.h>
#include "../msgprotocol/msgprotocol.h"
#include <sys/socket.h>
#include <stdint.h>




/* Functions *************************************************************/

void sendrecv_send(protocol_t *packet,int socket_desc,char *serialize)
{
	int packet_lenght = 4+packet->len;

	msgprotocol_serialize(packet,serialize);
	if(send(socket_desc,serialize,packet_lenght,0) > 0)
	{
		printf("send packet\n");
	}
	else
	{
		printf("can't packet to client");
	}

}

//void sendrecv_recv(protocol_t *packet,int socket_desc,char *serialize)
//{
//
//	int packet_lenght = recv(socket_desc,serialize,100, 0);
//
//	if(packet_lenght > 0)
//	{
//		msgprotocol_parse(packet, serialize);
//	}
//	else
//	{
//
//		packet=NULL;
//	}
//
//}
