/* filename: rdma_server.cpp */

#include <iostream>
#include <rdma_server.h>
#include <rdma_common.h>

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
        std::cerr<< "Exception was caught:" << e.description() << "\nExiting.\n";
        return false;
    }

    return true;
}

bool RDMAServer::creatChannel()
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
        std::cerr<< "Exception was caught:" << e.description() << "\nExiting.\n";
        return false;
    }

    return true;
}

bool RDMAServer::dealEvent()
{
    std::string msg;
    *channel >> msg;

    int offset;
    int memSize;

    if (msg == EVENT_READ)
    {
        *channel >> offset;
        *channel >> memSize;

        if (offset + memSize > size)
        {
            cerr<<"Request size too large"<<endl;
            *channel << EVENT_READ_FAIL_ACK;
            return false;
        }

        uchar* mem = memory + offset;
        
        // transfer all memory data via char?
        for (int i = 0; i < size; i++)
        {
            *channel << uchar2str( *mem );
            mem++;
        }

        *channel << EVENT_READ_ACK;
    }
    else if (msg == EVENT_WRITE)
    {
        *channel >> offset;
        *channel >> memSize;

        if (offset + memSize > size)
        {
            cerr<<"Request write size too large"<<endl;
            *channel << EVENT_WRITE_FAIL_ACK;
            return false;
        }

        // Got memory data & save to memory
        uchar* mem = memory + offset;
        
        std::string data_str;
        for (int i = 0; i < size; i++)
        {
            *channel >> data_str;
            *mem = str2uchar(data_str);
            mem++;
        }

        *channel << EVENT_WRITE_ACK;
    }
    else if (msg == DISCONNECT)
    {
        //close socket

        *channel << DISCONNECT_ACK;
        return true;
    }
    else
    {
        cerr<<"Unkonw msg: "<<msg<<endl;
        return true;
    }
}
