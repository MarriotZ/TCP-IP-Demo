#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 1024

void Sender_error(char *message);

int main(int argc, char *argv[])
{
    int port, serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    printf("Please input the port of socket that you want to create:\n");
    scanf("%d", &port);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        Sender_error((char *)"Sock creation error");
    }
    else
    {
        // 注意：serv_sock初始化成功后值为0
        fd_max = serv_sock;
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(port);

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        Sender_error((char *)"bind() error");
    }
    if (listen(serv_sock, 5) == -1)
    {
        Sender_error((char *)"listen() error");
    }
    // 对监测项的文件描述符作初始化赋0操作
    FD_ZERO(&reads);
    // 注册serv_sock套接字信息至reads变量中
    FD_SET(serv_sock, &reads);

    while (1)
    {
        // cpy_reads用来记录文件描述符变化
        cpy_reads = reads;
        // 设置超时等待周期为5s
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // 只关注接收数据的套接字，不对传输数据、出现异常的套接字进行监视
        // fd_num用来记录发生监视事件的文件描述符数量
        if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
        {
            break;
        }

        if (fd_num == 0)
        {
            continue;
        }

        for (i = 0; i < fd_max + 1; i++)
        {
            // 判断cpy_reads中是否含有文件描述符i的信息
            if (FD_ISSET(i, &cpy_reads))
            {
                // 若当前只有服务器端套接字，则尝试接收客户端请求
                if (i == serv_sock)
                {
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
                    // 将客户端的套接字文件描述符信息注册至reads中
                    FD_SET(clnt_sock, &reads);
                    if (fd_max < clnt_sock)
                    {
                        // 增加监视数上限（因为有新的客户端套接字加入）
                        fd_max = clnt_sock;
                    }
                    printf("Connected client: %d \n", clnt_sock);
                }
                else
                {
                    // 接收数据
                    str_len = read(i, buf, BUF_SIZE);
                    // 无数据，则关闭对应套接字
                    if (str_len == 0)
                    {
                        // 清除reads变量中文件描述符i的信息
                        FD_CLR(i, &reads);
                        close(i);
                        printf("Closed client: %d \n", i);
                    }
                    else
                    {
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}

void Sender_error(char *message)
{
    puts(message);
    exit(1);
}