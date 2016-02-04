#ifndef MICROCONTROLLERENVIRONMENT_H
#define MICROCONTROLLERENVIRONMENT_H

#include <QObject>
#include <QVector>
#include <QMutex>

#include "simulatedtask.h"
#include "interruptflags.h"
#include "tasktimer.h"

class QTimer;

class MicroControllerEnvironment : public QObject
{
    Q_OBJECT
public:
    explicit MicroControllerEnvironment(QObject *parent = 0);
    ~MicroControllerEnvironment();

    SimulatedTask* addTask(TaskFunction_t pxCode, void *pvParameters);
    void AppendFreeRtosNameToLastTask(char const * const freeRtosTaskName);
    inline void setTimerInterruptFlag() {m_irqFlags.setFlag(InterruptFlags::TaskTimer);}

signals:

public slots:
    void run();
    void yield();


private:
    QVector<SimulatedTask*>  *m_pTaskList;

    TaskTimer                *m_taskTimer;

    InterruptFlags            m_irqFlags;
    bool                      m_processorRunning;
};

#endif // MICROCONTROLLERENVIRONMENT_H
