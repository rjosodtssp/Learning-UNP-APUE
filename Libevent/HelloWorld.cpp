/*
  This example program provides a trivial server program that listens for TCP
  connections on port 9995.  When they arrive, it writes a short message to
  each client connection, and closes each connection once it is flushed.

  Where possible, it exits cleanly in response to a SIGINT (ctrl-c).
*/


#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#ifndef _WIN32
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif
#include <iostream>
using namespace std;
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

static const char MESSAGE[] = "Hello, World!\n";

static const int PORT = 13000;

 void listener_cb(struct evconnlistener *, evutil_socket_t,
    struct sockaddr *, int socklen, void *);
 void conn_writecb(struct bufferevent *, void *);
 void conn_readcb(struct bufferevent *, void *);
static void conn_eventcb(struct bufferevent *, short, void *);
static void signal_cb(evutil_socket_t, short, void *);

int main(int argc, char **argv)
{
    struct event_base *base;
    struct evconnlistener *listener;
    struct event *signal_event;

    struct sockaddr_in sin;
#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(0x0201, &wsa_data);
#endif

    base = event_base_new();
    if (!base) {
        fprintf(stderr, "Could not initialize libevent!\n");
        return 1;
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(PORT);
    sin.sin_addr.s_addr = inet_addr("192.168.0.106");
    listener = evconnlistener_new_bind(base, listener_cb, (void *)base,
        LEV_OPT_REUSEABLE  | LEV_OPT_CLOSE_ON_FREE, -1,
        (struct sockaddr*)&sin,
        sizeof(sin));

    if (!listener) {
        fprintf(stderr, "Could not create a listener!\n");
        return 1;
    }

    signal_event = evsignal_new(base, SIGINT, signal_cb, (void *)base);

    if (!signal_event || event_add(signal_event, NULL)<0) {
        fprintf(stderr, "Could not create/add a signal event!\n");
        return 1;
    }

    event_base_dispatch(base);

    evconnlistener_free(listener);
    event_free(signal_event);
    event_base_free(base);

    printf("done\n");
    return 0;
}
//有新连接时调用
 void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,
    struct sockaddr *sa, int socklen, void *user_data)
{
    printf("func:%s\n",__FUNCTION__);
    struct event_base *base = (struct event_base *)user_data;
    struct bufferevent *bev;
    bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        fprintf(stderr, "Error constructing bufferevent !");
        event_base_loopbreak(base);
        return;
    }
    bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, NULL);
    bufferevent_enable(bev, EV_WRITE);
    bufferevent_enable(bev, EV_READ);
    bufferevent_setwatermark(bev,EV_WRITE,8,48);
    bufferevent_setwatermark(bev,EV_READ,20,36);    //设置读取低水位和高水位
    bufferevent_write(bev, MESSAGE, strlen(MESSAGE));
}
//产生新连接会调用，因为此时输出缓冲区为空
 void conn_writecb(struct bufferevent *bev, void *user_data)
{
    printf("func:%s\n",__FUNCTION__);
    struct evbuffer *output = bufferevent_get_output(bev);
    cout<<"output size:"<<evbuffer_get_length(output)<<endl;
    if (evbuffer_get_length(output) == 0) {
        printf("向客户端发送数据为0\n");
//        bufferevent_free(bev);
    }
}
//受到客户的发送数据大小超过低水位且少于高水位时才调用
void conn_readcb(struct bufferevent *bev, void *user_data)
{
    printf("func:%s\n",__FUNCTION__);
    struct evbuffer *input = bufferevent_get_input(bev);
    cout<<"累计收到客户端发送数据大小:"<<evbuffer_get_length(input)<<endl;
}

static void
conn_eventcb(struct bufferevent *bev, short events, void *user_data)
{
    printf("func:%s\n",__FUNCTION__);
    if (events & BEV_EVENT_EOF) {
        printf("Connection closed.\n");
    } else if (events & BEV_EVENT_ERROR) {
        printf("Got an error on the connection: %s\n",
            strerror(errno));/*XXX win32*/
    }
    /* None of the other events can happen here, since we haven't enabled
     * timeouts */
    bufferevent_free(bev);
}

static void signal_cb(evutil_socket_t sig, short events, void *user_data)
{
    printf("func:%s\n",__FUNCTION__);
    struct event_base *base = (struct event_base *)user_data;
    struct timeval delay = { 2, 0 };

    printf("Caught an interrupt signal; exiting cleanly in two seconds.\n");

    event_base_loopexit(base, &delay);
}
