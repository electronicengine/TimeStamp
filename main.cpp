#include "mainwindow.h"
#include "graphwindow.h"
#include "graphworker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    GraphWorker w;

    w.show();

    return a.exec();
}
