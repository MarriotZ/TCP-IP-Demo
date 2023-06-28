#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void Handler_childproc(int sig)
{
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("Removed process id: %d \n", pid);
}

void Sender_message(char *message)
{
    puts(message);
    exit(1);
}

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    int str_len, state, port;
    struct sockaddr_in serv_adr, clnt_adr;
    struct sigaction act;
    pid_t pid;
    socklen_t adr_sz;
    char buf[BUF_SIZE];
    //防止僵尸进程
    act.sa_handler = Handler_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
    {
        Sender_message((char *)"Socket creation error");
    }

    printf("Please input the port of socket that you want to create:\n");
    scanf("%d", &port);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(port);

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
    {
        Sender_message((char *)"bind error");
    }
    if (listen(serv_sock, 5) == -1)
    {
        Sender_message((char *)"listen error");
    }

    while (1)
    {
        adr_sz = sizeof(clnt_adr);
        //accept后（受理客户端发来的请求）创建子进程
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &adr_sz);
        if (clnt_sock == -1)
        {
            continue;
        }
        else
        {
            puts("New client was connected...");
        }

        pid = fork();

        switch (pid)
        {
        case -1:
        {
            close(clnt_sock);
            continue;
        }
        case 0: //子进程
        {
            //关闭服务器端套接字（fork前的变量都被拷贝了的哟~）
            close(serv_sock);
            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
            {
                write(clnt_sock, buf, str_len);
            }
            close(clnt_sock);
            puts("Client was disconnected...");
            return 0;
        }
        default: //父进程
        {
            //关闭客户端连接
            close(clnt_sock);
            break;
        }
        }
    }
    close(serv_sock);
    return 0;
}
