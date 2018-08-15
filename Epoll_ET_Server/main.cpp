#include <iostream>
using namespace std;
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <string.h>
#include <strings.h>


int main()
{
    int fd,epfd,connfd,nready,ret,sockfd;
    int size;
    char buf[30];
    struct sockaddr_in addr,clie_addr;
    epoll_event event, evts[1024];
    socklen_t len = sizeof(addr);
    bzero(&addr,len);
    addr.sin_addr.s_addr = inet_addr("192.168.0.106");
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(13000);
    fd = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0);
    int opt =1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    bind(fd,(struct sockaddr*)&addr,len);
    listen(fd,128);
    epfd = epoll_create(10);

    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    if(ret<0)
    {
        perror("epoll ctl error");
        perror(strerror(errno));
        exit(EXIT_FAILURE);
    }
    while(1)
    {
        cout<<"before epoll wait"<<endl;
        nready = epoll_wait(epfd,evts,1024,-1);
        cout<<"after epoll wait"<<endl;
        if(nready<0)
        {
            perror("epoll wait error");
            perror(strerror(errno));
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<nready;i++)
        {
            if(!(evts[i].events | EPOLLIN))
                continue;
            if(evts[i].data.fd == fd)
            {
                connfd = accept4(fd,(struct sockaddr*)&clie_addr,&len,SOCK_NONBLOCK);
                printf("client addr:%s, port:%d\n",
                       inet_ntoa(clie_addr.sin_addr),ntohs(clie_addr.sin_port));
                event.data.fd = connfd;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&event);

            }
            else{

                if(evts[i].events | EPOLLIN)
                {
                    sockfd = evts[i].data.fd;
                    int n=0;
                    while((size=read(sockfd,buf+n,5)) >0 )
                    {
                        n+=size;
                        printf("print buf:%s\n",buf);
                    }
                    for(int j=0;j<n;j++)
                    {
                        buf[j] = toupper(buf[j]);
                        printf("print buf:%c\n",buf[j]);
                    }
                    write(sockfd,buf,n);
                    write(STDOUT_FILENO,buf,n);
                    cout<<endl;
                }
            }

        }
    }
    close(fd);
    close(epfd);
    return 0;
}
