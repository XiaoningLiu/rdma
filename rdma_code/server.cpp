#include "rdma_server.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace::std;

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        cerr<<"need 2 parmeters, port & memSize, eg. ./server 3000 100"<<endl;
        return 0;
    }

    string port_str(argv[1]);
    string size_str(argv[2]);

    int port = str2int(port_str);
    int memSize = str2int(size_str);

    cout<<"port: "<<port<<" size: "<<memSize<<endl;

    RDMAServer server;
    bool res = server.bind(port); // port 3000
    cout<<"Server bind port"<<endl;
    if (res == false)
    {
        cerr<<"Server binds port failed: "<<port<<endl;
        return 0;
    }

    res = server.createChannel();
    cout<<"Server create channel"<<endl;
    if (res == false)
    {
        cerr<<"Server create channel failed: "<<port<<endl;
        return 0;
    }

    uchar* mem = (uchar*)malloc(memSize * sizeof(uchar)); 
    memset(mem, 0, sizeof(uchar) * memSize);
    res = server.registerMemory(mem, memSize);
    cout<<"Server registerMemory"<<endl;

    res = server.acceptConnection();
    cout<<"Server acceptConnection"<<endl;
    if (res == false)
    {
        cerr<<"Server acceptConnection failed: "<<port<<endl;
        return 0;
    }

    EVENT_TYPE event = server.dealEvent(); // write
    cout<<"receive wirte"<<endl;
    event = server.dealEvent(); // read
    cout<<"receive read"<<endl;

    char newStr[10] = "123456789";
    strcpy((char*)mem, newStr);
    cout<<"receive read, try to modify memory 10->19"<<endl;

    event = server.dealEvent(); // read 
    event = server.dealEvent(); // disconnect
    cout<<"receive disconnect"<<endl;

    /*
    while (event != ERROR && event != CLOSE)
    {
        event = server.dealEvent();
        continue;
    }
    */

    free(mem);
    return 0;
}
