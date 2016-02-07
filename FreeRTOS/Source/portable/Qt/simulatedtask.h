#ifndef SIMULATEDTASK_H
#define SIMULATEDTASK_H

#include <QObject>
#include <QString>

#include "FreeRTOS.h"

class SimulatedTask : public QObject
{
    Q_OBJECT
    public:
        SimulatedTask(TaskFunction_t pxCode, void *pvParameters)
            :m_pxCode(pxCode),
             m_pvParameters(pvParameters)
        {setObjectName(QString("Simulated FreeRTOS task (%1): ").arg(s_totalNumThreads++));}

    public slots:
        void run() {
            m_pxCode(m_pvParameters);
        }


private:
    const TaskFunction_t m_pxCode;
    void * const m_pvParameters;

    static uint_least16_t s_totalNumThreads;
};

#endif // SIMULATEDTASK_H
