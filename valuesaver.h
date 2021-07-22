#ifndef VALUESAVER_H
#define VALUESAVER_H

#include <QObject>
#include <QFile>
#include <QThread>
#include <queue>
#include "containers.h"

class ValueSaver : public QThread
{
    Q_OBJECT

    QFile *Save_File;
    std::queue<std::vector<USBValueContainer>> Log_Queue;
    std::condition_variable Condition_;
    std::mutex Lock_;

    void writeToFile(std::vector<USBValueContainer> &Values);

public:
    explicit ValueSaver(QObject *parent = nullptr);

    void saveValues(const std::vector<USBValueContainer> &Values);
    void openFile(const QString &FileDir);
    void closeFile();

    void run();

signals:

};

#endif // VALUESAVER_H
