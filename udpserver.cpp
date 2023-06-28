#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <string>
#include <iostream>

#define BUF_SIZE 1024

int main()
{
    int serv_sock;
    int str_len;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;
    char message[BUF_SIZE];

    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock == -1)
    {
        std::cout << "UDP socket creation error";
        exit(1);
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(9190);

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        std::cout << "bind() error";
        exit(1);
    }

    while (1)
    {
        clnt_adr_sz = sizeof(clnt_adr);
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        sendto(serv_sock, message, str_len, 0, (struct sockaddr *)&clnt_adr, clnt_adr_sz);
    }
    
    close(serv_sock);
    return 0;
}
