#include <iostream>
#include <pthread.h>
using namespace std;
#include <unistd.h>
#include <string>
#include <string.h>

pthread_mutex_t mA,mB;
int A;

void* create1(void* arg)
{
    pthread_t tid = pthread_self();
    while(1)
    {
        pthread_mutex_lock(&mA);
        A = 10;
        sleep(1);
        cout<<"thread 1:"<<tid<<"  "<<A<<endl;
        sleep(1);
        pthread_mutex_lock(&mB);
        cout<<"thread 1 lock mB"<<endl;
    }
}

void* create2(void* arg)
{
    pthread_t tid = pthread_self();
    while(1)
    {
        pthread_mutex_lock(&mB);
        A = 40;
        sleep(2);
        cout<<"thread 2:"<<tid<<"  "<<A<<endl;
        sleep(2);
        pthread_mutex_lock(&mA);
        cout<<"thread 2 lock mA"<<endl;
    }
}

int main()
{
    pthread_t t1,t2;
    void* ret1;
    void* ret2;
    pthread_mutex_init(&mA,NULL);
    pthread_mutex_init(&mB,NULL);

    pthread_create(&t1,NULL,create1,NULL);
    pthread_create(&t2,NULL,create2,NULL);
    while(1)
        sleep(1);
    pthread_join(t1,&ret1);
    pthread_join(t1,&ret2);
    pthread_mutex_destroy(&mA);
    pthread_mutex_destroy(&mB);
    return 0;
}
