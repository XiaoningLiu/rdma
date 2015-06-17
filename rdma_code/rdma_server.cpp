/* filename: rdma_server.cpp */

#include <iostream>
#include <stdio.h>
#include "rdma_server.h"

using namespace::std;

RDMAServer::RDMAServer()
{
    server = 0;
    channel = 0;
    memory = 0;
    size = 0;
}

RDMAServer::~RDMAServer()
{
    delete server;
    delete channel;
}

bool RDMAServer::bind(int port)
{
    if (server != 0)
    {
        cerr<<"Server has been binded"<<endl;
        return false;
    }

    try
    {
        server = new ServerSocket(port);
    }
    catch ( SocketException& e )
    {
        std::cerr<< "Cannot bind, Exception was caught:" << e.description() << "\nExiting.\n";
        return false;
    }

    return true;
}

bool RDMAServer::createChannel()
{
    if (channel != 0)
    {
        cerr<<"Channel has been created"<<endl;
        return false;
    }

    try
    {
        channel = new ServerSocket();
        server->accept ( *channel );
    }
    catch ( SocketException& e )
    {
        std::cerr<< "Cannot create channel, Exception was caught:" << e.description() << "\nExiting.\n";
        return false;
    }

    return true;
}

bool RDMAServer::registerMemory(uchar *p, int s)
{
    memory = p;
    size = s;
}

bool RDMAServer::acceptConnection()
{
    string msg;
    *channel >> msg;

    if (msg == CONNECT_REQUEST)
    {
        cout<<"RDMAServer::acceptConnection(), receive CONNECT_REQUEST"<<endl;
        *channel << CONNECT_REQUEST_ACK;
    }
    else
    {
        cerr<<"RDMAServer::acceptConnection(), receive unkonwn msg: "<<msg<<endl;
        return false;
    }

    *channel >> msg;
    if (msg == CONNECT_ESTABLISHED)
    {
        cout<<"RDMAServer::acceptConnection(), receive CONNECT_ESTABLISHED"<<endl;
        *channel << CONNECT_ESTABLISHED_ACK;
    }
    else
    {
        cerr<<"RDMAServer::acceptConnection(), receive unkonwn msg: "<<msg<<endl;
        return false;
    }

    return true;
}

EVENT_TYPE RDMAServer::dealEvent()
{
    std::string msg;
    *channel >> msg;

    string offset_str;
    string memSize_str;

    int offset;
    int memSize;

    if (msg == EVENT_READ)
    {
        *channel << ACK;

        cout<<"RDMAServer::dealEvent(), got EVENT_READ"<<endl;
        *channel >> offset_str;
        *channel << ACK;
        cout<<"offset_str: "<<offset_str<<endl;

        *channel >> memSize_str;
        cout<<"memSize_str: "<<memSize_str<<endl;

        offset = str2int(offset_str);
        memSize  = str2int(memSize_str);

        if (offset + memSize > size)
        {
            cerr<<"RDMAServer::dealEvent(), Request size too large"<<endl;
            *channel << EVENT_READ_FAIL_ACK ;
            return ERROR;
        }
        else
        {
            *channel << EVENT_READ_START ;
        }

        *channel >> msg;

        uchar* mem = memory + offset;
        
        // transfer all memory data via char?
        for (int i = 0; i < memSize; i++)
        {
            cout<<"send "<<uchar2str( *mem );
            printf("\t0x%.2x\n", uchar2uint( *mem )); 
            *channel << uchar2str( *mem ) ;
            mem++;

            *channel >> msg;
        }

        *channel << EVENT_READ_ACK ;
        return READ;
    }
    else if (msg == EVENT_WRITE)
    {
        cout<<"RDMAServer::dealEvent(), got EVENT_WRITE"<<endl;
        *channel << ACK;

        *channel >> offset_str;
        cout<<"offset_str "<<offset_str<<endl;
        *channel << ACK;

        *channel >> memSize_str;
        cout<<"memSize_str "<<memSize_str<<endl;

        offset = str2int(offset_str);
        memSize = str2int(memSize_str);

        if (offset + memSize > size)
        {
            cerr<<"RDMAServer::dealEvent(), Request size too large "<<offset<<" "<<memSize<<endl;
            *channel << EVENT_WRITE_FAIL_ACK ;
            return ERROR;
        }
        else
        {
            *channel << EVENT_WRITE_START ;
        }

        cout<<"RDMAServer::dealEvent(), start to receive write msg"<<endl;
        // Got memory data & save to memory
        uchar* mem = memory + offset;
        
        std::string data_str;
        for (int i = 0; i < memSize; i++)
        {
            *channel >> data_str;
            *mem = str2uchar(data_str);
            cout<<"get "<< data_str;
            printf("\t0x%.2x\n", uchar2uint( *mem )); 
            mem++;

            if (i < memSize - 1)
            {
                *channel << ACK;
            }
        }

        *channel << EVENT_WRITE_ACK ;
        cout<<"EVENT_WRITE_ACK"<<endl;
        return WRITE;
    }
    else if (msg == DISCONNECT)
    {
        cout<<"RDMAServer::dealEvent(), got DISCONNECT"<<endl;
        disconnect();
        return CLOSE;
    }
    else
    {
        cerr<<"Unkonw msg: "<<msg<<endl;
        return ERROR;
    }
}

bool RDMAServer::disconnect()
{
    *channel << DISCONNECT_ACK ;
    return true;
}
