#ifndef DRIVER_H
#define DRIVER_H


#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <ftdsocket.h>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include "ftd3xx.h"


class MainWindow;

class Driver : public QThread
{
    Q_OBJECT

    FtdSocket Device_;
    MainWindow *Main_Window_;

public:
    Driver(QObject * parent = 0);
    void parseAndWriteValues(int *Data);
    int bitExtracted(int Data, int K, int P);
    void waitTermination();

    volatile bool Terminate_;

protected:
    void run();


};


#endif // DRIVER_H
