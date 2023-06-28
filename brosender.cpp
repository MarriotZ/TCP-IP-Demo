#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

//报错消息发送
void Sender_message(char *message)
{
    puts(message);
    exit(1);
}


int main(int argc, char *argv[])
{
	int send_sock;
	struct sockaddr_in brd_addr;
	FILE *fp;
	char buf[BUF_SIZE];
	int so_brd=1;
	
	send_sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(send_sock==-1)
    {
        Sender_message((char*)"socket creation error");
    }	
	memset(&brd_addr, 0, sizeof(brd_addr));
	brd_addr.sin_family=AF_INET;
	brd_addr.sin_addr.s_addr=inet_addr(argv[1]);
	brd_addr.sin_port=htons(atoi(argv[2]));
	
	setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));	
	fp=fopen(argv[3], "r");
    if(fp==NULL)
    {
		Sender_message((char*)"file open error");
    }

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&brd_addr, sizeof(brd_addr));
	}

	close(send_sock);
	return 0;
}
