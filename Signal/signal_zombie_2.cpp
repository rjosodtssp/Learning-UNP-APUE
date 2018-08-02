#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
using namespace std;
void func(int n)
{
    printf("signal %d catched  !\n",n);
    int stat;
    pid_t pid;
    while(pid =waitpid(-1,&stat,WNOHANG)>0)
    {
        if(WIFEXITED(stat))
            printf("process %d terminated %d!\n",pid,WEXITSTATUS(stat));
        else if(WIFSIGNALED(stat))
            printf("process %d signal %d!\n",pid,WTERMSIG(stat));
    }
}
int main()
{
    pid_t pid;
    printf("original  pid:%d\n",getpid());
    for(int i=0;i<3;i++)
    {
        pid = fork();
        if(pid>0)
        {
            printf("111  pid:%d, parent pid:%d\n",getpid(),getppid());
            sleep(1);
        }
        else if(pid==0)
        {
            printf("222 pid:%d, parent pid:%d\n",getpid(),getppid());
            sleep(2);
        }
    }

    struct sigaction act;
    act.sa_handler = func;
    sigaddset(&act.sa_mask,SIGQUIT);
    act.sa_flags = 0;
    if(sigaction(SIGCHLD,&act,NULL)<0)
    {
        perror("action error\n");
        exit(1);
    }
    while(1)    ;
    return 0;
}
