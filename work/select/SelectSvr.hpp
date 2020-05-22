#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <vector>

#include "Tcp.hpp"

using namespace std;

class SelectSvr
{
    public:
        SelectSvr()
        {
            FD_ZERO(&readfd);
            max_readfd=-1;
        }

        void Add(int fd)
        {
            
            FD_SET(fd,&readfd);

            if(fd > max_readfd)
                max_readfd = fd;
        }

        bool SelectWait(vector<Tcp>& vec)
        {
            vector<Tcp> vc;   

            fd_set tmp = readfd;
            int ret = select(max_readfd+1,&tmp,NULL,NULL,NULL);

            if(ret < 0)
            {
                perror("select error!!!");
                return false;
            }
            else if(ret == 0)
            {
                return false;
            }

            for(int i = 0;i < max_readfd;i++)
            {
                if(FD_ISSET(i,&tmp))
                {
                    Tcp newfd;
                    newfd.setfd(i);
                    vec.push_back(newfd);
                }
            }
            return true;
        }

        void Delete(int fd)
        {
            FD_CLR(fd,&readfd);

            for(int i = max_readfd;i >= 0;i--)
            {
                if(!FD_ISSET(i,&readfd))
                    continue;

                max_readfd = i;
                break;
            }
        }
    private:
        fd_set readfd;

        int max_readfd;
};
