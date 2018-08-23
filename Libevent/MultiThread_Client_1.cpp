#include <iostream>
#include <unistd.h>
using namespace std;
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <event.h>

#define PORT 13000
#define MAX_LINE 80
#define UNUSED(expr) do { (void)(expr); } while (0)

char s[MAX_LINE];

void read_cb(struct bufferevent* bev, void* arg);
void eventcb(struct bufferevent* bev,short events, void* arg);
void* Work_Thread(void* arg)
{
    UNUSED(arg);
    pthread_t tid = pthread_self();
//    printf("work thread:%lu\n",tid);
    sockaddr_in addr;
    socklen_t size = sizeof(addr);
    memset(&addr,0,size);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.0.106");
    addr.sin_port = htons(PORT);

    event_base* base = event_base_new();
    struct bufferevent *buffer = bufferevent_socket_new(base,-1,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(buffer,read_cb,NULL,eventcb,NULL);
    bufferevent_enable(buffer,EV_READ | EV_WRITE);
    if(bufferevent_socket_connect(buffer,(struct sockaddr*)&addr,size)<0)
    {
        perror("socket connect error");
        bufferevent_free(buffer);
        exit(1);
    }
    event_base_dispatch(base);
    return NULL;
}

int main()
{
    while(1)
    {
        fgets(s,MAX_LINE,stdin);
        pthread_t tid;
        int err = pthread_create(&tid,NULL,Work_Thread,NULL);
        if(err)
        {
            perror(strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        pthread_t main_tid = pthread_self();
//        printf("main thread:%lu\n",main_tid);
    }
    return 0;
}

void read_cb(struct bufferevent* bev, void* arg)
{
    UNUSED(arg);
    printf("func:%s\n",__FUNCTION__);
    struct evbuffer* eb = bufferevent_get_input(bev);
    size_t size = evbuffer_get_length(eb);
//    printf("rev from server:%u\n",size);
    char data[MAX_LINE];    //千万别用char* data;或者是动态分配内存
    int n = bufferevent_read(bev,data,MAX_LINE);
    if(n>0)
    {
        printf("rev data from server:%s",data);
    }
    struct event_base* base = bufferevent_get_base(bev);
    event_base_loopbreak(base);
}

void eventcb(struct bufferevent* bev,short events, void* arg)
{
    printf("func:%s\n",__FUNCTION__);
    UNUSED(arg);
    if(events & BEV_EVENT_CONNECTED)
    {
        fprintf(stdout,"connected !\n");
        bufferevent_write(bev,s,strlen(s));
    }
    else if(events & BEV_EVENT_ERROR)
    {
        fprintf(stderr,"connect failed !");
        bufferevent_free(bev);
    }
    return;
}
