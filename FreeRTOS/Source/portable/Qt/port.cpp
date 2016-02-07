/*
    FreeRTOS V8.2.3 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Standard includes. */
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

/* Qt includes */
#include <QMutex>
#include <QThread>
#include <QDebug>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "simulatedtask.h"
#include "microcontrollerenvironment.h"



static QThread * mod_microControllerSimulationThread;
static MicroControllerEnvironment * mod_microControllerSimulation;

MicroControllerEnvironment * getMicroControllerEnvironment()
{
    if (!mod_microControllerSimulation)
    {
        mod_microControllerSimulationThread = new QThread();

        mod_microControllerSimulation = new MicroControllerEnvironment();
        mod_microControllerSimulation->moveToThread(mod_microControllerSimulationThread);
        mod_microControllerSimulationThread->start(THREAD_SV_TIMER_PRIO);
        QObject::connect(mod_microControllerSimulationThread, SIGNAL(finished()), mod_microControllerSimulationThread, SLOT(deleteLater()));
    }
    return mod_microControllerSimulation;
}




void vPortYield() {
    Q_ASSERT(mod_microControllerSimulation);
    mod_microControllerSimulation->yield();
}


#define portMAX_INTERRUPTS				( ( uint32_t ) sizeof( uint32_t ) * 8UL ) /* The number of bits in an uint32_t. */
#define portNO_CRITICAL_NESTING 		( ( uint32_t ) 0 )





void vPortAssert(bool cond, unsigned long ulLine, const char * const pcFileName )
{
    if (!cond)
    {
        qFatal("%s (%td)", pcFileName, ulLine);
    }
}


void portTraceTaskCreate(char const * const taskName)
{
    qDebug() << "Created FreeRTOS task: " << taskName;
    getMicroControllerEnvironment()->AppendFreeRtosNameToLastTask(taskName);
}



void portTraceTaskSwitchIn(char const * const taskName)
{
    qDebug() << "FreeRTOS task switch out: " << taskName;
}

void portTraceTaskSwitchOut(char const * const taskName)
{
    qDebug() << "Created FreeRTOS task switch in: " << taskName;
}


/*
 * Process all the simulated interrupts - each represented by a bit in
 * ulPendingInterrupts variable.
 */
static void prvProcessSimulatedInterrupts( void );

/*
 * Interrupt handlers used by the kernel itself.  These are executed from the
 * simulated interrupt handler thread.
 */
static uint32_t prvProcessYieldInterrupt( void );
static uint32_t prvProcessTickInterrupt( void );


/*
 * Called when the process exits to let Windows know the high timer resolution
 * is no longer required.
 */
static bool prvEndProcess( uint32_t dwCtrlType );


unsigned long ulGetRunTimeCounterValue( void )
{
    // TODO: QTimer
    return (unsigned long)(-1);
}

/*-----------------------------------------------------------*/




/* The WIN32 simulator runs each task in a thread.  The context switching is
managed by the threads, so the task stack does not have to be managed directly,
although the task stack is still used to hold an xThreadState structure this is
the only thing it will ever hold.  The structure indirectly maps the task handle
to a thread handle. */
typedef struct
{
	/* Handle of the thread that executes the task. */
    SimulatedTask *pvThread;

} xThreadState;

/* Mutex used to protect all the simulated interrupt variables that are accessed
by multiple threads. */
static QMutex *pvInterruptEventMutex = NULL;
//static void *pvInterruptEventMutex = NULL;

/* The critical nesting count for the currently executing task.  This is
initialised to a non-zero value so interrupts do not become enabled during
the initialisation phase.  As each task has its own critical nesting value
ulCriticalNesting will get set to zero when the first task runs.  This
initialisation is probably not critical in this simulated environment as the
simulated interrupt handlers do not get created until the FreeRTOS scheduler is
started anyway. */
static uint32_t ulCriticalNesting = 9999UL;


/* Pointer to the TCB of the currently executing task. */
extern void *pxCurrentTCB;

/* Used to ensure nothing is processed during the startup sequence. */
static BaseType_t xPortRunning = pdFALSE;

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

static bool prvEndProcess( uint32_t dwCtrlType )
{
	( void ) dwCtrlType;

    /* Match the call to timeBeginPeriod( xTimeCaps.wPeriodMin ) made when
    the process started with a timeEndPeriod() as the process exits. */
// TODO Use QTimer?    timeEndPeriod( 1 /*ms*/ );

	return pdPASS;
}
/*-----------------------------------------------------------*/

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
xThreadState *pxThreadState = NULL;
int8_t *pcTopOfStack = ( int8_t * ) pxTopOfStack;

	/* In this simulated case a stack is not initialised, but instead a thread
    is created that will execute the task being created.  The thread handles
	the context switching itself.  The xThreadState object is placed onto
	the stack that was created for the task - so the stack buffer is still
	used, just not in the conventional way.  It will not be used for anything
	other than holding this structure. */
	pxThreadState = ( xThreadState * ) ( pcTopOfStack - sizeof( xThreadState ) );

	/* Create the thread itself. */
    pxThreadState->pvThread = getMicroControllerEnvironment()->addTask(pxCode, pvParameters);
    configASSERT( pxThreadState->pvThread );


	return ( StackType_t * ) pxThreadState;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{

    /* Start the highest priority task by obtaining its associated thread
        state structure, in which is stored the thread handle. */
   // (( xThreadState * ) *( ( size_t * ) pxCurrentTCB ))->pvThread->thread()->setPriority(THREAD_TASK_RUNNING_PRIO);
 
    //ulCriticalNesting = portNO_CRITICAL_NESTING;

    /* Bump up the priority of the thread that is going to run, in the
        hope that this will assist in getting the Windows thread scheduler to
        behave as an embedded engineer might expect. */
    //pxThreadState->pvThread->start(THREAD_TASK_RUNNING_PRIO);

    getMicroControllerEnvironment()->run();
    
    forever;

	/* Would not expect to return from prvProcessSimulatedInterrupts(), so should
	not get here. */
	return 0;
}
/*-----------------------------------------------------------*/

