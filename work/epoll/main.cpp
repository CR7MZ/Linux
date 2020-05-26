#include "Tcp.hpp"
#include "epoll_svr.hpp"

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        cout<<"Start this process use in: ./tcpsvr [ip] [port]"<<endl;
        return 0;
    }

    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    Tcp listen_tcpsvr;
    listen_tcpsvr.Socket();
    listen_tcpsvr.Bind(ip,port);
    listen_tcpsvr.Listen();

    epoll_svr es;
    es.Add(listen_tcpsvr.getfd());

    while(1)
    {
        std::vector<Tcp> vec;
        if(!es.Wait(vec))
        {
            continue;
        }
        
        for(size_t i = 0;i < vec.size();i++)
        {
            if(vec[i].getfd() == listen_tcpsvr.getfd())
            {
                Tcp peer;
                struct sockaddr_in peers; 
                if(!listen_tcpsvr.Accept(peer,peers))
                {
                    cout<<"accpet failed"<<endl;
                    continue;
                }

                cout<<"i have a new accpet ,ip :"<<inet_ntoa(peers.sin_addr)<<",port :"<<ntohs(peers.sin_port)<<endl;
                es.Add(peer.getfd());
            }
            else 
            {
                printf("recv data from %d\n", vec[i].getfd());
                char buf[1024];
                if(!read(vec[i].getfd(),buf,sizeof(buf)-1))
                {
                    es.Del(vec[i].getfd());
                    vec[i].Close();
                }

                cout<< "svr read from cil :"<<buf<<endl;
            }
        }
    }
    listen_tcpsvr.Close();
    return 0;
}
