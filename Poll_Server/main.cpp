#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#define INFTIM -1
int main()
{
    int fd,ret,maxi,maxfd,connfd,nready;
    char buf[30];
    struct sockaddr_in addr,clie_addr;
    socklen_t len = sizeof(addr);
    int open_max = sysconf(_SC_OPEN_MAX);
    pollfd clients[open_max];

    fd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&addr,sizeof(addr));
    addr.sin_addr.s_addr = inet_addr("192.168.0.106");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(13000);
    int opt = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bind(fd,(struct sockaddr*)&addr,len);
    ret = listen(fd,128);
    if(ret<0)
    {
        perror("listen error");
        exit(1);
    }
    maxfd = fd;
    clients[0].fd = fd;
    clients[0].events = POLLRDNORM;
    for(int i=1;i<open_max;i++)
    {
        clients[i].fd = -1;
    }
    maxi = 0;
    while(1)
    {
        nready = poll(clients,maxi+1,INFTIM);
        if(clients[0].revents & POLLRDNORM)
        {
            socklen_t clie_len = sizeof(clie_addr);
            connfd = accept(fd,(struct sockaddr*)&clie_addr,&clie_len);
            printf("client addr:%s, port:%d\n",
                   inet_ntoa(clie_addr.sin_addr),
                   htons(clie_addr.sin_port) );
            maxfd = connfd;
            int i;
            for(i=1;i<open_max;i++)
            {
                if(clients[i].fd<0)
                {
                    clients[i].fd = connfd;
                    clients[i].events = POLLRDNORM;
                    break;
                }
            }
            if(i>=open_max)
            {
                perror("too many clients !\n");
                exit(1);
            }
            if(i>maxi)
                maxi = i;
            if(--nready==0)
                continue;
        }
        for(int j=1;j<open_max;j++)
        {
            if(clients[j].fd<0)
                continue;
            if(clients[j].revents & (POLLRDNORM | POLLERR))   // ???
            {
                int n = read(clients[j].fd,buf,sizeof(buf));
                if(n<0)
                {
                    if(errno==ECONNRESET)
                    {
                        close(clients[j].fd);
                        clients[j].fd = -1;
                    }
                    else
                        perror("read error");
                }
                else if(n==0)
                {
                    close(clients[j].fd);
                    clients[j].fd = -1;
                }
                else{
                    for(int k=0;k<n;k++)
                    {
                        buf[k] = toupper(buf[k]);
                    }
                    write(clients[j].fd,buf,n);
                    write(STDOUT_FILENO,buf,n);
                    cout<<endl;
                }
                if(--nready<=0)
                    break;
            }

        }
    }

    return 0;
}
