#include "simulatedperipheraltimer.h"

#include "portmacro.h"

SimulatedPeripheralTimer::SimulatedPeripheralTimer(QObject *parent) : QObject(parent)
{

}


void SimulatedPeripheralTimer::timerShot()
{
    prvSimulatedPeripheralTimer(); // TODO make this nicer
}
