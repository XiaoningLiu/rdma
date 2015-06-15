/* filename:  rdma_client.h 
 * ClassName: RDMAClient
 * Emulate the rdma client
 */

#ifndef RDMACLIENT_H
#define RDMACLIENT_H

#include "ClientSocket.h"
#include "SocketException.h"
#include "rdma_common.h"
#include <string>

class RDMAClient
{
public:
    RDMAClient();
    ~RDMAClient();
    bool createChannel(std::string serverIp, int port);
    bool registerMemory(uchar* m, int size);
    bool connect();
    bool read(int localOffset, int remoteOffset, int remoteSize);
    bool write(int localOffset, int remoteOffset, int remoteSize);
    bool disconnect();
private:
   ClientSocket *channel;
   uchar* memory;
   int    size;
};

#endif//RDMACLIENT_H
