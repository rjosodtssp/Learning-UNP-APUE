#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#define MAX_LINE 1024
#define PORT 13000
#define UNUSED(expr) do { (void)(expr); } while (0)

void read_cb(struct bufferevent *bev, void *ctx)
{
    printf("func:%s\n",__FUNCTION__);
    char buf[MAX_LINE];
    memset(buf, 0, sizeof(buf));

    size_t n;
    while((n = bufferevent_read(bev, buf, MAX_LINE)) > 0)
    {
        printf("recv from server: %s\n", buf);
    }
}

void write_callback(struct bufferevent *bev, void *ctx)
{
    printf("func:%s\n",__FUNCTION__);
}

struct bufferevent *g_bev = NULL;

void eventcb(struct bufferevent *bev, short event, void *ptr)
{
    printf("func:%s\n",__FUNCTION__);
    if (event & BEV_EVENT_CONNECTED)
    {
        printf("connectde !\n");
        bufferevent_write(bev, "hello", strlen("hello"));
    }
    else if (event & BEV_EVENT_ERROR)
        printf("connect error !\n");
}


void *work_thread(void *arg)
{
    printf("func:%s\n",__FUNCTION__);
    struct event_base *base = event_base_new();
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr("192.168.0.106");
    sin.sin_port = htons(PORT);

    struct bufferevent *bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
    bufferevent_setcb(bev, read_cb, write_callback, eventcb, NULL);

    g_bev = bev;

    if(bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        bufferevent_free(bev);
        exit(EXIT_FAILURE);
    }
    int fd = bufferevent_getfd(bev);
    printf("work thread fd:%d\n",fd);
    event_base_dispatch(base);
    return NULL;
}
//主线程接收用户输入，工作线程执行事件循环
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, work_thread, NULL);

    char buf[MAX_LINE];
    while (1)
    {
        if (fgets(buf, MAX_LINE, stdin) == NULL)
            break;
        int fd = bufferevent_getfd(g_bev);
        printf("fd:%d\n",fd);
        bufferevent_write(g_bev, buf, strlen(buf));  //???
    }
    pthread_join(tid, NULL);
    return 0;
}
