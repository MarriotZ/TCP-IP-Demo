#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void Read_routine(int sock, char *buf);
void Write_routine(int sock, char *buf);
void Sender_message(char *message);

int main(int argc, char *argv[])
{
    int sock;
    int port;
    pid_t pid;
    std::string ipAddress;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;

    std::cout << "Please input IP and port that you want to connect:" << std::endl;
    std::cin >> ipAddress >> port;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        Sender_message((char *)"Socket creation error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(ipAddress.c_str());
    serv_adr.sin_port = htons(port);

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        Sender_message((char *)"Connect error!");
    }
    // 创建子进程
    pid = fork();
    // 如果是子进程，那么进行写操作（发送数据）；父进程则进行读操作（接收数据）
    pid == 0 ? Write_routine(sock, buf) : Read_routine(sock, buf);

    close(sock);
    return 0;
}

void Read_routine(int sock, char *buf)
{
    while (1)
    {
        int str_len = read(sock, buf, BUF_SIZE);
        if (str_len == 0)
        {
            return;
        }
        buf[str_len] = 0;
        printf("Message from server: %s", buf);
    }
}

void Write_routine(int sock, char *buf)
{
    while (1)
    {
        fgets(buf, BUF_SIZE, stdin);
        // 如果输入为q或Q，则关闭套接字写权限
        if (!strcmp(buf, "Q\n") || !strcmp(buf, "q\n"))
        {
            // 关闭套接字写权限
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}

void Sender_message(char *message)
{
    puts(message);
    exit(1);
}