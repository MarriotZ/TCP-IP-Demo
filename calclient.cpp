#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#define BUF_SIZE 1024
#define BYTEUNIT 4
#define RLT_SIZE 4

using namespace std;
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
int main()
{
    int opnd_cnt;
    char opmsg[BUF_SIZE];

    struct sockaddr_in serv_addr;

    int clnt_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (clnt_sock == -1)
    {
        error_handling((char*)"socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9190);

    if (connect(clnt_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        error_handling( (char*)"connect() error!");
    }
    else
    {
        puts("Connected....");
    }

    fputs("Operand count:", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for (int i = 0; i < opnd_cnt; i++)
    {
        printf("Operand %d: ", i + 1);
        //每个整数型占用4个字节
        scanf("%d", (int *)&opmsg[i * BYTEUNIT + 1]);
    }
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt * BYTEUNIT + 1]);
    write(clnt_sock, opmsg, opnd_cnt * BYTEUNIT + 2);
    int result;
    read(clnt_sock,&result,RLT_SIZE);

    printf("Operation result: %d \n",result);
    close(clnt_sock);
    return 0;
}

