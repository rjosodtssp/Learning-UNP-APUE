#include <iostream>
#include <unistd.h>
using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
    pid_t pid,sid;
    pid = fork();

    if(pid>0)
        exit(0);
    else{
        cout<<"child"<<endl;
        cout<<"jklgdfgh";
        sid = setsid();

        int ret = chdir("/");
        if(ret<0)
        {
            perror("chdir error");
            exit(1);
        }

        umask(0002);
        close(0);
        int fd = open("/dev/null", O_RDWR);
        dup2(0, STDOUT_FILENO);
        dup2(0, STDERR_FILENO);
        while(1)
        {
            ;
        }
    }

    return 0;
}
