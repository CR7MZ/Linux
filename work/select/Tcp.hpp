#pragma once
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <stdlib.h>


#define Max_Num 10 

class Tcp
{
    private:
        int _sockfd;
    private:
        void MakeSockaddr(struct sockaddr_in &addr,const std::string &ip,const uint16_t &port)
        {
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
        }
    public:
        //创建套接字
        bool Socket()
        {
            _sockfd=socket(AF_INET,SOCK_STREAM,6);
            if(_sockfd<0)
            {
                perror("socket error!!!");
                return false;
            }
            return true;
        }

        void setfd(int fd)
        {
           _sockfd = fd; 
        }

        int getfd()
        {
            return _sockfd;
        }

        //套接字赋值
        bool Bind(const std::string &ip,const uint16_t &port)
        {
            struct sockaddr_in addr;
            MakeSockaddr(addr,ip,port);
            int ret = -1;
            int len = sizeof(struct sockaddr_in);
            ret = bind(_sockfd,(struct sockaddr*)&addr,len);
            if(ret<0)
            {
                perror("bind error");
                return false;
            }
            return true;
        }
        //监听（服务器端）
        bool Listen()
        {
            int ret = listen(_sockfd,Max_Num);
            if(ret<0)
            {
                perror("listen error");
                return false;
            }
            return true;
        }
        //向服务器连接（客户端）
        bool Connect(const std::string &ip,const uint16_t &port)
        {
            struct sockaddr_in addr;
            MakeSockaddr(addr,ip,port);
            socklen_t len = sizeof(struct sockaddr);
            int ret = connect(_sockfd,(struct sockaddr*)&addr,len);
            if(ret<0)
            {
                perror("connect error");
                return false;
            }
            return true;
        }
        //接收套接字（服务器端）
        bool Accept(Tcp &sock,struct sockaddr_in& addr)
        {
            int newsock=-1;
            socklen_t len = sizeof(struct sockaddr_in);
            newsock = accept(_sockfd,(struct sockaddr*)&addr,&len);
            if(newsock<0)
            {
                perror("accpet error");
                return false;
            }
            sock._sockfd = newsock;
            return true;
        }
        //数据传输
        
        void Close()
        {
            close(_sockfd);
        }
        //关闭套接字
};
