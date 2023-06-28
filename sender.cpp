#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 1024

void Sender_message(char *message)
{
	puts(message);
	exit(1);
}

int main(int argc, char *argv[])
{
	int send_sock;
	struct sockaddr_in mul_addr;
	int time2live = TTL;
	FILE *fp;
	char buf[BUF_SIZE];

	send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&mul_addr, 0, sizeof(mul_addr));

	mul_addr.sin_family = AF_INET;
	mul_addr.sin_addr.s_addr = inet_addr(argv[1]); // 多播地址
	mul_addr.sin_port = htons(atoi(argv[2]));	   // 多播端口号

	setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time2live, sizeof(time2live));

	fp = fopen(argv[3], "r");
	if (fp == NULL)
	{
		Sender_message((char*)"file open error");
	}

	while (!feof(fp)) /* 发送数据 */
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_addr, sizeof(mul_addr));
	}

	fclose(fp);
	close(send_sock);
	return 0;
}
