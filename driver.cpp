#include "driver.h"
#include "mainwindow.h"



Driver::Driver(QObject *parent) : QThread(parent)
{

    Memory_.setSize(RAUNDROBIN_SIZE);

    Device_.openSocket();
    Terminate_ = false;
    Value_Index = 0;
}



std::vector<USBValueContainer> &Driver::getValueQueue()
{

    Value_Index = 0;

    return Memory_.getLastQueue();
}


// Function to extract k bits from p position
// and returns the extracted value as integer
int Driver::bitExtracted(int Data, int K, int P)
{
    return (((1 << K) - 1) & (Data >> (P)));
}



void Driver::waitTermination()
{
    Terminate_ = true;

    while(Terminate_);
}



void Driver::parseAndWriteValues(int *Data)
{

    USBValueContainer container;

    container.index = Value_Index;
    container.lock = bitExtracted(*Data, 1, 0);
    container.channel = bitExtracted(*Data, 3, 1) + 1;
    container.coarse = bitExtracted(*Data, 20, 4);
    container.fine = bitExtracted(*Data, 6, 24);

    Memory_.pushBack(container);

}



void Driver::run()
{
    int value = 0;


    while(1)
    {
        if(Terminate_ == true)
            break;

//        value = Device_.read32();
//        if(value != FAIL)
            parseAndWriteValues(&value);
            usleep(100);

    }

    Terminate_ = false;

}
