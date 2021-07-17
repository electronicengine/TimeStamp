#include "driver.h"
#include "mainwindow.h"



Driver::Driver(QObject *parent) : QThread(parent)
{
    Main_Window_ = dynamic_cast<MainWindow *>(parent);

    Device_.openSocket();
    Terminate_ = false;
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

    uint8_t lock = bitExtracted(*Data, 1, 0);
    uint8_t channel = bitExtracted(*Data, 3, 1) + 1;
    uint32_t coarse = bitExtracted(*Data, 20, 4);
    uint8_t fine = bitExtracted(*Data, 6, 24);


    Main_Window_->updateValues(&channel, &fine, &coarse, &lock);

}



void Driver::run()
{
    int value = 0;


    while(1)
    {
        if(Terminate_ == true)
            break;

        value = Device_.read32();
        if(value != FAIL)
            parseAndWriteValues(&value);
        else
            Main_Window_->usbReadFailed();
    }

    Terminate_ = false;

}
