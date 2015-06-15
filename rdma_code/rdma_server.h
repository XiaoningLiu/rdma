/* filename:  rdma_server.h 
 * ClassName: RDMAServer
 * Create & emulate rdma server
 */

#ifndef RDMASERVER_H
#define RDMASERVER_H

class RDMAServer
{	
public:
    RDMAServer();
    ~RDMAServer();
    bool bind(int port);
    bool creatChannel(); // wait until receive "CONNECT_REQUEST"
    bool registerMemory(uchar* p, int size);
    bool acceptConnection(); // receive CONNECT_REQUEST, send ACK, receive CONNECT_ESTABLISHED, send ACK
    bool dealEvent(); // judge evenet types & deal different msg

private:
    bool disconnect(); //

    ServerSocket *server;  // Initialized in bind()
    ServerSocket *channel; // Initialized in waitConnection()
   
    uchar* memory; // shared memory start address
    int   size;   // shared memory size
};

#endif //RDMASERVER_H
