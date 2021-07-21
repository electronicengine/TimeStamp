
#include <QDebug>
#include <QMessageBox>
#include "graphworker.h"
#include "ui_graphwindow.h"

using namespace std::placeholders;



GraphWorker::GraphWorker(QWidget *parent) : GraphWindow(parent)
{
    Thread_Pool.setWorkerSize(THREADPOOL_SIZE);

    Total_Refresh_Count = 0;
    Current_Refresh_Count = 0;

    Driver_ = new Driver(this);

    connect(ui->start_button, SIGNAL(toggled(bool)), this, SLOT(startOrStopGraphWork(bool)));
    connect(&Refresh_Timer, SIGNAL(timeout()), this, SLOT(refreshGraph()));
    connect(this, SIGNAL(updateProgressBar(int)), ui->progress_bar, SLOT(setValue(int)));



    Signals_.push_back(std::bind(&GraphWorker::setPanel1Text, this, _1));
    Signals_.push_back(std::bind(&GraphWorker::setPanel2Text, this, _1));
    Signals_.push_back(std::bind(&GraphWorker::setPanel3Text, this, _1));
    Signals_.push_back(std::bind(&GraphWorker::setPanel4Text, this, _1));
    Signals_.push_back(std::bind(&GraphWorker::setPanel5Text, this, _1));
    Signals_.push_back(std::bind(&GraphWorker::setPanel6Text, this, _1));

}



void GraphWorker::calculateTimeStamp(USBValueContainer *Container)
{
    Container->timestamp = Container->coarse * Container->fine;
}



void GraphWorker::attachPanel()
{
    int signal_count = 0;


    foreach (GraphContainer container, Series_List)
    {
        switch (signal_count)
        {
            case 0:
                connect(this, SIGNAL(setPanel1Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 1:
                connect(this, SIGNAL(setPanel2Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 2:
                connect(this, SIGNAL(setPanel3Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 3:
                connect(this, SIGNAL(setPanel4Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 4:
                connect(this, SIGNAL(setPanel5Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 5:
                connect(this, SIGNAL(setPanel6Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;

            default:
                break;

        }

        signal_count++;

    }


}




void GraphWorker::detachPanel()
{
    int signal_count = 0;


    foreach (GraphContainer container, Series_List)
    {
        switch (signal_count)
        {
            case 0:
                disconnect(this, SIGNAL(setPanel1Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 1:
                disconnect(this, SIGNAL(setPanel2Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 2:
                disconnect(this, SIGNAL(setPanel3Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 3:
                disconnect(this, SIGNAL(setPanel4Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 4:
                disconnect(this, SIGNAL(setPanel5Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;
            case 5:
                disconnect(this, SIGNAL(setPanel6Text(QString)), container.Panel_.Counter_Panel, SLOT(setText(QString)));
                break;

            default:
                break;

        }

        signal_count++;

    }

}



void GraphWorker::startOrStopGraphWork(bool State)
{

    if(State)
    {

        qDebug() << "Started";

        Total_Refresh_Count = (ui->record_time_box->value() * 1000)/ ui->refresh_rate_box->value();

        attachPanel();

        emit updateProgressBar(0);
        qDebug() << ui->refresh_rate_box->value();
        Refresh_Timer.start(ui->refresh_rate_box->value());
        Driver_->start();
    }
    else
    {
        qDebug() << "Terminated";

        detachPanel();
        Refresh_Timer.stop();
        Driver_->waitTermination();
    }

}



void GraphWorker::refreshGraph()
{
    std::vector<USBValueContainer> &values = Driver_->getValueQueue();

    std::future<void> results[values.size()];

    for(size_t i = 0; i < values.size(); i++)
    {
        auto binded_function = std::bind(&GraphWorker::calculateTimeStamp, this, _1);
        results[i] = Thread_Pool.pushWork(binded_function, &values[i]);
    }

    processData(&results[0], values);

    if(Current_Refresh_Count >= Total_Refresh_Count)
    {
        Refresh_Timer.stop();
        ui->start_button->toggle();
        QMessageBox::information(this, "Done", "Record is finished");
        Current_Refresh_Count = 0;
    }

    emit updateProgressBar((float)((float)Current_Refresh_Count / Total_Refresh_Count) * 100);
    Current_Refresh_Count++;

}



void GraphWorker::processData(std::future<void> *Results, std::vector<USBValueContainer> &Values)
{

    Channel1_Count.clear();
    Channel2_Count.clear();
    Channel3_Count.clear();
    Channel4_Count.clear();

    for(size_t i = 0; i < Values.size(); i++)
    {
        Results[i].get();

        switch (Values[i].channel)
        {
            case SeriesType::c1:
                Channel1_Count.push_back(Values[i].timestamp);
                break;
            case SeriesType::c2:
                Channel2_Count.push_back(Values[i].timestamp);
                break;
            case SeriesType::c3:
                Channel3_Count.push_back(Values[i].timestamp);
                break;
            case SeriesType::c4:
                Channel4_Count.push_back(Values[i].timestamp);
                break;

            default:
                break;
        }
    }

    drawGraph();

}



void GraphWorker::drawGraph()
{
    int signal_count = 0;



    foreach (GraphContainer container, Series_List)
    {



        switch (container.Type_)
        {

            case SeriesType::c1:
                emit Signals_[signal_count](QString::number(Channel1_Count.size()));
                updateSeries(container, Channel1_Count);

                break;

            case SeriesType::c2:
                emit Signals_[signal_count](QString::number(Channel2_Count.size()));
                updateSeries(container, Channel2_Count);

                break;

            case SeriesType::c3:
                emit Signals_[signal_count](QString::number(Channel3_Count.size()));
                updateSeries(container, Channel3_Count);

                break;

            case SeriesType::c4:
                emit Signals_[signal_count](QString::number(Channel4_Count.size()));
                updateSeries(container, Channel4_Count);

                break;




            break;
        default:
            break;
        }


        signal_count++;
    }

}



void GraphWorker::updateSeries(const GraphContainer &Container, const std::vector<int> &ChannelCount)
{

    QVector<QPointF> old_values= Container.Series_->pointsVector();
    QVector<QPointF> series_values;


    for(int i = 0; i < 99; i++)
        series_values.append(QPointF(i,old_values.at(i+1).y()));

    series_values.append(QPointF(100, ChannelCount.size()));
    Container.Series_->replace(series_values);

}
