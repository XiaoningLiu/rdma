/* filename: rdma_server.cpp */

#include <iostream>

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

    *channel >> msg;
    if (msg == CONNECT_ESTABLISHED)
    {
        cout<<"RDMAServer::acceptConnection(), receive CONNECT_ESTABLISHED"<<endl;
        *channel << CONNECT_ESTABLISHED_ACK;
    }

    return true;
}

bool RDMAServer::dealEvent()
{
    std::string msg;
    *channel >> msg;

    string offset_str;
    string memSize_str;

    int offset;
    int memSize;

    if (msg == EVENT_READ)
    {
        cout<<"RDMAServer::dealEvent(), got EVENT_READ"<<endl;
        *channel >> offset_str;
        *channel >> memSize_str;
        offset = str2int(offset_str);
        size   = str2int(memSize_str);

        if (offset + memSize > size)
        {
            cerr<<"RDMAServer::dealEvent(), Request size too large"<<endl;
            *channel << EVENT_READ_FAIL_ACK + " ";
            return false;
        }

        uchar* mem = memory + offset;
        
        // transfer all memory data via char?
        for (int i = 0; i < size; i++)
        {
            *channel << uchar2str( *mem ) + " ";
            mem++;
        }

        *channel << EVENT_READ_ACK + " ";
        return true;
    }
    else if (msg == EVENT_WRITE)
    {
        cout<<"RDMAServer::dealEvent(), got EVENT_WRITE"<<endl;
        *channel >> offset_str;
        *channel >> memSize_str;
        offset = str2int(offset_str);
        size   = str2int(memSize_str);

        if (offset + memSize > size)
        {
            cerr<<"RDMAServer::dealEvent(), Request size too large"<<endl;
            *channel << EVENT_WRITE_FAIL_ACK + " ";
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

        *channel << EVENT_WRITE_ACK + " ";
        return true;
    }
    else if (msg == DISCONNECT)
    {
        cout<<"RDMAServer::dealEvent(), got DISCONNECT"<<endl;
        return disconnect();
    }
    else
    {
        cerr<<"Unkonw msg: "<<msg<<endl;
        return true;
    }
}

bool RDMAServer::disconnect()
{
    *channel << DISCONNECT_ACK + " ";
    return true;
}
