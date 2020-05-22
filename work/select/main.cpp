#include "Tcp.hpp"
#include "SelectSvr.hpp"

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
    listen_tcpsvr.Bind(ip,port);
    listen_tcpsvr.Listen();

    SelectSvr st;
    st.Add(listen_tcpsvr.getfd());

    while(1)
    {
        std::vector<Tcp> vec;
        if(!st.SelectWait(vec))
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

                cout<<"i have a new accpet ,ip :"<<peers.sin_addr.s_addr<<",port :"<<peers.sin_port<<endl;
                st.Add(peer.getfd());
            }
            else 
            {
                printf("recv data from %d\n", vec[i].getfd());
                char buf[1024];
                if(!read(vec[i].getfd(),buf,sizeof(buf)-1))
                {
                    st.Delete(vec[i].getfd());
                    vec[i].Close();
                }

                cout<< "svr read from cil :"<<buf<<endl;
            }
        }
    }
    listen_tcpsvr.Close();
    return 0;
}
