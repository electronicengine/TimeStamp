#include "graphwindow.h"
#include "QSplineSeries"
#include "QFrame"
#include "ui_graphwindow.h"



GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    setupGraph();

    addSeries(SeriesType::c1);

//    removeSeries(SeriesType::c1);
}



GraphWindow::~GraphWindow()
{
    delete ui;
}



void GraphWindow::setupGraph()
{
    QLineSeries *default_series = new QLineSeries(this);

    Chart_ = std::make_unique<QChart>();

    Chart_View = std::make_unique<QChartView>(Chart_.get(), this);
    Chart_->addSeries(default_series);
    Chart_->legend()->hide();
    Chart_->createDefaultAxes();
    Chart_->axes(Qt::Horizontal).back()->setGridLineVisible(true);
    Chart_->axes(Qt::Horizontal).back()->setLabelsColor(QColor(114, 159, 207));

    Chart_->axes(Qt::Vertical).back()->setLabelsColor(QColor(114, 159, 207));
    Chart_->axes(Qt::Vertical).back()->setGridLineVisible(true);

    Chart_->setTitleBrush(QBrush(QColor(114, 159, 207)));
    Chart_->setBackgroundBrush(QBrush(QColor(46, 52, 54)));
    Chart_->setPlotAreaBackgroundBrush(QBrush(QColor(46, 52, 54)));
    Chart_->setAnimationOptions(QChart::AllAnimations);

    Chart_View->setRenderHint(QPainter::Antialiasing);

    ui->groupBox_3->layout()->addWidget(Chart_View.get());


    Chart_->axes(Qt::Horizontal).back()->setRange(0, 100);
    Chart_->axes(Qt::Vertical).back()->setRange(0, 100);

}



void GraphWindow::addSeries(SeriesType::Type Type)
{

    GraphContainer graph_container;

    graph_container.Series_ = new QLineSeries;
    graph_container.Color_ = Line_Colors.getColor(Series_List.size());
    graph_container.Type_ = Type;

    Chart_->addSeries(graph_container.Series_ );

    for(int i = 0; i<100; i++)
        graph_container.Series_->append(i*(Series_List.size()+1), i);

    QPen pen = graph_container.Series_->pen();
    pen.setBrush(QBrush(graph_container.Color_));
    graph_container.Series_->setPen(pen);

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    QLabel *counter_panel = new QLabel;
    QLabel *panel_label = new QLabel;

    counter_panel->setText("0");
    counter_panel->setFrameShape(QFrame::Panel);
    counter_panel->setAlignment(Qt::AlignCenter);
    counter_panel->setStyleSheet("color:" + Line_Colors.Colors_Html[Series_List.size()] +  ";");

    QFont font;
    font.setPointSize(32);
    counter_panel->setFont(font);

    panel_label->setText(Series_Type.getName(graph_container.Type_));
    panel_label->setAlignment(Qt::AlignCenter);
    panel_label->setStyleSheet("color:" + Line_Colors.Colors_Html[Series_List.size()] +  ";");

    vertical_layout->addWidget(counter_panel);
    vertical_layout->addWidget(panel_label);

    ui->graph_panel_layout->addLayout(vertical_layout);

    graph_container.Panel_.V_Layout = vertical_layout;
    graph_container.Panel_.Counter_Panel = counter_panel;
    graph_container.Panel_.Panel_Label = panel_label;

    Series_List.push_back(graph_container);



}



void GraphWindow::removeSeries(SeriesType::Type Type)
{


    for (std::list<GraphContainer>::iterator it=Series_List.begin(); it!=Series_List.end(); ++it)
    {
        if(it->Type_ == Type)
        {

            it->Panel_.Counter_Panel->deleteLater();
            it->Panel_.Panel_Label->deleteLater();
            it->Panel_.V_Layout->deleteLater();
            it->Series_->deleteLater();


            Series_List.erase(it++);


        }
    }

}





