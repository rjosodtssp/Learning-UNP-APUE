#include <iostream>
#include "sys/epoll.h"
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
using namespace std;

int main()
{
    //listen的准备工作
    int fd,connfd,maxi,nready;
    struct sockaddr_in addr,clie_addr;
    int open_max = sysconf(_SC_OPEN_MAX);
    int clients[open_max];

    fd = socket(AF_INET,SOCK_STREAM,0);
    //setsockopt在socket之后，bind之前
    int opt = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.0.106");
    addr.sin_port = htons(13000);
    socklen_t len = sizeof(addr);
    socklen_t clie_len = sizeof(clie_addr);

    bind(fd,(struct sockaddr*)&addr,len);
    int ret = listen(fd,128);
    if(ret<0)
    {
        cout<<"listen error:";
        perror(strerror(errno));
        exit(1);
    }

    char buf[40];
    for(int i=0;i<FD_SETSIZE;i++)
    {
        clients[i] = -1;
    }
    maxi = -1;
    int epfd = epoll_create(10);
    struct epoll_event event,evts[open_max];
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    if(ret<0)
    {
        cout<<"epoll_ctl error:";
        perror(strerror(errno));
        exit(1);
    }

    while(1)
    {
        cout<<"before wait"<<endl;
//        cout<< "evts 0:"<<evts[0].data.fd<<endl;
//        cout<< "evts 1:"<<evts[1].data.fd<<endl;
//        cout<< "evts 2:"<<evts[2].data.fd<<endl;
        nready = epoll_wait(epfd,evts,open_max,-1);
        cout<<"after wait"<<endl;
        cout<<"nready:"<<nready<<endl;
        if(nready<0)
        {
            cout<<"epoll wait error:";
            perror(strerror(errno));
            exit(1);
        }
        for(int i=0;i<nready;i++)
        {
            if(!(evts[i].events & EPOLLIN) )
                continue;
            cout<<"evts fd:"<<evts[i].data.fd<<endl;
            if(evts[i].data.fd == fd)
            {
                connfd = accept(evts[i].data.fd,(struct sockaddr*)&clie_addr,&clie_len);
                printf("client addr:%s, port:%d\n",
                       inet_ntoa(clie_addr.sin_addr),
                       htons(clie_addr.sin_port) );
                cout<<"connfd:"<<connfd<<endl;

                event.data.fd = connfd;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&event);
            }
            else{
                 int sockfd = evts[i].data.fd;
                 int n = read(sockfd,buf,sizeof(buf));
                 if(n<0)
                 {
                     cout<<"read error:";
                     perror(strerror(errno));
                     exit(1);
                 }
                 else if(n==0)
                 {
                     close(sockfd);
                     epoll_ctl(epfd,EPOLL_CTL_DEL,sockfd,NULL);
                 }
                 else
                 {
                     for(int k=0;k<n;k++)
                     {
                         buf[k] = toupper(buf[k]);
                     }
                     write(sockfd,buf,n);
                     write(STDOUT_FILENO,buf,n );
                     cout<<endl;
                 }
            }
        }
    }
    close(fd);
    close(epfd);
    return 0;
}