static uint32_t prvProcessYieldInterrupt( void )
{
	return pdTRUE;
}
/*-----------------------------------------------------------*/

static uint32_t prvProcessTickInterrupt( void )
{
uint32_t ulSwitchRequired;

	/* Process the tick itself. */
	configASSERT( xPortRunning );
	ulSwitchRequired = ( uint32_t ) xTaskIncrementTick();

	return ulSwitchRequired;
}
/*-----------------------------------------------------------*/

static void prvProcessSimulatedInterrupts( void )
{
// moved to run() in micro proc. env.
}
/*-----------------------------------------------------------*/

void vPortDeleteThread( void *pvTaskToDelete )
{
xThreadState *pxThreadState;
uint32_t ulErrorCode;

	/* Remove compiler warnings if configASSERT() is not defined. */
	( void ) ulErrorCode;

	/* Find the handle of the thread being deleted. */
	pxThreadState = ( xThreadState * ) ( *( size_t *) pvTaskToDelete );

	/* Check that the thread is still valid, it might have been closed by
	vPortCloseRunningThread() - which will be the case if the task associated
	with the thread originally deleted itself rather than being deleted by a
	different task. */
	if( pxThreadState->pvThread != NULL )
	{
        pvInterruptEventMutex->lock();

        //QThread::setTerminationEnabled();
        //pxThreadState->pvThread->terminate();
        // not to wait? pxThreadState->pvThread->wait(1);

        delete pxThreadState->pvThread;
        pxThreadState->pvThread = NULL;

        pvInterruptEventMutex->unlock();
	}
}
/*-----------------------------------------------------------*/

void vPortCloseRunningThread( void *pvTaskToDelete, volatile BaseType_t *pxPendYield )
{
xThreadState *pxThreadState;
SimulatedTask *pvThread;
uint32_t ulErrorCode;

	/* Remove compiler warnings if configASSERT() is not defined. */
	( void ) ulErrorCode;

	/* Find the handle of the thread being deleted. */
	pxThreadState = ( xThreadState * ) ( *( size_t *) pvTaskToDelete );
	pvThread = pxThreadState->pvThread;

	/* Raise the Windows priority of the thread to ensure the FreeRTOS scheduler
	does not run and swap it out before it is closed.  If that were to happen
	the thread would never run again and effectively be a thread handle and
	memory leak. */

    //pvThread->setPriority(THREAD_SV_TIMER_PRIO);
	/* This function will not return, therefore a yield is set as pending to
	ensure a context switch occurs away from this thread on the next tick. */
	*pxPendYield = pdTRUE;

	/* Mark the thread associated with this task as invalid so
	vPortDeleteThread() does not try to terminate it. */
	pxThreadState->pvThread = NULL;

	/* Close the thread. */
    delete pvThread; // TODO ???

    QThread::currentThread()->exit(0);
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* This function IS NOT TESTED! */
// TODO Use QThread?	TerminateProcess( GetCurrentProcess(), 0 );
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

void vPortSetInterruptHandler( uint32_t ulInterruptNumber, uint32_t (*pvHandler)( void ) )
{
	if( ulInterruptNumber < portMAX_INTERRUPTS )
	{
		if( pvInterruptEventMutex != NULL )
		{
            pvInterruptEventMutex->lock();
//			ulIsrHandler[ ulInterruptNumber ] = pvHandler;
            pvInterruptEventMutex->unlock();
		}
		else
		{
    //		ulIsrHandler[ ulInterruptNumber ] = pvHandler;
		}
	}
}
/*-----------------------------------------------------------*/

void vPortEnterCritical( void )
{
	if( xPortRunning == pdTRUE )
	{
		/* The interrupt event mutex is held for the entire critical section,
		effectively disabling (simulated) interrupts. */
        pvInterruptEventMutex->lock();
		ulCriticalNesting++;
	}
	else
	{
		ulCriticalNesting++;
	}
}
/*-----------------------------------------------------------*/

void vPortExitCritical( void )
{
int32_t lMutexNeedsReleasing;

	/* The interrupt event mutex should already be held by this thread as it was
	obtained on entry to the critical section. */

	lMutexNeedsReleasing = pdTRUE;

	if( ulCriticalNesting > portNO_CRITICAL_NESTING )
	{
		if( ulCriticalNesting == ( portNO_CRITICAL_NESTING + 1 ) )
		{
			ulCriticalNesting--;

			/* Were any interrupts set to pending while interrupts were
			(simulated) disabled? */
//			if( ulPendingInterrupts != 0UL )
			{
				configASSERT( xPortRunning );

				/* Mutex will be released now, so does not require releasing
				on function exit. */
				lMutexNeedsReleasing = pdFALSE;
                pvInterruptEventMutex->unlock();
			}
		}
		else
		{
			/* Tick interrupts will still not be processed as the critical
			nesting depth will not be zero. */
			ulCriticalNesting--;
		}
	}

	if( pvInterruptEventMutex != NULL )
	{
		if( lMutexNeedsReleasing == pdTRUE )
		{
			configASSERT( xPortRunning );
            pvInterruptEventMutex->unlock();
		}
	}
}
/*-----------------------------------------------------------*/

