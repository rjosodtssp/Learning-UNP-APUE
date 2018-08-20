#include <iostream>

using namespace std;
#include <event2/event.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void cb_func(evutil_socket_t fd, short what, void *arg)
{
    printf("func:%s\n",__FUNCTION__);
    const char *data = (const char *)arg;
    printf("Got an event on socket %d:%s%s%s%s [%s]\n",
        (int) fd,
        (what&EV_TIMEOUT) ? " timeout" : "",
        (what&EV_READ)    ? " read" : "",
        (what&EV_WRITE)   ? " write" : "",
        (what&EV_SIGNAL)  ? " signal" : "",
        data );
}

void main_loop(evutil_socket_t fd1, evutil_socket_t fd2)
{
    printf("func:%s\n",__FUNCTION__);
    struct event *ev1, *ev2;
    struct timeval five_seconds = {8,0};
    struct event_base *base = event_base_new();

    /* The caller has already set up fd1, fd2 somehow, and make them
       nonblocking. */

    ev1 = event_new(base, fd1, EV_TIMEOUT|EV_READ|EV_PERSIST, cb_func,
       (char*)"Reading event");
    ev2 = event_new(base, fd2, EV_WRITE, cb_func,
       (char*)"Writing event");
    //令事件进入未决状态
    event_add(ev1, &five_seconds);
    event_add(ev2, NULL);
//    监听base，对应的事件满足时，直接调用回调函数
    event_base_dispatch(base);
}
int main()
{
    const char* fifo = "event.fifo";
    int fd1,fd2;
    unlink(fifo);   //确保fifo不存在
    if(mkfifo(fifo,0644))
    {
        perror("mkfifo error");
        exit(1);
    }
    fd1 = open(fifo, O_RDONLY | O_NONBLOCK, 0644);
    fd2 = open(fifo, O_WRONLY | O_NONBLOCK, 0644);
    printf("fd1:%d, fd2:%d\n",fd1,fd2);
    if(fd1<0 || fd2<0)
    {
        perror("open error");
        exit(1);
    }
    std::cout<<"-----------"<<endl;
    main_loop(fd1,fd2);
    close(fd1);
    close(fd2);
    return 0;
}
