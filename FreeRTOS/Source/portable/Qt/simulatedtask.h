#ifndef SIMULATEDTASK_H
#define SIMULATEDTASK_H

#include <QThread>
#include <QString>

#include "FreeRTOS.h"

class SimulatedTask : public QThread
{
    Q_OBJECT
    public:
        SimulatedTask(TaskFunction_t pxCode, void *pvParameters)
            :m_pxCode(pxCode),
             m_pvParameters(pvParameters),
             m_threadNo(s_totalNumThreads++)
        {}

        const QString toString() const {return QString("Thread no %1").arg(m_threadNo);}

    protected:
        void run() {
            m_pxCode(m_pvParameters);
        }


private:
    const TaskFunction_t m_pxCode;
    void * const m_pvParameters;

    uint_least16_t m_threadNo;
    static uint_least16_t s_totalNumThreads;
};

#endif // SIMULATEDTASK_H
