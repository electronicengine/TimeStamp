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

    std::vector<int> Channel1_Count;
    std::vector<int> Channel2_Count;
    std::vector<int> Channel3_Count;
    std::vector<int> Channel4_Count;

    std::vector<SeriesType::Type> Single_Count;
    std::vector<SeriesType::Type> Double_Count;
    std::vector<SeriesType::Type> Triple_Count;
    std::vector<SeriesType::Type> Quad_Count;

    std::vector<std::function<void(QString)>> Signals_;
    std::mutex mux;

    void attachPanel();
    void detachPanel();


public:
    explicit GraphWorker(QWidget *parent = nullptr);


    void updateSeries(const GraphContainer &Container, const std::vector<int> &ChannelCount);
    void drawGraph();
    void processData(std::future<void> *Results, std::vector<USBValueContainer> &Values);
    void calculateTimeStamp(USBValueContainer *Container);

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


};

#endif // GRAPHWORKER_H
