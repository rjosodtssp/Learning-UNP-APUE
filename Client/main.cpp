#include <iostream>
using namespace std;
#include<unistd.h>
#include <netinet/in.h>
#include <strings.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
int main()
{
    int cfd;
    sockaddr_in addr;
    cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd==-1)
        perror("socket error");
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(13000);
    addr.sin_addr.s_addr = inet_addr("192.168.0.100");
    int ret = connect(cfd,(struct sockaddr*)&addr,sizeof(addr));
    std::cout<<"ret:"<<ret<<endl;
    if(ret<0)
    {
        std::cout<<"connect error: ";
        perror(strerror(errno));
    }
    while(1)
        ;
    return 0;
}
