#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

// 报错消息发送
void Sender_message(char *message)
{
    puts(message);
    exit(1);
}

int main(int argc, char *argv[])
{
    int recv_sock;
    struct sockaddr_in addr;
    int str_len;
    char buf[BUF_SIZE];

    recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (recv_sock == -1)
    {
        Sender_message((char *)"socket creation error");
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    if (bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        Sender_message((char *)"bind error");
    }
    while (1)
    {
        str_len = recvfrom(recv_sock, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (str_len < 0)
        {
            break;
        }
        buf[str_len] = 0;
        fputs(buf, stdout);
    }

    close(recv_sock);
    return 0;
}
