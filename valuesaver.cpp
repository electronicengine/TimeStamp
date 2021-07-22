#include "valuesaver.h"
#include "graphwindow.h"



ValueSaver::ValueSaver(QObject *parent) : QThread(parent)
{

}



void ValueSaver::saveValues(const std::vector<USBValueContainer> &Values)
{

    Log_Queue.push(Values);

    Condition_.notify_one();

}



void ValueSaver::openFile(const QString &FileDir)
{

    Save_File = new QFile(FileDir + "/TimeStamp_" + QString::number(time(NULL)) + ".log");
    Save_File->open(QIODevice::WriteOnly | QIODevice::Text);

}



void ValueSaver::closeFile()
{
    Save_File->close();
}



void ValueSaver::run()
{

    while(1)
    {

        std::unique_lock<std::mutex> ul(Lock_);

        Condition_.wait(ul, [=]{ return !Log_Queue.empty();});

        if(!Log_Queue.empty())
        {

            std::vector<USBValueContainer> &values = Log_Queue.front();
            Log_Queue.pop();
            writeToFile(values);

        }


    }

}



void ValueSaver::writeToFile(std::vector<USBValueContainer> &Values)
{

    if(Save_File->isOpen())
    {

        QTextStream out(Save_File);

        foreach (USBValueContainer container, Values)
            out << container.timestamp  << "\t" << container.channel << "\n";

    }


}

