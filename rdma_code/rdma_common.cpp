#include "rdma_common.h"

uint uchar2uint(uchar c)
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

    ss << uchar2uint(c);
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

int str2int(std::string str)
{
    std::stringstream ss(str);

    int i;
    ss >> i;

    return i;
}

std::string int2str(int i)
{
    std::stringstream ss;
    std::string str;

    ss << i;
    ss >> str;

    return str;
}

