#ifndef CONTAINERS_H
#define CONTAINERS_H



#include <QChart>
#include <QChartView>
#include <QLineSeries>
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



#define SUCCESS 0

#ifndef FAIL
#define FAIL    255
#endif


using namespace QtCharts;





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
        c1_2 = 5,       // 1 + (2*2)
        c1_3 = 7,       // 1 + (3*2)
        c1_4 = 9,       // 1 + (4*2)
        c2_3 = 8,       // 2 + (3*2)
        c2_4 = 10,      // 2 + (4*2)
        c3_4 = 11,      // 3 + (4*2)
        c1_2_3 = 14,    // 1 + (2*2) + (3*3)
        c1_2_4 = 17,    // 1 + (2*2) + (4*3)
        c1_3_4 = 19,    // 1 + (3*2) + (4*3)
        c2_3_4 = 20,    // 2 + (3*2) + (4*3)
        c1_2_3_4 = 30   // 1 + (2*2) + (3*3) +(4*4)

    };

    QStringList Type_Text = {"C1", "C2", "C3", "C4", "C1/2", "C1/3", "C1/4",
                             "C2/3", "C2/4", "C3/4", "C1/2/3", "C1/2/4",
                             "C1/3/4", "C2/3/4", "C1/2/3/4"};
    std::vector<QString> Colors_Html = {"#9fca43", "#f1a700", "#695dd7", "#b95a39", "#00fff4", "#ff00e2"};
    std::vector<QColor> Colors_ = {"#9fca43", "#f1a700", "#695dd7", "#b95a39", "#00fff4", "#ff00e2"};

    QColor getColor(int Index){return Colors_[Index];}
    QString getName(Type Type_){return Type_Text.at(Type_);}
    int getType(QString Std){ return Type_Text.indexOf(Std);}

};



struct GraphContainer
{

   SeriesType::Type Type_;
   PanelContainer Panel_;
   QColor Color_;
   QLineSeries *Series_;
   int Current_Value;

};



struct USBValueContainer
{

    uint32_t index;
    uint8_t lock;
    uint8_t channel;
    uint8_t pre_channel;
    uint32_t coarse;
    uint8_t fine;
    uint32_t pre_coarse;
    uint8_t pre_fine;
    uint32_t timestamp;
    uint32_t covergence_section;
};




#endif // CONTAINERS_H


