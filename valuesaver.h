#ifndef VALUESAVER_H
#define VALUESAVER_H

#include <QObject>

class ValueSaver : public QObject
{
    Q_OBJECT
public:
    explicit ValueSaver(QObject *parent = nullptr);

signals:

};

#endif // VALUESAVER_H
