#include <iostream>
#include <string>
#include "rdma_common.h"

using namespace std;

int main()
{
    for (uint i = 0; i < 256; i++)
    {
        cout<<"Test "<<i;
        uchar c = uint2uchar(i);
        string str = uchar2str(c);
        uchar c_2 = str2uchar(str);

        if ( c == c_2 )
        {
            cout<<"ok"<<endl;
        }
    }
    
    return 0;
}
