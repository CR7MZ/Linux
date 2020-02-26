#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>
#include <stdlib.h>

#define THREADCOUNT   4

typedef void(*Hander_t)(int);
class TaskTable//线程池中，队列中的任务结点。
{
    public:
        TaskTable()
        {
            _data=-1;
            _Hander=NULL;
        }
        TaskTable(int data,Hander_t Hander)
        {
            _data=data;
            _Hander=Hander;
        }
        void Run()
        {
            _Hander(_data);
        }
    private:
        int _data;
        Hander_t _Hander;
};

class ThreadPool
{
    public:
        ThreadPool()
        {
            _ThreadCapacity=THREADCOUNT;
            _ThreadCurNum=THREADCOUNT;

            pthread_mutex_init(&_mutex,NULL);

            pthread_cond_init(&_cond,NULL);

            bool IsCreat=ThreadCreat();
            if(!IsCreat)
            {
                std:: cout<<"creat thread failed!!!"<<std::endl;
                exit(0);
            }
            IsQuit=false;
        }

        ~ThreadPool()
        {
            pthread_mutex_destroy(&_mutex);
            pthread_cond_destroy(&_cond);
        }

        bool Push(TaskTable* tt)
        {
            pthread_mutex_lock(&_mutex);
            //如果设置了线程池中的线程退出就不要push数据了，释放互斥锁
            if(IsQuit)
            {
            pthread_mutex_unlock(&_mutex);
            return false;
            }
            _Que.push(tt);
            pthread_mutex_unlock(&_mutex);

            //放完数据后要通知线程来消费
            pthread_cond_signal(&_cond);
            return true;
        }

        bool  Pop(TaskTable*& tt)
        {
            tt=_Que.front();
            _Que.pop();
            return true;
        }

        bool PoolClear()
        {
            pthread_mutex_lock(&_mutex);
            IsQuit=true;
            pthread_mutex_unlock(&_mutex);
            if(_ThreadCurNum>0)
            {
                pthread_cond_broadcast(&_cond);
            }
            return true;
        }

    private:
        static void* ThreadStart(void* arg)
        {
            ThreadPool* tp=(ThreadPool*)arg;
            while(1)
            {
                pthread_mutex_lock(&tp->_mutex);
               while(tp->_Que.empty())  //如果队列为空，则线程陷入阻塞等待
               {
                    if(tp->IsQuit)
                    {
                        tp->ThreadQuit();
                    }
                   pthread_cond_wait(&tp->_cond,&tp->_mutex);
               }
                TaskTable* tt;
                tp->Pop(tt);
                pthread_mutex_unlock(&tp->_mutex);
                tt->Run();
                delete tt;
            }
            return NULL;
        }

        void ThreadQuit()
        {
            _ThreadCurNum--;
            pthread_mutex_unlock(&_mutex);
            pthread_exit(NULL);
        }

        bool ThreadCreat()
        {
            pthread_t tid;
            int ret=-1;
            for(int i=0;i<THREADCOUNT;i++)
            {
                    ret=pthread_create(&tid,NULL,ThreadStart,(void*)this);
                    std::cout<<"tid["<<i<<"]"<<std::endl;
                    if(ret!=0)
                    return false;
            }
            return true;
        }
    private:
        //线程池当中初始化时候，线程数量
        size_t _ThreadCapacity;
        //当前线程池有多少个线程
        size_t _ThreadCurNum;
        //线程安全的队列
        std::queue<TaskTable*> _Que;
        //互斥
        pthread_mutex_t _mutex;
        //同步
        pthread_cond_t _cond;
        //是否退出
        bool IsQuit;
};

void Print(int data)
{
    printf("i am Hander Func,i print[%d]\n",data);
}
int main()
{
    ThreadPool* tp=new ThreadPool();
    for(int i=0;i<10;i++)
    {
        TaskTable* tt =new TaskTable(i,Print); 
        tp->Push(tt);
    }

    sleep(3);
    tp->PoolClear();
    delete tp;
    return 0;
}
