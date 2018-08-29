#include <iostream>
#include <unistd.h>
using namespace std;
#include <pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct Node{
    int num;
    Node* next;
};
struct Node* head=NULL;
struct Node* p=NULL;
void* produce(void* arg)
{
    while(1)
    {
        p = (struct Node*)malloc(sizeof(Node));

        p->num = rand()%20;
        cout<<"produce:  "<<p->num<<endl;
        pthread_mutex_lock(&mutex);
        p->next = head;
        head = p;

        pthread_mutex_unlock(&mutex);
        sleep(rand()%4);
        pthread_cond_signal(&cond);
    }

    return NULL;
}

void* consume(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(!head)
            pthread_cond_wait(&cond,&mutex);

//        struct Node* oldHead = head;
//        int data = oldHead->num;
//        head = head->next;
//        oldHead->next = NULL;

        p = head;
        head = p->next;
//        while(head)
//        {
//            cout<<head->num<<"  ";
//            head = head->next;
//        }
        pthread_mutex_unlock(&mutex);
        cout<<"consume: "<<p->num<<endl;
        free(p);
        p = NULL;
        sleep(rand()%3);
    }
    return NULL;
}

int main()
{
    pthread_t pid,cid;
    void* ret;
    pthread_create(&pid,NULL,produce,NULL);
    pthread_create(&cid,NULL,consume,NULL);
//    sleep(1);
    pthread_join(pid,&ret);
    pthread_join(cid,&ret);
    return 0;
}
