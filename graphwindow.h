#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>
#include "containers.h"


#define MAX_LINE_NUM    6

namespace Ui {
class GraphWindow;
}

using namespace QtCharts;


class GraphWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();

    SeriesType Series_Type;


signals:
    void setPanel1Text(QString);
    void setPanel2Text(QString);
    void setPanel3Text(QString);
    void setPanel4Text(QString);
    void setPanel5Text(QString);
    void setPanel6Text(QString);


private slots:
    void onCheckBoxStateChanged(int arg1);

protected:
    Ui::GraphWindow *ui;
    std::list<GraphContainer> Series_List;

private:

    std::vector<GraphContainer> test;

    std::unique_ptr<QChartView> Chart_View;
    std::unique_ptr<QChart> Chart_;


    void setupGraph();
    bool addSeries(SeriesType::Type Type);
    void removeSeries(SeriesType::Type Type);
};

#endif // GRAPHWINDOW_H
