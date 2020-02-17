#include<semaphore.h>
#include<pthread.h>
#include<cstdio>
#include<iostream>
#include<vector>

#define size 1
#define THREADNUM 4

class RingQueue
{
    public:
        RingQueue()
            :v_(size)
        {
            cap_=size;
            sem_init(&ProSem_,0,size);//初始化生产者计数器，规定生产者有多少空间去生产。
            sem_init(&ComSem_,0,0);//规定生产者当前有多少资源可以消费。
            sem_init(&LockSem_,0,1);//初始化互斥锁，计数器只能为。
            Write_=0;
            Read_=0;
        }
        
        ~RingQueue()
        {
            sem_destroy(&ProSem_);
            sem_destroy(&ComSem_);
            sem_destroy(&LockSem_);
        }
        void Push(int& data)
        {
            sem_wait(&ProSem_);
            sem_wait(&LockSem_);
            v_[Write_]=data;
            Write_=(Write_+1)%cap_;
            sem_post(&LockSem_);
            sem_post(&ComSem_);
        }
        void Pop(int* data)
        {
            sem_wait(&ComSem_);
            sem_wait(&LockSem_);
            *data=v_[Read_];
            Read_=(Read_+1)%cap_;
            sem_post(&LockSem_);
            sem_post(&ProSem_);
        }
    private:
        std::vector<int> v_;
        size_t cap_;
        int Write_;
        int Read_;
        sem_t ProSem_;
        sem_t ComSem_;
        sem_t LockSem_;
};

void* Pro_s(void* arg)
{
    RingQueue* bq=(RingQueue*)arg;
    int i=0;
    while(1)
    {
        bq->Push(i);
        printf("Pro make [%p][%d]\n",pthread_self(),i);
        i++;
    }
    return NULL;
}
void* Con_s(void* arg)
{
    RingQueue* bq=(RingQueue*)arg;
    int data;
    while(1)
    {
        bq->Pop(&data);
        printf("Pro consum [%p][%d]\n",pthread_self(),data);
    }
    return NULL;
}
int main()
{
    RingQueue* bq = new RingQueue();
    pthread_t Pro[THREADNUM],Con[THREADNUM];
    int i=0;
    int ret=-1;
    for(;i<THREADNUM;i++)
    {
        ret=pthread_create(&Pro[i],NULL,Pro_s,(void*)bq);
        if(ret!=0)
        {
            printf("create Thread faild\n");
            return 0;
        }
        ret=pthread_create(&Con[i],NULL,Con_s,(void*)bq);
        if(ret!=0)
        {
            printf("create Thread faild\n");
            return 0;
        }
    }

    for(i=0;i<THREADNUM;i++)
    {
        pthread_join(Pro[i],NULL);
        pthread_join(Con[i],NULL);
    }
    delete bq;
    return 0;
}
