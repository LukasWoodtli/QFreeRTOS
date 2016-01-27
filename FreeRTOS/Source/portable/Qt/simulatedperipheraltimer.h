#ifndef SIMULATEDPERIPHERALTIMER_H
#define SIMULATEDPERIPHERALTIMER_H

#include <QObject>

class SimulatedPeripheralTimer : public QObject
{
    Q_OBJECT
public:
    explicit SimulatedPeripheralTimer(QObject *parent = 0);

public slots:
    void timerShot();

signals:

public slots:
};

#endif // SIMULATEDPERIPHERALTIMER_H
