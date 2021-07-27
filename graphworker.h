#ifndef GRAPHWORKER_H
#define GRAPHWORKER_H

#include <QObject>
#include <QTimer>
#include <functional>
#include <mutex>
#include "graphwindow.h"
#include "threadpool.h"
#include "driver.h"


#define THREADPOOL_SIZE 50

class GraphWorker : public GraphWindow
{
    Q_OBJECT
    ThreadPool Thread_Pool;
    QTimer Refresh_Timer;
    Driver *Driver_;

    int Total_Refresh_Count;
    int Current_Refresh_Count;
    int Axis_Maximum;
    int Covergence_Treshold;
    int Starting_TimeStamp;
    uint32_t Current_Covergence_Index;
    bool Recording_Started;

    int Channel1_Count;
    int Channel2_Count;
    int Channel3_Count;
    int Channel4_Count;

    int Channel1_2_Count;
    int Channel1_3_Count;
    int Channel1_4_Count;
    int Channel2_3_Count;
    int Channel2_4_Count;
    int Channel3_4_Count;

    int Channel1_2_3Count;
    int Channel1_2_4Count;
    int Channel1_3_4Count;
    int Channel2_3_4Count;

    int Channel1_2_3_4Count;


    int Current_Covergent_Channels[4];
    std::vector<std::function<void(QString)>> Signals_;
    std::mutex mux;

    void attachPanel();
    void detachPanel();
    void setGraphAxisMaximum(int Value);
    void updateSeries(const GraphContainer &Container, int &ChannelCount);
    void drawGraph();
    void processData(std::future<void> &Last_Result, std::vector<USBValueContainer> &Values);
    void calculateTimeStamp(USBValueContainer *Container);
    void checkCovergentChannels(USBValueContainer &Container);
    void countCovergentChannels(SeriesType::Type Type_);

    void resetCounters();

public:
    explicit GraphWorker(QWidget *parent = nullptr);



private slots:
    void startOrStopGraphWork(bool State);
    void refreshGraph();

signals:
    void setPanel1Text(QString);
    void setPanel2Text(QString);
    void setPanel3Text(QString);
    void setPanel4Text(QString);
    void setPanel5Text(QString);
    void setPanel6Text(QString);

    void updateProgressBar(int);

};

#endif // GRAPHWORKER_H
