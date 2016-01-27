#include "simulatedperipheraltimer.h"

#include <QTimer>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>

#include <functional>

#include "portmacro.h"
#include "FreeRTOSConfig.h"

SimulatedPeripheralTimer::SimulatedPeripheralTimer(timerInterruptHandler_t function,
                                                   QObject *parent)
    : QThread(parent),
      m_intervallTimer(new QTimer(this)),
      m_function(function)
{

}

void SimulatedPeripheralTimer::run()
{
    forever {/* don't do anything, just wait for timer fireing */ }
}

void SimulatedPeripheralTimer::startTimer()
{
    QObject::connect(m_intervallTimer, SIGNAL(timeout()), this, SLOT(timerShot()));

    start(THREAD_SV_TIMER_PRIO);

    m_intervallTimer->setInterval(static_cast<int>(portTICK_PERIOD_MS));
    m_intervallTimer->setTimerType(Qt::PreciseTimer);

    m_intervallTimer->start();
}


void SimulatedPeripheralTimer::timerShot()
{
    m_function();

}
