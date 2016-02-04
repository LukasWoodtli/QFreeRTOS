#include "tasktimer.h"

#include <QThread>

#include "microcontrollerenvironment.h"

TaskTimer::TaskTimer(MicroControllerEnvironment  &mc)
    : m_intervallTimer(new QTimer(this)),
      m_mC(mc)
{
    const bool connected = connect(m_intervallTimer, SIGNAL(timeout()), SLOT(timerFinished()));
    Q_ASSERT(connected);

    m_intervallTimer->setInterval(static_cast<int>(portTICK_PERIOD_MS));
    m_intervallTimer->setSingleShot(false);
    m_intervallTimer->setTimerType(Qt::PreciseTimer);
}

void TaskTimer::startTimer()
{
    m_intervallTimer->start();
}

void TaskTimer::timerFinished()
{
    //Q_ASSERT(m_processorRunning);
    /* The timer has expired, generate the simulated tick event. */
    m_mC.setTimerInterruptFlag();
}
