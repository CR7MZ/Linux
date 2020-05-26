#include <stdio.h>
#include <iostream>
#include <poll.h>
#include <unistd.h>

using namespace std;

int main()
{
    struct pollfd array[10];

    array[0].fd = 0;
    array[0].events = POLLIN;

    while(1)
    {
        int ret = poll(array,1,-1);
        if(ret < 0)
        {
            perror("poll wrong!!!");
            return 0;
        }
        else if(ret == 0)
        {
            continue;;
        }
        
        for(size_t i = 0;i < sizeof(array)/sizeof(array[0]);i++)
        {
            if(array[i].revents == POLLIN)
            {
                char buf[1024] = {0};
                read(array[i].fd,buf,sizeof(buf));
                printf("i read is : %s\n",buf);
            }
        }
    }
    return 0;
}
