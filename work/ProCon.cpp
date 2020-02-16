#include<stdio.h>
#include<pthread.h>
#include<iostream>
#include<queue>
using namespace std;

#define num 4

class BlockQueue 
{
    public:
        BlockQueue(int cap)
        {
            _Cap=cap;
            pthread_mutex_init(&_Mutex,NULL);
            pthread_cond_init(&_Pro,NULL);
            pthread_cond_init(&_Con,NULL);
        }
        ~BlockQueue()
        {
            pthread_mutex_destroy(&_Mutex);
            pthread_cond_destroy(&_Pro);
            pthread_cond_destroy(&_Con);
        }
        bool Full()
        {
            if(_Queue.size()==_Cap)
                return true;
            return false;
        }
        int Pop(int *data)
        {
            pthread_mutex_lock(&_Mutex);
            while(_Queue.empty())
            {
                pthread_cond_wait(&_Con,&_Mutex);
            }
            *data=_Queue.front();
            _Queue.pop();
            pthread_mutex_unlock(&_Mutex);
            pthread_cond_signal(&_Pro);
            return 0;
        }
        int Push(int &data)
        {
            pthread_mutex_lock(&_Mutex);
            while(Full())
            {
                pthread_cond_wait(&_Pro,&_Mutex);
            }
            _Queue.push(data);
            pthread_mutex_unlock(&_Mutex);
            pthread_cond_signal(&_Con);
            return 0;
        }
    private:
        queue<int> _Queue;
        size_t _Cap;//队列容量
        pthread_mutex_t _Mutex;//互斥
        pthread_cond_t _Pro;//生产者条件变量
        pthread_cond_t _Con;//消费者条件变量
};

void* Produce_(void* arg)
{
    BlockQueue* bq=(BlockQueue*)arg;
    int i=0;
    while(1)
    {
        bq->Push(i);
        printf("Produce_ [%p][%d]\n",pthread_self(),i);
        i++;
    }
    return NULL;
}

void* Consum_(void* arg)
{
    BlockQueue* bq=(BlockQueue*)arg;
    while(1)
    {
        int data;
        bq->Pop(&data);
        printf("Consum_ [%p][%d]\n",pthread_self(),data);
    }
    return NULL;
}

int main()
{
    pthread_t ProTid[num],ConTid[num];
    int ret=-1;
    BlockQueue* bq=new BlockQueue(4);
    for(int i=0;i<num;i++)
    {
        ret=pthread_create(&ProTid[i],NULL,Produce_,(void*)bq);
        if(ret!=0)
        {
            cout<<"create Pro pthread failed"<<endl;
            return 0;
        }
        ret=pthread_create(&ConTid[i],NULL,Consum_,(void*)bq);
        if(ret!=0)
        {
            cout<<"create Con pthread failed"<<endl;
            return 0;
        }
    }

    for(int i=0;i<num;i++)
    {
        pthread_join(ProTid[i],NULL);
        pthread_join(ConTid[i],NULL);
    }
    delete bq;
    return 0;
}
