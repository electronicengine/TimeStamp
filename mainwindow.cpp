#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    Driver_ = new Driver(this);

    connect(this, SIGNAL(updateCoarse1(QString)), ui->coarse1, SLOT(setText(QString)));
    connect(this, SIGNAL(updateCoarse2(QString)), ui->coarse2, SLOT(setText(QString)));
    connect(this, SIGNAL(updateCoarse3(QString)), ui->coarse3, SLOT(setText(QString)));
    connect(this, SIGNAL(updateCoarse4(QString)), ui->coarse4, SLOT(setText(QString)));
    connect(this, SIGNAL(updateCoarse5(QString)), ui->coarse5, SLOT(setText(QString)));
    connect(this, SIGNAL(updateCoarse6(QString)), ui->coarse6, SLOT(setText(QString)));
    connect(this, SIGNAL(updateCoarse7(QString)), ui->coarse7, SLOT(setText(QString)));
    connect(this, SIGNAL(updateCoarse8(QString)), ui->coarse8, SLOT(setText(QString)));

    connect(this, SIGNAL(updateFine1(QString)), ui->fine1, SLOT(setText(QString)));
    connect(this, SIGNAL(updateFine2(QString)), ui->fine2, SLOT(setText(QString)));
    connect(this, SIGNAL(updateFine3(QString)), ui->fine3, SLOT(setText(QString)));
    connect(this, SIGNAL(updateFine4(QString)), ui->fine4, SLOT(setText(QString)));
    connect(this, SIGNAL(updateFine5(QString)), ui->fine5, SLOT(setText(QString)));
    connect(this, SIGNAL(updateFine6(QString)), ui->fine6, SLOT(setText(QString)));
    connect(this, SIGNAL(updateFine7(QString)), ui->fine7, SLOT(setText(QString)));
    connect(this, SIGNAL(updateFine8(QString)), ui->fine8, SLOT(setText(QString)));

    connect(this, SIGNAL(updateLock1(bool)), ui->lock1, SLOT(setChecked(bool)));
    connect(this, SIGNAL(updateLock2(bool)), ui->lock2, SLOT(setChecked(bool)));
    connect(this, SIGNAL(updateLock3(bool)), ui->lock3, SLOT(setChecked(bool)));
    connect(this, SIGNAL(updateLock4(bool)), ui->lock4, SLOT(setChecked(bool)));
    connect(this, SIGNAL(updateLock5(bool)), ui->lock5, SLOT(setChecked(bool)));
    connect(this, SIGNAL(updateLock6(bool)), ui->lock6, SLOT(setChecked(bool)));
    connect(this, SIGNAL(updateLock7(bool)), ui->lock7, SLOT(setChecked(bool)));
    connect(this, SIGNAL(updateLock8(bool)), ui->lock8, SLOT(setChecked(bool)));

    connect(ui->clear, SIGNAL(clicked(bool)), this, SLOT(clearButtonClicked()));


    Driver_->start();

}



MainWindow::~MainWindow()
{
    Driver_->waitTermination();
    delete ui;
}



void MainWindow::updateValues(uint8_t *Channel, uint8_t *Fine, uint32_t *Coarse, uint8_t *Lock)
{
    switch (*Channel)
    {
        case 1:

            emit updateFine1(QString::number(*Fine));
            emit updateCoarse1(QString::number(*Coarse));

            if(ui->lock1->isChecked() != *Lock)
                emit updateLock1(*Lock);

            break;
        case 2:
            emit updateFine2(QString::number(*Fine));
            emit updateCoarse2(QString::number(*Coarse));

            if(ui->lock2->isChecked() != *Lock)
                emit updateLock2(*Lock);

            break;
        case 3:
            emit updateFine3(QString::number(*Fine));
            emit updateCoarse3(QString::number(*Coarse));

            if(ui->lock3->isChecked() != *Lock)
                emit updateLock3(*Lock);

            break;
        case 4:
            emit updateFine4(QString::number(*Fine));
            emit updateCoarse4(QString::number(*Coarse));

            if(ui->lock4->isChecked() != *Lock)
                emit updateLock4(*Lock);

            break;
        case 5:
            emit updateFine5(QString::number(*Fine));
            emit updateCoarse5(QString::number(*Coarse));

            if(ui->lock5->isChecked() != *Lock)
                emit updateLock5(*Lock);

            break;
        case 6:
            emit updateFine6(QString::number(*Fine));
            emit updateCoarse6(QString::number(*Coarse));

            if(ui->lock6->isChecked() != *Lock)
                emit updateLock6(*Lock);

            break;
        case 7:
            emit updateFine7(QString::number(*Fine));
            emit updateCoarse7(QString::number(*Coarse));

            if(ui->lock7->isChecked() != *Lock)
                emit updateLock7(*Lock);

            break;
        case 8:
            emit updateFine8(QString::number(*Fine));
            emit updateCoarse8(QString::number(*Coarse));

            if(ui->lock8->isChecked() != *Lock)
                emit updateLock8(*Lock);

            break;
        default:
            break;
    }
}



void MainWindow::clearButtonClicked()
{

    emit updateFine1(QString::number(0));
    emit updateFine2(QString::number(0));
    emit updateFine3(QString::number(0));
    emit updateFine4(QString::number(0));
    emit updateFine5(QString::number(0));
    emit updateFine6(QString::number(0));
    emit updateFine7(QString::number(0));
    emit updateFine8(QString::number(0));

    emit updateCoarse1(QString::number(0));
    emit updateCoarse2(QString::number(0));
    emit updateCoarse3(QString::number(0));
    emit updateCoarse4(QString::number(0));
    emit updateCoarse5(QString::number(0));
    emit updateCoarse6(QString::number(0));
    emit updateCoarse7(QString::number(0));
    emit updateCoarse8(QString::number(0));

    emit updateLock1(false);
    emit updateLock2(false);
    emit updateLock3(false);
    emit updateLock4(false);
    emit updateLock5(false);
    emit updateLock6(false);
    emit updateLock7(false);
    emit updateLock8(false);

}



void MainWindow::usbReadFailed()
{
    emit updateLock1(false);
    emit updateLock2(false);
    emit updateLock3(false);
    emit updateLock4(false);
    emit updateLock5(false);
    emit updateLock6(false);
    emit updateLock7(false);
    emit updateLock8(false);
}

