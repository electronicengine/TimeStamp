#ifndef DRIVER_H
#define DRIVER_H


#include <QMainWindow>
#include <queue>
#include <QThread>
#include <QTimer>
#include <ftdsocket.h>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include "ftd3xx.h"
#include "raundrobinmemory.h"

#define RAUNDROBIN_SIZE 100


class MainWindow;

class Driver : public QThread
{
    Q_OBJECT

    FtdSocket Device_;
    RaundRobinMemory<USBValueContainer> Memory_;

    int Value_Index;

public:
    Driver(QObject * parent = 0);

    std::vector<USBValueContainer> &getValueQueue();
    void parseAndWriteValues(int *Data);
    int bitExtracted(int Data, int K, int P);
    void waitTermination();

    volatile bool Terminate_;

protected:
    void run();


};


#endif // DRIVER_H
