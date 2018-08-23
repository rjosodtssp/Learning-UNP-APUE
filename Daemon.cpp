#include <iostream>
#include <unistd.h>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
int main()
{
    pid_t pid;
    pid=fork();
    if(pid>0)
        exit(0);
    else if(pid<0)
        exit(1);

    setsid();
    pid=fork();
    if(pid>0)
        exit(0);
    else if(pid<0)
        exit(1);

    for(int i=0;i<5; i++)
        close(i);
    chdir("/");
    umask(0);
    signal(SIGCHLD,SIG_IGN);
    while(1)
    {
        ;
    }
    return 0;
}
