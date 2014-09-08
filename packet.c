#include "packet.h"
#include <stdio.h>
int get_start_packet(PACKET *pk_start,char *addr){
	memcpy(pk_start->dstmac,mac_8021x_standard,6);
	memcpy(pk_start->srcmac,addr,6);
	pk_start->auth_type[0]=0x88;
	pk_start->auth_type[1]=0x8e;
	pk_start->version=0x01;
	pk_start->pkt_type=PACKET_TYPE_START;
	memset(pk_start->length,0,2);
	return 0;
}

int packet_type(PACKET *pkt){
	switch(pkt->pkt_type){
		case PACKET_TYPE_EAP:
			printf("eap packet\n");
			break;
		default:
			printf("unknown type:%d\n",pkt->pkt_type);
			break;
	}
	printf("type:%d\n",pkt->body.test.type);
	return 0;
}
