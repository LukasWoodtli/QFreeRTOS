#include "microcontrollerenvironment.h"

#include <QVector>
#include <QThread>
#include <QTimer>
#include <QtAlgorithms>
#include <QDebug>
#include <QCoreApplication>

#include "simulatedtask.h"


#include "task.h"
#include "portmacro.h"

#define UC_RESPONSIVENESS  (portTICK_PERIOD_MS / 10)

/* Pointer to the TCB of the currently executing task. */
extern void *pxCurrentTCB;


MicroControllerEnvironment::MicroControllerEnvironment(QObject *parent)
    : QObject(parent),
      m_pTaskList(new QVector<SimulatedTask*>(5)),
      m_irqFlags(0),
      m_processorRunning(false),
      m_taskTimer(new TaskTimer(*this))
{
    QThread *thread = new QThread();
    m_taskTimer->moveToThread(thread);
    thread->start(THREAD_SV_TIMER_PRIO);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
}



SimulatedTask* MicroControllerEnvironment::addTask(TaskFunction_t pxCode, void *pvParameters) {
    QThread* thread = new QThread();
    SimulatedTask * newTask = new SimulatedTask(pxCode, pvParameters);

    m_pTaskList->append(newTask);

    newTask->moveToThread(thread);

    qDebug() << "Add new simulated task: " << newTask->objectName();

    return newTask; // we save this on the FreeRTOS stack for the task
}

void MicroControllerEnvironment::AppendFreeRtosNameToLastTask(char const * const freeRtosTaskName)
{
    SimulatedTask * const lastTask = m_pTaskList->last();
    lastTask->setObjectName(lastTask->objectName().append(freeRtosTaskName));
    lastTask->thread()->setObjectName(QString("Thread of simulated FreeRTOS task: %1").arg(freeRtosTaskName));
}

void MicroControllerEnvironment::run()
{
        foreach(SimulatedTask* task, *m_pTaskList)
        {
         //   qDebug() << task->objectName();
        }
    
        /* Create a pending tick to ensure the first task is started as soon as
        this thread pends. */
        m_irqFlags.setFlag(InterruptFlags::TaskTimer);

        m_processorRunning = true;

        Q_ASSERT(m_taskTimer->thread()->isRunning());
        QMetaObject::invokeMethod(m_taskTimer, "startTimer", Qt::BlockingQueuedConnection);

        forever
        {
            QMutex interruptEventMutex;
            interruptEventMutex.lock();

            /* Used to indicate whether the simulated interrupt processing has
            necessitated a context switch to another task/thread. */
            bool freertosSchedulerSwitchRequired = false;

            /* For each interrupt we are interested in processing, each of which is
            represented by a bit in the 32bit ulPendingInterrupts variable. */
            for(int i = 0; i < m_irqFlags.numFlags(); i++ )
            {
                /* Is the simulated interrupt pending? */
                if( m_irqFlags.isFlagSet(InterruptFlags::Interrupt(1UL << i)))
                {
                    freertosSchedulerSwitchRequired = true;
                }
            }
            
            m_irqFlags.clear();

            if( freertosSchedulerSwitchRequired)
            {
                void *pvOldCurrentTCB;

                pvOldCurrentTCB = pxCurrentTCB;

                /* Select the next task to run. */
                vTaskSwitchContext();

                /* If the task selected to enter the running state is not the task
                that is already in the running state. */
                if( pvOldCurrentTCB != pxCurrentTCB )
                {
                    /* Suspend the old thread. */
                    SimulatedTask * oldTask = ( SimulatedTask *) *( ( size_t * ) pvOldCurrentTCB );

                    oldTask->thread()->setPriority(THREAD_TASK_IDLE_PRIO);

                    /* Obtain the state of the task now selected to enter the
                    Running state. */
                    SimulatedTask * newTask = (SimulatedTask* ) ( *( size_t *) pxCurrentTCB );
                    if (newTask->thread()->isRunning())
                    {
                        newTask->thread()->setPriority(THREAD_TASK_RUNNING_PRIO);
                    }
                    else
                    {
                         newTask->thread()->start(THREAD_TASK_RUNNING_PRIO);
                         QMetaObject::invokeMethod(newTask, "run", Qt::BlockingQueuedConnection);
                    }
                }
            }

            interruptEventMutex.unlock();
            QCoreApplication::processEvents();
            while(m_irqFlags.isZero())
            {
                QThread::currentThread()->msleep(UC_RESPONSIVENESS);
                QCoreApplication::processEvents();
            }
        }
}

void MicroControllerEnvironment::yield() {
    Q_ASSERT(m_processorRunning);
    m_irqFlags.setFlag(InterruptFlags::Yield);
    foreach(SimulatedTask *task, *m_pTaskList)
    {
        task->thread()->setPriority(THREAD_TASK_IDLE_PRIO);
        if (task->thread() == QThread::currentThread()) {
            QThread::currentThread()->msleep(UC_RESPONSIVENESS);
            
        }
    }
}



MicroControllerEnvironment::~MicroControllerEnvironment()
{
    // Delete all tasks
    qDeleteAll(m_pTaskList->begin(), m_pTaskList->end());
    // Remove tasks from list
    m_pTaskList->clear();

    // Remove list
    delete m_pTaskList;

}

