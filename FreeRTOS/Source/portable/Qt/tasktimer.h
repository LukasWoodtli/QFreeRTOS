#ifndef TASKTIMER_H
#define TASKTIMER_H

#include <QObject>
#include <QTimer>

class MicroControllerEnvironment ;

class TaskTimer : public QObject
{
    Q_OBJECT
public:
    explicit TaskTimer(MicroControllerEnvironment  &mc);

    inline bool isRunning() const {return m_intervallTimer->isActive();}

signals:

public slots:
    void startTimer();
    void timerFinished();

private:
    QTimer * const            m_intervallTimer;
    MicroControllerEnvironment &m_mC;
};

#endif // TASKTIMER_H
