#ifndef TASKTIMER_H
#define TASKTIMER_H

#include <QObject>

class TaskTimer : public QObject
{
    Q_OBJECT
public:
    explicit TaskTimer(QObject *parent = 0);

signals:

public slots:
};

#endif // TASKTIMER_H
