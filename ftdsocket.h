#ifndef FTDSOCKET_H
#define FTDSOCKET_H


#include <iostream>
#include <vector>
#include <inttypes.h>
#include "ftd3xx.h"

#define FIFO_CHANNEL_1	0
#define FIFO_CHANNEL_2	1
#define FIFO_CHANNEL_3	2
#define FIFO_CHANNEL_4	3

#define TIMEOUT 1000

#define FAIL    -1
#define SUCCESS 0

class FtdSocket
{

    FT_HANDLE Socket_Handle;


public:
    FtdSocket();

    void closeSocket();
    int openSocket();
    std::vector<uint8_t> readFromFTD(int Lenght, int Channel);
    int read32();
    int writeToFtd(std::vector<uint8_t> &Data, int Channel);


};

#endif // FTDSOCKET_H
