#pragma once
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <sys/epoll.h>

#include "Tcp.hpp"

using namespace std;

class epoll_svr
{
    public:
        epoll_svr()
        {
            epoll_fd = epoll_create(10);
        }

        bool Add(int fd)
        {
            struct epoll_event events;
            events.events = EPOLLIN;
            events.data.fd = fd;
            int ret = epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&events);
            if(ret < 0)
            {
                perror("epoll_ctl failed!!!");
                return false;
            }
            return true;
        }
        
        bool Del(int fd)
        {

            int ret = epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,NULL);
            if(ret < 0)
            {
                perror("epoll_ctl failed!!!");
                return false;
            }
            return true;
        }

        bool Wait(vector<Tcp>& vec)
        {
            struct epoll_event arry[1000];
            
            int ret = epoll_wait(epoll_fd,arry,sizeof(arry)/sizeof(arry[0]),-1);
            if(ret < 0)
            {
                perror("epoll_wait failed!!!");
                return false;
            }

            for(int i = 0;i < ret;i++)
            {
                Tcp tcp;
                tcp.setfd(arry[i].data.fd);
                vec.push_back(tcp);
            }
            return true;
        }
    private:
        //epoll操作句柄：
        int  epoll_fd;
};
