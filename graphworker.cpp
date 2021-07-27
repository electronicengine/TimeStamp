
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
    Axis_Maximum = 100;
    Current_Covergence_Index = 0;
    Recording_Started = false;

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

    //[(coarse_cntr_Y-coarse_cntr_X)*5 ns] - [(fine_cntr_Y +fine_cntr_X + 2) * 125 ps]

    Container->timestamp = ((Container->coarse - Container->pre_coarse) * 5) -
            ((Container->fine + Container->pre_fine + 2) * 125);

    Container->covergence_section = (Container->timestamp - Starting_TimeStamp) / Covergence_Treshold;

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



void GraphWorker::setGraphAxisMaximum(int Value)
{
    Chart_->axes(Qt::Vertical).back()->setRange(0, Value);
}



void GraphWorker::startOrStopGraphWork(bool State)
{

    if(State)
    {

        qDebug() << "Started";

        Covergence_Treshold = ui->convergence_box->value();

        Total_Refresh_Count = (ui->record_time_box->value() * 1000)/ ui->refresh_rate_box->value();

        attachPanel();

        emit updateProgressBar(0);
        qDebug() << ui->refresh_rate_box->value();
        Refresh_Timer.start(ui->refresh_rate_box->value());
        Driver_->start();

        qDebug() <<ui->browse_file->text();

        Saver_->openFile(ui->save_directory->text());

        Saver_->start();

    }
    else
    {
        qDebug() << "Terminated";
        Recording_Started = false;

        detachPanel();
        Refresh_Timer.stop();
        Driver_->waitTermination();

        Saver_->closeFile();

    }

}



void GraphWorker::refreshGraph()
{
    std::vector<USBValueContainer> &values = Driver_->getValueQueue();
    std::future<void> result;

    if(Recording_Started == false)
    {
        resetCounters();
        Recording_Started = true;
        calculateTimeStamp(&values[0]);
        Starting_TimeStamp = values[0].timestamp;
    }

    for(size_t i = 0; i < THREADPOOL_SIZE; i++)
    {

        auto binded_function = std::bind(&GraphWorker::calculateTimeStamp, this, _1);
        result = Thread_Pool.pushWork(binded_function, &values[i]);

    }

    processData(result, values);

    if(Current_Refresh_Count >= Total_Refresh_Count)
    {
        Refresh_Timer.stop();
        ui->start_button->toggle();
        QMessageBox::information(this, "Done", "Record is finished");
        Current_Refresh_Count = 0;
        Saver_->closeFile();
        Recording_Started = false;

    }

    emit updateProgressBar((float)((float)Current_Refresh_Count / Total_Refresh_Count) * 100);
    Current_Refresh_Count++;

}



void GraphWorker::processData(std::future<void> &Last_Result, std::vector<USBValueContainer> &Values)
{

    Last_Result.get();


    for(size_t i = 0; i < Values.size(); i++)
    {

        switch (Values[i].channel)
        {
            case SeriesType::c1:
                Channel1_Count++;
                break;
            case SeriesType::c2:
                Channel2_Count++;
                break;
            case SeriesType::c3:
                Channel3_Count++;
                break;
            case SeriesType::c4:
                Channel4_Count++;
                break;

            default:
                break;
        }

        checkCovergentChannels(Values[i]);

    }

    drawGraph();

    Saver_->saveValues(Values);

}



void GraphWorker::checkCovergentChannels(USBValueContainer &Container)
{


    if(Container.covergence_section == Current_Covergence_Index)
    {
        Current_Covergent_Channels[Container.channel - 1] = Container.channel;
    }
    else
    {
        int digit = 1;
        int type = 0;

        for(int i = 0; i < 4; i++)
        {
            if(Current_Covergent_Channels[i] != 0)
            {
                type += Current_Covergent_Channels[i] * digit++;
            }
        }

        countCovergentChannels((SeriesType::Type)type);

        for(int i = 0; i < 4; i++)
            Current_Covergent_Channels[i] = 0;
    }

}



