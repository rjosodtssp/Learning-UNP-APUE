#include <iostream>
#include "sys/select.h"
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
    int fd,lfd,connfd,maxi,nready;
    struct sockaddr_in addr,clie_addr;

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
    // select的准备工作
    int clients[FD_SETSIZE];
    int maxfd = fd;
    char buf[40];
    for(int i=0;i<FD_SETSIZE;i++)
    {
        clients[i] = -1;
    }
    maxi = -1;
    fd_set rset,allset;
    FD_ZERO(&allset);
    FD_SET(fd,&allset);
    while(1)
    {
        rset = allset;
        nready = select(maxfd+1,&rset,NULL,NULL,NULL);
        if(FD_ISSET(fd,&rset))
        {
            connfd = accept(fd,(struct sockaddr*)&clie_addr,&clie_len);
            printf("client addr:%s, port:%d\n",
                   inet_ntoa(clie_addr.sin_addr),
                   htons(clie_addr.sin_port) );
            FD_SET(connfd,&allset);
            int i;
            for(i=0;i<FD_SETSIZE;i++)
            {
                if(clients[i]<0)
                {
                    clients[i] = connfd;
                    break;
                }
            }
            if(i>FD_SETSIZE)
                exit(1);
            if(maxi<i)
                maxi = 0;
            if(maxfd<connfd)
                maxfd = connfd;
            if(--nready<=0)     //
                continue;
        }

        for(int i=0;i<FD_SETSIZE;i++)
        {
            if(clients[i]<0)
                continue;
            if(FD_ISSET(clients[i],&rset))    //不在集合则不处理
            {
                int n = read(clients[i],buf,sizeof(buf));
                if(n<0)
                {
                    perror("read error");
                    exit(1);
                }
                else if(n==0)
                {
                    printf("client closed, unable to read !\n");
                    close(clients[i]);
                    FD_CLR(clients[i],&allset);
                    clients[i] = -1;
                }
                else
                {
                    for(int j=0;j<n;j++)    //是n,不是sizeof(buf)
                    {
                        buf[j] = toupper(buf[j]);
                    }
                    write(clients[i],buf,n);
                    write(STDOUT_FILENO,"处理后的字符串:",24);
                    write(STDOUT_FILENO,buf,n);
                    cout<<endl;
                }
            }
            //处理nready，可以没有
            if(--nready<=0)
                break;
        }

    }
    close(fd);
    return 0;
}
