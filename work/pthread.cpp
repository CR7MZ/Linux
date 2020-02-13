#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int g_va =100;//临界资源

#define tm 4

pthread_mutex_t g_mutex;//定义互斥锁变量

typedef struct TP
{
    int val;
}TP;

void* Thread(void* arg)
{
    TP* tp=(TP*)arg;
    while(1)
    {
        pthread_mutex_lock(&g_mutex);//加锁
        if(g_va>0)
        {
            usleep(1000);
            printf("i am pthread [%d],i have ticket [%d]\n",tp->val,g_va);
            g_va--;
        }
        else 
        {
            pthread_mutex_unlock(&g_mutex);//解锁
            delete tp;
            tp=NULL;
            break;
        }
        //在线程所有退出的地方前都需要解锁
        pthread_mutex_unlock(&g_mutex);//解锁
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&g_mutex,NULL);//初始化互斥锁
    pthread_t tid[tm];
    for(int i=0;i<tm;++i)
    {   
    TP* tp=new TP;
    tp->val=i;
    pthread_create(&tid[i],NULL,Thread,(void*)tp);
    }

    for(int i=0;i<tm;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&g_mutex);//销毁互斥锁
    return 0;
}
