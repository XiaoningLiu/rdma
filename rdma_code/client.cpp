#include "rdma_client.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace::std;

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        cerr<<"need 3 parmeters, host, port & memSize, eg. ./client localhost 3000 100"<<endl;
        return 0;
    }

    string host(argv[1]);
    string port_str(argv[2]);
    string size_str(argv[3]);

    int port = str2int(port_str);
    int memSize = str2int(size_str);

    cout<<"host: "<<host<<" port: "<<port<<" size: "<<memSize<<endl;

    RDMAClient client;
    bool res = client.createChannel(host, port);
    if (res == false)
    {
        cerr<<"Client binds port failed: "<<port<<endl;
        return 0;
    }
    cout<<"Client channel success"<<endl;

    uchar* mem = (uchar*)malloc(memSize * sizeof(uchar)); 
    memset(mem, 0, sizeof(uchar) * memSize);
    res = client.registerMemory(mem, memSize);

    res = client.connect();
    if (res == false)
    {
        cerr<<"Client connect failed: "<<port<<endl;
        return 0;
    }
    cout<<"Client connect success"<<endl;

    const char *str1 = "Hello Bot"; // 10 bytes
    cout<<"str1: "<<str1<<endl;
    strcpy((char*)mem, str1); // set mem[0] - > mem[9] to string "Hello Bot"
    client.write(0, 0, strlen(str1) + 1); // write local memory mem[0]-mem[9] to remote memory [0]-[9]

    cout<<"client begin reading from remote "<<0<<" "<<strlen(str1) + 1<<endl;
    client.read(strlen(str1) + 1, 0, strlen(str1) + 1); // read remote memory [0]-[5] to local memory [6]-[12]

    uchar *memPointer = mem + strlen(str1) + 1;
    char str2[strlen(str1) + 1];
    strncpy(str2, (char*)memPointer, strlen(str1) + 1);
    cout<<"str2: "<<str2<<endl;

    cout<<"client begin reading from remote twice"<<0<<" "<<strlen(str1) + 1<<endl;
    client.read(0, 0, strlen(str1) + 1); // read remote memory [0]-[5] to local memory [6]-[12]

    memPointer = mem;
    char str3[strlen(str1) + 1];
    strncpy(str3, (char*)memPointer, strlen(str1) + 1);
    cout<<"str3: "<<str3<<endl;

    client.disconnect();
    cout<<"Client disconnect"<<endl;

    free(mem);
    return 0;
}
