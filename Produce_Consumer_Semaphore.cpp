#include <iostream>
#include <semaphore.h>
using namespace std;
#include <unistd.h>

#define NUM 5
int queue[NUM];
sem_t blank, product;

void* produce(void* arg)
{
    int i=0;
    while(1)
    {
        sem_wait(&blank);
        queue[i] = rand()%50+50;
        printf("produce number:%d\n",queue[i]);
        sem_post(&product);
        i = (i+1)%NUM;
//        printf("after post:produce number:%d\n",queue[i]);    //0
        sleep(rand()%3);
    }

}

void* consume(void* arg)
{
    int i=0;
    while(1)
    {
        sem_wait(&product);
        printf("    consume number:%d\n",queue[i]);
        queue[i] = 0;
        sem_post(&blank);
        i = (i+1)%NUM;
        sleep(rand()%3);
    }
}
int main()
{
    pthread_t pid,cid;
    void* ret;
    srand(time(NULL));
    sem_init(&blank,0,NUM);
    sem_init(&product,0,0);
    pthread_create(&pid,NULL,produce,NULL);
    pthread_create(&cid,NULL,consume,NULL);

    pthread_join(pid,&ret);
    pthread_join(cid,&ret);
    sem_destroy(&blank);
    sem_destroy(&product);
    return 0;
}
