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
    int clnt_sock;
    int str_len;
    struct sockaddr_in serv_adr, from_adr;
    socklen_t clnt_adr_sz;
    char message[BUF_SIZE];

   

    clnt_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (clnt_sock == -1)
    {
        std::cout << "UDP socket creation error";
        exit(1);
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(9190);

    while(1)
    {
        std::cout<<"Please input message(q to quit)"<<std::endl;
        std::cin>>message;
        if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))
        {
            break;
        }
        sendto(clnt_sock,message,strlen(message),0,(struct sockaddr *)&serv_adr,sizeof(serv_adr));
        clnt_adr_sz=sizeof(from_adr);
        str_len=recvfrom(clnt_sock,message,BUF_SIZE,0,(struct sockaddr *)&from_adr,&clnt_adr_sz);
        message[str_len]=0;
        std::cout<<"Message from server: "<<message<<std::endl;

    }

    close(clnt_sock);
    return 0;
}
