#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <ftdsocket.h>
#include <QDebug>
#include <QLabel>
#include <QString>
#include <QPushButton>
#include "driver.h"


#define CHANNEL_NUMBER 8

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int bitExtracted(int Data, int K, int P);
    void updateValues(uint8_t *Channel, uint8_t *Fine, uint32_t *Coarse, uint8_t *Lock);
    void usbReadFailed();

private:
    Ui::MainWindow *ui;
    Driver *Driver_;

signals:
    void updateCoarse1(QString);
    void updateCoarse2(QString);
    void updateCoarse3(QString);
    void updateCoarse4(QString);
    void updateCoarse5(QString);
    void updateCoarse6(QString);
    void updateCoarse7(QString);
    void updateCoarse8(QString);

    void updateFine1(QString);
    void updateFine2(QString);
    void updateFine3(QString);
    void updateFine4(QString);
    void updateFine5(QString);
    void updateFine6(QString);
    void updateFine7(QString);
    void updateFine8(QString);

    void updateLock1(bool);
    void updateLock2(bool);
    void updateLock3(bool);
    void updateLock4(bool);
    void updateLock5(bool);
    void updateLock6(bool);
    void updateLock7(bool);
    void updateLock8(bool);

private slots:
    void clearButtonClicked();



};
#endif // MAINWINDOW_H
