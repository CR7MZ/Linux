#include <pthread.h>
#include <unistd.h>
#include <iostream>
using namespace std;
void* test(void* arg)
{
    while(1)
    {
        (void) arg;
        sleep(1);
        cout<<"linux"<<endl;
    }
         sleep(30);
        return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,test,NULL);
    while(1)
    {
        sleep(1);
    cout<<"zhu "<<endl;
    }
    return 0;
}
