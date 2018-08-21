#include <iostream>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <event.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
using namespace std;

#define PORT 13000
#define MAX_LINE 40
#define UNUSED(expr) do { (void)(expr); } while (0)

void accept_conn_cb(struct evconnlistener* listener, evutil_socket_t fd,
                           struct sockaddr* add, int socklen, void *ctx);

void read_cb(struct bufferevent* bev,  void *arg);
void write_cb(struct bufferevent* bev, void *arg);
void err_cb(struct bufferevent* bev,  short events, void *arg);

int main()
{
    struct event_base* base;
    struct evconnlistener* listener;
    struct sockaddr_in addr;

    base = event_base_new();
    if(!base)
    {
        fprintf(stderr,"new base error\n");
        exit(1);
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_addr.s_addr = inet_addr("192.168.0.106");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);

    listener = evconnlistener_new_bind(base,accept_conn_cb,NULL,
            LEV_OPT_REUSEABLE,-1,(struct sockaddr*)&addr,sizeof(addr));
    if(!listener)
    {
        fprintf(stderr,"new listener error\n");
        exit(1);
    }
//    evconnlistener_set_cb();
    event_base_dispatch(base);
    return 0;
}

void accept_conn_cb(struct evconnlistener* listener, evutil_socket_t fd,
                           struct sockaddr* add, int socklen, void *ctx)
{
    printf("function:%s\n",__FUNCTION__);
    UNUSED(add);
    UNUSED(socklen);
    UNUSED(ctx);
    struct event_base *base = evconnlistener_get_base(listener);
    evutil_socket_t lfd = evconnlistener_get_fd(listener);
    printf("lfd:%d\n",lfd);
//    struct sockaddr_in sin;
//    socklen_t slen = sizeof(sin);
//    fd = accept(7, (struct sockaddr *)&sin, &slen);
//    printf("fd:%d  client addr:%s  port:%d\n",fd,inet_ntoa(sin.sin_addr));

    struct bufferevent* bev = bufferevent_socket_new(base,fd,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, read_cb, write_cb, err_cb, NULL);
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void read_cb(struct bufferevent* bev,void *arg)
{
    printf("function:%s\n",__FUNCTION__);
    UNUSED(arg);
    struct evbuffer * buf = bufferevent_get_input(bev);
    size_t size = evbuffer_get_length(buf);
    printf("read size:%d\n",size);

    char Rev[MAX_LINE+1], Send[MAX_LINE+1];
    int n;
    evutil_socket_t cfd = bufferevent_getfd(bev);
    if ( (n = bufferevent_read(bev, Rev, MAX_LINE)) > 0)
    {
//        printf("read size:%d\n",n);
        Rev[n] = '\0';
        printf("cfd=%u, read read from client: %s\n", cfd, Rev);

        for(int i=0;i<n;i++)
        {
            Send[i] = toupper(Rev[i]);
        }
        Send[n] = '\0';
        bufferevent_write(bev, Send, n);
    }
}

void write_cb(struct bufferevent* bev, void *arg)
{
    printf("function:%s\n",__FUNCTION__);
    UNUSED(bev);
    UNUSED(arg);
}

void err_cb(struct bufferevent* bev, short events, void *arg)
{
    printf("function:%s\n",__FUNCTION__);
    UNUSED(arg);
    if(events & BEV_EVENT_EOF)
        printf("Connection Closed\n");
    else if(events & BEV_EVENT_ERROR)
        printf("error on Connection\n");
    bufferevent_free(bev);
}
