#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <string>
#include <memory>
#include "QCategoryAxis"
#include "QVBoxLayout"
#include "QLabel"
#include "QFrame"
#include "QDebug"
#include "qdebug.h"

namespace Ui {
class GraphWindow;
}

using namespace QtCharts;


struct LineColors
{
    std::vector<QString> Colors_Html = {"#9fca43", "#f1a700", "#695dd7", "#b95a39", "#00fff4", "#ff00e2"};
    std::vector<QColor> Colors_ = {"#9fca43", "#f1a700", "#695dd7", "#b95a39", "#00fff4", "#ff00e2"};

    QColor getColor(int Index){return Colors_[Index];}

};



struct PanelContainer
{
    QVBoxLayout *V_Layout;
    QLabel *Counter_Panel;
    QLabel *Panel_Label;
};



struct SeriesType
{
    enum Type
    {
        c1,
        c2,
        c3,
        c4,
        c1_2,
        c1_3,
        c1_4,
        c2_3,
        c2_4,
        c3_4,
        c1_2_3,
        c1_2_4,
        c2_3_4,
        c1_2_3_4

    };

    std::vector<QString> Type_Text = {"C1", "C2", "C3", "C4", "C1/2", "C1/3", "C1/4",
                                    "C2/3", "C2/4", "C3/4", "C1/2/3", "C1/2/4", "C2/3/4", "C1/2/3/4"};

    QString getName(Type Type_){return Type_Text[Type_];}

};



struct GraphContainer
{

   SeriesType::Type Type_;
   PanelContainer Panel_;
   QColor Color_;
   QLineSeries *Series_;

};


class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow(QWidget *parent = nullptr);
    ~GraphWindow();


private:
    Ui::GraphWindow *ui;

    std::vector<GraphContainer> test;

    std::list<GraphContainer> Series_List;
    std::unique_ptr<QChartView> Chart_View;
    std::unique_ptr<QChart> Chart_;

    LineColors Line_Colors;
    SeriesType Series_Type;

    void setupGraph();
    void addSeries(SeriesType::Type Type);
    void removeSeries(SeriesType::Type Type);
};

#endif // GRAPHWINDOW_H
