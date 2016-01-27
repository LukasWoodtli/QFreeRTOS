#ifndef SIMULATEDPERIPHERALTIMER_H
#define SIMULATEDPERIPHERALTIMER_H

#include <QThread>
#include <functional>

class QTimer;
class QMutex;
class QWaitCondition;

typedef void (*timerInterruptHandler_t)(void);

class SimulatedPeripheralTimer : public QThread
{
    Q_OBJECT
public:
    SimulatedPeripheralTimer(timerInterruptHandler_t function,
                             QObject *parent = 0);
    void startTimer();

protected slots:
    void timerShot();

protected:
    virtual void run();


private:
    QTimer * const m_intervallTimer;

    timerInterruptHandler_t m_function;
};

#endif // SIMULATEDPERIPHERALTIMER_H
