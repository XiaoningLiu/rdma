/* Common verbes */

#ifndef RDMA_COMMON_H
#define RDMA_COMMON_H

#include <string>
#include <sstream>

typedef unsigned char uchar;
typedef unsigned int  uint;

const std::string CONNECT_REQUEST = "CONNECT_REQUEST";
const std::string CONNECT_REQUEST_ACK = "CONNECT_REQUEST_ACK";
const std::string CONNECT_ESTABLISHED = "CONNECT_ESTABLISHED";
const std::string CONNECT_ESTABLISHED_ACK = "CONNECT_ESTABLISHED_ACK";
const std::string DISCONNECT = "DISCONNECT";
const std::string DISCONNECT_ACK = "DISCONNECT_ACK";
const std::string EVENT_READ  = "EVENT_READ";
const std::string EVENT_READ_ACK  = "EVENT_READ_ACK";
const std::string EVENT_READ_FAIL_ACK  = "EVENT_READ_FAIL_ACK";
const std::string EVENT_WRITE = "EVENT_WRITE";
const std::string EVENT_WRITE_ACK = "EVENT_WRITE_ACK";
const std::string EVENT_WRITE_FAIL_ACK = "EVENT_WRITE_FAIL_ACK";

uint uchar2int(uchar c)
{
   return uint(c); 
}

uchar uint2uchar(uint i)
{
    return uchar(i);
}

std::string uchar2str(uchar c)
{
    std::stringstream ss;
    std::string str;

    ss << uchar2int(c);
    ss >> str;

    return str;
}

uchar str2uchar(std::string str)
{
    std::stringstream ss(str);

    uint i;
    ss >> i;

    return uint2uchar(i);
}

#endif//RDMA_COMMON_H
