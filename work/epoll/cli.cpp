#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>

#include "Tcp.hpp"

using namespace std;

int main(int argc,char* argv[])
{
    if(argc != 3)
    {
        printf("./cli [ip] [port]\n");
        return 0;
    }

    string ip = argv[1];
    int16_t port = atoi(argv[2]);

    Tcp ts;
    ts.Socket();
    ts.Connect(ip,port);

    while(1)
    {
        printf("cli say : ");
        fflush(stdout);
        string out;
        cin>>out;

        write(ts.getfd(),out.c_str(),sizeof(out));
    }

    ts.Close();
    return 0;
}
