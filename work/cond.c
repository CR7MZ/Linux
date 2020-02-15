#include <stdio.h>
#include <unistd.h>
#include<iostream>
#include <pthread.h>

int g_va = 0;//临界资源

#define tm 4

pthread_mutex_t g_mutex;//定义互斥锁变量
pthread_cond_t make;
pthread_cond_t con;

void* Eat(void* arg)
{
    (void)arg;
    while(1)
    {
        pthread_mutex_lock(&g_mutex);//加锁
        while(g_va==0)//循环判断资源是否为空，空：则不能消费了，进入等待队列。
        {
            pthread_cond_wait(&con,&g_mutex);
        }
        g_va--;
        printf("i am [%p],eat one [%d]\n",pthread_self(),g_va);
        pthread_mutex_unlock(&g_mutex);//解锁i
        pthread_cond_signal(&make);//唤醒生产者来生产资源
    }
    return NULL;
}

void* Make(void* arg)
{
    (void)arg;
    while(1)
    {
        pthread_mutex_lock(&g_mutex);
        while(g_va==1)//循环判断资源是否有资源，有：则不必再生产，进入等待队列。
        {
            pthread_cond_wait(&make,&g_mutex);
        }
        g_va++;
        printf("i am [%p],make one [%d]\n",pthread_self(),g_va);
        pthread_mutex_unlock(&g_mutex);//解锁
        pthread_cond_signal(&con);
    }
    return NULL;
}

int main()
{
    pthread_mutex_init(&g_mutex,NULL);//初始化互斥锁
    pthread_cond_init(&make,NULL);//因为只有一个PCB等待队列有可能出现阻塞，所以定义两个条件变量，生产者进生产者PCB，消费者一样。这样就不会存在问题。
    pthread_cond_init(&con,NULL);
    pthread_t tid[tm];
    for(int i=0;i<tm;++i)
    {   
    int ret=-1;
    ret= pthread_create(&tid[i],NULL,Eat,NULL);
    if(ret!=0)
    {
        printf("creat pthread error\n");
        return 0;
    }
    ret= pthread_create(&tid[i],NULL,Make,NULL);
    if(ret!=0)
    {
        printf("creat pthread error\n");
        return 0;
    }
    }

    for(int i=0;i<tm;i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&g_mutex);  //销毁互斥锁
    pthread_cond_destroy(&make);
    pthread_cond_destroy(&con);
    return  0;
}