void GraphWorker::countCovergentChannels(SeriesType::Type Type_)
{

    switch (Type_)
    {
        case SeriesType::c1_2:
            Channel1_2_Count++;
            break;
        case SeriesType::c1_3:
            Channel1_3_Count++;
            break;
        case SeriesType::c1_4:
            Channel1_4_Count++;
            break;
        case SeriesType::c2_3:
            Channel2_3_Count++;
            break;
        case SeriesType::c2_4:
            Channel2_4_Count++;
            break;
        case SeriesType::c3_4:
            Channel3_4_Count++;
            break;
        case SeriesType::c1_2_3:
            Channel1_2_3Count++;
            Channel1_2_Count++;
            Channel2_3_Count++;
            Channel1_3_Count++;
            break;
        case SeriesType::c1_3_4:
            Channel1_3_4Count++;
            Channel1_3_Count++;
            Channel3_4_Count++;
            Channel1_4_Count++;
            break;
        case SeriesType::c1_2_4:
            Channel1_2_4Count++;
            Channel1_2_Count++;
            Channel1_4_Count++;
            Channel2_4_Count++;
            break;
        case SeriesType::c2_3_4:
            Channel2_3_4Count++;
            Channel2_3_Count++;
            Channel3_4_Count++;
            Channel2_4_Count++;
            break;
        case SeriesType::c1_2_3_4:
            Channel1_2_Count++;
            Channel1_3_Count++;
            Channel1_4_Count++;
            Channel2_3_Count++;
            Channel2_4_Count++;
            Channel3_4_Count++;
            Channel1_2_3Count++;
            Channel1_2_4Count++;
            Channel2_3_4Count++;
            Channel1_2_3_4Count++;
            break;
        default:
            break;
    }
}



void GraphWorker::resetCounters()
{
    Channel1_Count = 0;
    Channel2_Count = 0;
    Channel3_Count = 0;
    Channel4_Count = 0;

    Channel1_2_Count = 0;
    Channel1_3_Count = 0;
    Channel1_4_Count = 0;
    Channel2_3_Count = 0;
    Channel2_4_Count = 0;
    Channel3_4_Count = 0;

    Channel1_2_3Count = 0;
    Channel1_2_4Count = 0;
    Channel1_3_4Count = 0;
    Channel2_3_4Count = 0;

    Channel1_2_3_4Count = 0;

    Current_Covergence_Index = 0;

    for(int i = 0; i < 4; i++)
        Current_Covergent_Channels[i] = 0;
}



void GraphWorker::drawGraph()
{
    int signal_count = 0;

    foreach (GraphContainer container, Series_List)
    {
        switch (container.Type_)
        {

            case SeriesType::c1:
                emit Signals_[signal_count](QString::number(Channel1_Count));
                updateSeries(container, Channel1_Count);

                break;

            case SeriesType::c2:
                emit Signals_[signal_count](QString::number(Channel2_Count));
                updateSeries(container, Channel2_Count);

                break;

            case SeriesType::c3:
                emit Signals_[signal_count](QString::number(Channel3_Count));
                updateSeries(container, Channel3_Count);

                break;

            case SeriesType::c4:
                emit Signals_[signal_count](QString::number(Channel4_Count));
                updateSeries(container, Channel4_Count);

                break;
            case SeriesType::c1_2:
                emit Signals_[signal_count](QString::number(Channel1_2_Count));
                    updateSeries(container, Channel1_2_Count);

                break;
            case SeriesType::c1_3:
                emit Signals_[signal_count](QString::number(Channel1_3_Count));
                updateSeries(container, Channel1_3_Count);

                break;
            case SeriesType::c1_4:
                emit Signals_[signal_count](QString::number(Channel1_4_Count));
                updateSeries(container, Channel1_4_Count);

                break;
            case SeriesType::c2_3:
                emit Signals_[signal_count](QString::number(Channel2_3_Count));
                updateSeries(container, Channel2_3_Count);

                break;
            case SeriesType::c2_4:
                emit Signals_[signal_count](QString::number(Channel2_4_Count));
                updateSeries(container, Channel2_4_Count);

                break;
            case SeriesType::c3_4:
                emit Signals_[signal_count](QString::number(Channel3_4_Count));
                updateSeries(container, Channel3_4_Count);

                break;
            case SeriesType::c1_2_3:
                emit Signals_[signal_count](QString::number(Channel1_2_3Count));
                updateSeries(container, Channel1_2_3Count);

                break;
            case SeriesType::c1_2_4:
                emit Signals_[signal_count](QString::number(Channel1_2_4Count));
                updateSeries(container, Channel1_2_4Count);

                break;
            case SeriesType::c2_3_4:
                emit Signals_[signal_count](QString::number(Channel2_3_4Count));
                updateSeries(container, Channel2_3_4Count);

                break;
            case SeriesType::c1_2_3_4:
                emit Signals_[signal_count](QString::number(Channel1_2_3_4Count));
                updateSeries(container, Channel1_2_3_4Count);

                break;

        default:
            break;
        }


        signal_count++;
    }

}



void GraphWorker::updateSeries(const GraphContainer &Container, int &ChannelCount)
{

    QVector<QPointF> old_values= Container.Series_->pointsVector();
    QVector<QPointF> series_values;

    if((int) ChannelCount > Axis_Maximum)
    {
        Axis_Maximum = ChannelCount;
        setGraphAxisMaximum(Axis_Maximum);
    }

    for(int i = 0; i < 99; i++)
        series_values.append(QPointF(i,old_values.at(i+1).y()));

    series_values.append(QPointF(100, ChannelCount));
    Container.Series_->replace(series_values);

}



