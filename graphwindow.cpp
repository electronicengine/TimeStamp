#include "graphwindow.h"
#include "QSplineSeries"
#include "QFrame"
#include "QMessageBox"
#include "ui_graphwindow.h"



GraphWindow::GraphWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    setupGraph();

    connect(ui->browse_file, SIGNAL(clicked(bool)), this, SLOT(onClickFileBrowserButton(bool)));
    connect(this, SIGNAL(setSaveDirectory(QString)), ui->save_directory, SLOT(setText(QString)));

    connect(ui->c1_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c2_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c3_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(ui->c1_2_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c1_3_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c1_4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c2_3_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c2_4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c3_4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c1_2_3_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c1_2_4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c1_3_4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c2_3_4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui->c1_2_3_4_checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));

    connect(ui->refresh_rate_box, SIGNAL(valueChanged(int)), this, SLOT(onRefreshCountChanged(int)));

    Saver_ = new ValueSaver(this);
}



GraphWindow::~GraphWindow()
{
    delete ui;
}



void GraphWindow::onCheckBoxStateChanged(int arg1)
{

    QCheckBox *check_box = (QCheckBox *)sender();

    SeriesType::Type type = (SeriesType::Type) Series_Type.getType(check_box->text());
    if(arg1 == 2)
    {
        bool ret = addSeries(type);
        if(!ret)
        {
            QMessageBox::warning(this, "Warning!", "You can select most 6 counter at the same time.");
            check_box->setChecked(false);
        }
    }
    else if(arg1 == 0)
        removeSeries(type);
}



void GraphWindow::onClickFileBrowserButton(bool)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"));

    emit setSaveDirectory(dir);
}



void GraphWindow::setupGraph()
{
    QLineSeries *default_series = new QLineSeries(this);

    Chart_ =  new QChart;
    Chart_View = new QChartView(Chart_, this);

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

    ui->groupBox_3->layout()->addWidget(Chart_View);

    Chart_->axes(Qt::Horizontal).back()->setRange(0, 100);
    Chart_->axes(Qt::Vertical).back()->setRange(0, 100);

}



void GraphWindow::onRefreshCountChanged(int Value)
{
    Time_Axis->deleteLater();

    Time_Axis = new QCategoryAxis(this);

    float time_interval = (float)(Value * 25) / 1000;


    Time_Axis->append(QString::number(time_interval * 4) + " s", 25);
    Time_Axis->append(QString::number(time_interval * 3) + " s", 50);
    Time_Axis->append(QString::number(time_interval * 2) + " s", 75);
    Time_Axis->append(QString::number(time_interval) + " s", 100);


    Time_Axis->setLinePenColor(QColor(114, 159, 207));
    Time_Axis->setLabelsColor(QColor(114, 159, 207));
    Time_Axis->setTitleBrush(QBrush(QColor(255, 255, 255)));
    Time_Axis->setTitleText("Time");
    Chart_->addAxis(Time_Axis, Qt::AlignBottom);

    foreach (GraphContainer container, Series_List)
    {
        container.Series_->attachAxis(Time_Axis);
    }

}



bool GraphWindow::addSeries(SeriesType::Type Type)
{

    Time_Axis = new QCategoryAxis(this);
    float time_interval = (float)(ui->refresh_rate_box->value() * 25) /1000;

    if(Series_List.size() > 5)
        return false;

    GraphContainer graph_container;

    graph_container.Series_ = new QLineSeries(this);
    graph_container.Color_ = Series_Type.getColor(Series_List.size());
    qDebug() << "Color: " << Series_List.size();

    graph_container.Type_ = Type;

    Chart_->addSeries(graph_container.Series_ );
    Chart_->legend()->hide();
    Chart_->createDefaultAxes();

    Chart_->createDefaultAxes();
    Chart_->axes(Qt::Horizontal).back()->setGridLineVisible(true);
    Chart_->axes(Qt::Horizontal).back()->setLabelsColor(QColor(114, 159, 207));
    Chart_->axes(Qt::Horizontal).back()->setTitleBrush(QBrush(QColor(255, 255, 255)));
    Chart_->axes(Qt::Horizontal).back()->setTitleText("Sample Number");

    Chart_->axes(Qt::Vertical).back()->setLabelsColor(QColor(114, 159, 207));
    Chart_->axes(Qt::Vertical).back()->setGridLineVisible(true);
    Chart_->axes(Qt::Vertical).back()->setTitleBrush(QBrush(QColor(255, 255, 255)));
    Chart_->axes(Qt::Vertical).back()->setTitleText("Channel Read Count");

    Chart_->setTitleBrush(QBrush(QColor(114, 159, 207)));
    Chart_->setBackgroundBrush(QBrush(QColor(46, 52, 54)));
    Chart_->setPlotAreaBackgroundBrush(QBrush(QColor(46, 52, 54)));
    Chart_->setAnimationOptions(QChart::NoAnimation);

    Chart_->axes(Qt::Horizontal).back()->setRange(0, 100);

    Time_Axis->append(QString::number(time_interval * 4) + " s", 25);
    Time_Axis->append(QString::number(time_interval * 3) + " s", 50);
    Time_Axis->append(QString::number(time_interval * 2) + " s", 75);
    Time_Axis->append(QString::number(time_interval) + " s", 100);

    Time_Axis->setLinePenColor(QColor(114, 159, 207));
    Time_Axis->setLabelsColor(QColor(114, 159, 207));
    Time_Axis->setTitleBrush(QBrush(QColor(255, 255, 255)));
    Time_Axis->setTitleText("Time");

    Chart_->addAxis(Time_Axis, Qt::AlignBottom);
    graph_container.Series_->attachAxis(Time_Axis);

    foreach (GraphContainer container, Series_List)
    {
        container.Series_->attachAxis(Time_Axis);
    }
    Chart_View->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    QLabel *counter_panel = new QLabel;
    QLabel *panel_label = new QLabel;

    counter_panel->setText("0");
    counter_panel->setFrameShape(QFrame::Panel);
    counter_panel->setAlignment(Qt::AlignCenter);
    counter_panel->setStyleSheet("color:" + Series_Type.Colors_Html[Series_List.size()] +  ";");

    QFont font;
    font.setPointSize(32);
    counter_panel->setFont(font);

    panel_label->setText(Series_Type.getName(graph_container.Type_));
    panel_label->setAlignment(Qt::AlignCenter);
    panel_label->setStyleSheet("color:" + Series_Type.Colors_Html[Series_List.size()] +  ";");

    vertical_layout->addWidget(counter_panel);
    vertical_layout->addWidget(panel_label);

    ui->graph_panel_layout->addLayout(vertical_layout);

    graph_container.Panel_.V_Layout = vertical_layout;
    graph_container.Panel_.Counter_Panel = counter_panel;
    graph_container.Panel_.Panel_Label = panel_label;

    for(int i=0; i<100; i++)
        graph_container.Series_->append(i, 1);

    Series_List.push_back(graph_container);


    return true;

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


