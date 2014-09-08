#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

int main(){
	int sock;
	int flags;
	struct sockaddr_in sin;
	int port=80;
	if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("create socket error\n");
		return -1;
	}
	memset(sin,0,sizeof(struct sockaddr_in));
	sin.sin_family=AF_INET;
	sin.sin_port=htons(port);
	sin.sin_addr.s_addr=inet_addr(hsname);
	flags=fcntl(sock,F_GETFL,0);
	connect(sock,(struct sockaddr *)&sin,sizeof(sin));
	fcntl(sock,F_SETFL,flags|O_NONBLOCK);
	
	return 0;
}
