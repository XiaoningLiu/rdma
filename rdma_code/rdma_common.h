/* Common verbes */

#ifndef RDMA_COMMON_H
#define RDMA_COMMON_H

#include <string>
#include <sstream>

typedef unsigned char uchar;
typedef unsigned int  uint;

enum EVENT_TYPE {ERROR, READ, WRITE, CLOSE};

const std::string CONNECT_REQUEST = "CONNECT_REQUEST";
const std::string CONNECT_REQUEST_ACK = "CONNECT_REQUEST_ACK";
const std::string CONNECT_ESTABLISHED = "CONNECT_ESTABLISHED";
const std::string CONNECT_ESTABLISHED_ACK = "CONNECT_ESTABLISHED_ACK";
const std::string ACK = "ACK";
const std::string DISCONNECT = "DISCONNECT";
const std::string DISCONNECT_ACK = "DISCONNECT_ACK";
const std::string EVENT_READ  = "EVENT_READ";
const std::string EVENT_READ_START  = "EVENT_READ_START";
const std::string EVENT_READ_ACK  = "EVENT_READ_ACK";
const std::string EVENT_READ_FAIL_ACK  = "EVENT_READ_FAIL_ACK";
const std::string EVENT_WRITE = "EVENT_WRITE";
const std::string EVENT_WRITE_START = "EVENT_WRITE_START";
const std::string EVENT_WRITE_ACK = "EVENT_WRITE_ACK";
const std::string EVENT_WRITE_FAIL_ACK = "EVENT_WRITE_FAIL_ACK";

uint uchar2uint(uchar c);
uchar uint2uchar(uint i);
std::string uchar2str(uchar c);
uchar str2uchar(std::string str);
int str2int(std::string str);
std::string int2str(int i);

struct Event
{
  std::string type;
  int localOffset;
  int remoteOffset;
  int size;
};

#endif//RDMA_COMMON_H
