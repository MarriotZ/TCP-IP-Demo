#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>
int main()
{
    struct hostent *host;
    std::string ipAddress;
    struct sockaddr_in addr;

    std::cout << "Please input a website IP addreess：" << std::endl;
    std::cin >> ipAddress;

    addr.sin_addr.s_addr=inet_addr(ipAddress.c_str());
    //特别注意！！
    host = gethostbyaddr((char*)&addr.sin_addr,4,AF_INET);

    if (!host)
    {
        std::cout << "Get host error" << std::endl;
        return -1;
    }
 
    std::cout << "Official name:" << host->h_name << std::endl;

    for (size_t i = 0; host->h_aliases[i]; i++)
    {
        std::cout << "Aliases " << i + 1 << ":" << host->h_aliases[i] << std::endl;
    }
 
    std::cout << "Address type: " << ((host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6") << std::endl;

    for (size_t i = 0; host->h_addr_list[i]; i++)
    {
        std::cout << "IP Address " << i + 1 << ":" << inet_ntoa(*(struct in_addr*)host->h_addr_list[i]) << std::endl;
    }

    return 0;
}