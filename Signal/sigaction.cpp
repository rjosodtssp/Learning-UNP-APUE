#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
using namespace std;
void func(int n)
{
    printf("signal %d catched  !\n",n);
}
int main()
{
    struct sigaction act;
    act.sa_handler = func;
    sigaddset(&act.sa_mask,SIGQUIT);
    act.sa_flags = 0;
    if(sigaction(SIGINT,&act,NULL)<0)
    {
        perror("action error\n");
        exit(1);
    }
    while(1);
    return 0;
}
