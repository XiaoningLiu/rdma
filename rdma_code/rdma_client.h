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
#include <queue>
#include <pthread.h>

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

    void* thr_fn(void *arg);
private:
   std::queue<Event> requests;
   std::queue<Event> completes;
   int event_id;
   pthread_t ntid;
   pthread_mutex_t mutex;
   bool thr_fin;

   ClientSocket *channel;
   uchar* memory;
   int    size;
};

#endif//RDMACLIENT_H
