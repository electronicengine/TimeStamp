#include "ftdsocket.h"
#include <QDebug>



FtdSocket::FtdSocket()
{
    Socket_Handle = NULL;
}



void FtdSocket::closeSocket()
{
    FT_Close(Socket_Handle);
}



int FtdSocket::openSocket()
{


    FT_Create(0, FT_OPEN_BY_INDEX, &Socket_Handle);

    if (!Socket_Handle)
    {
        qDebug() << "Failed to create ftd device";

        return FAIL;
    }

     qDebug() << "Device created";

     return SUCCESS;
}



std::vector<uint8_t> FtdSocket::readFromFTD(int Lenght, int Channel)
{
    DWORD count;
    std::vector<uint8_t> data;
    data.reserve(Lenght);

    if (FT_OK != FT_ReadPipeEx(Socket_Handle, Channel, &data[0], Lenght,
                &count, TIMEOUT))
    {
        qDebug() << "Failed to read";
        return data;
    }
    qDebug() << "Read bytes" << count;

    return data;
}



int FtdSocket::read32()
{
    DWORD count;
    uint32_t data;

    if (FT_OK != FT_ReadPipe(Socket_Handle, READ_PIPE, (uint8_t *)&data, sizeof(uint32_t),
                &count, NULL))
    {
//        qDebug() << "Failed to read";
        return FAIL;
    }

    return data;
}



int FtdSocket::writeToFtd(std::vector<uint8_t> &Data, int Channel)
{
    DWORD count;

    if (FT_OK != FT_WritePipeEx(Socket_Handle, Channel, &Data[0], Data.size(),
                &count, TIMEOUT))
    {
        qDebug() << "Failed to write";
        return FAIL;
    }

    qDebug() << "Wrote bytes\r\n" << count;

    return SUCCESS;
}
