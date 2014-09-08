#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <fcntl.h>
#include <linux/if_ether.h>
#include <strings.h>
#include <string.h>
#include <netpacket/packet.h> //struct sockaddr_ll
#include <net/ethernet.h>
#include "packet.h"

uint8_t mac_8021x_standard[6]={0x01,0x80,0xc2,0x00,0x00,0x03};

int gethwaddr(unsigned char *hwaddr){
	int sock;	
	struct ifreq ifr;
	if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		return -1;
	}
	memset(&ifr,0,sizeof(ifr));
	memset(hwaddr,0,6);
	strncpy(ifr.ifr_name,"eth0",6);
	if(ioctl(sock,SIOCGIFHWADDR,&ifr)<0)
	{
		return -1;
	}
	memcpy(hwaddr,ifr.ifr_hwaddr.sa_data,6);
	//printf("%02x\n",hwaddr[2]);
	return 0;
}



int main(){
	int sock,size,flags;
	char buf[1024];
	struct sockaddr_ll addr;
	struct ifreq ifr;
	unsigned char hwaddr[6];
	PACKET pk_start;
	if((sock=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_PAE)))<0){
		printf("create raw socket error\n");
		return -1;
	}
	flags=fcntl(sock,F_GETFL,0);
	printf("%d\n",flags);
	if(fcntl(sock,F_SETFL,flags|O_NONBLOCK)<0)
	{
		printf("set nonblock error\n");
		return -1;
	}
	strcpy(ifr.ifr_name,"eth0");
	if(ioctl(sock,SIOCGIFINDEX,&ifr)<0)
	{
		printf("get ifindex error\n");
		return -1;
	}
	printf("%d\n",ifr.ifr_ifindex);
	bzero(&addr,sizeof(struct sockaddr_ll));
	addr.sll_family=PF_PACKET;
	addr.sll_protocol=htons(ETH_P_PAE);
	addr.sll_ifindex=ifr.ifr_ifindex;
	if(bind(sock,(struct sockaddr *)&addr,sizeof(struct sockaddr_ll))<0)
	{
		printf("bind error\n");
		return -1;
	}
	if(gethwaddr(hwaddr)<0)
	{
		printf("get hwaddr error\n");
		return -1;
	}
	printf("mac %02x:%02x:%02x:%02x:%02x:%02x\n",hwaddr[0],hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);
	get_start_packet(&pk_start,hwaddr);
	if(sendto(sock,&pk_start,sizeof(pk_start),0,(struct sockaddr *)&addr,sizeof(struct sockaddr_ll))<0)
	{
		printf("send packet error");
		return -1;
	}
	PACKET pkt_recv;
	while(1){
		printf("recv message\n");
		size=recvfrom(sock,(void *)&pkt_recv,sizeof(pkt_recv),0,NULL,NULL);
		if(size>0)
		{
			packet_type(&pkt_recv);
			break;
		}
	}
	return 0;
}










