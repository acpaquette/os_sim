// Interupt Implementation File Information ///////////////////////////////
/**
* @file interupt.c
*
* @brief Implementation for interupt
*
* @details Implements all functions for handling interupts created
*          by incoming I/O operations
*
* @version 1.00
* C.S student (22 April 2018)
* Initial creation of interupt
*
* @note Requires interupt.h
*/

#include "interupt.h"

Interupt interuptQueue[500];
int interuptQueueIterator = -1;

pthread_mutex_t interuptMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t controlMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t stringMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t timerMutex = PTHREAD_MUTEX_INITIALIZER;

//
// runTimerIOInterupt Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes an interupt.
 *
 * @details This function processes an interupt by running a timer for a
 *          specific amount of time, then updating the process and interupt
 *          queue.
 *
 * @pre interupt a void pointer to (theoretically) an interupt struct
 *
 * @post interupt remains unchanged at post
 *
 * @param[in] interupt a pointer to an interupt struct
 *
 * @return null for running on a thread (void *)
 */
void *runTimerIOInterupt( void* interupt )
{
    float clockTime;
    char timeString[300];
    int runTime;

    runTime = ((Interupt *)interupt)->runTime;

    runTimer(runTime);
    ((Interupt *)interupt)->process->processTime -= runTime;

    pthread_mutex_lock(&interuptMutex);
    clockTime = accessTimer(LAP_TIMER, timeString);

    ((Interupt *) interupt)->endTime = clockTime;

    interuptQueueIterator += 1;
    interuptQueue[interuptQueueIterator] = *(Interupt *)interupt;
    pthread_mutex_unlock(&interuptMutex);

    return NULL;
}
//
// controllerThread Function Implementation ///////////////////////////////////
//

/**
 * @brief This function starts a thread to process and interupt.
 *
 * @details This function sends out a runTimerIOInterupt on a thread for
 *          processing.
 *
 * @pre interupt a void pointer to (theoretically) an interupt struct
 *
 * @post interupt remains unchanged at post
 *
 * @param[in] interupt a pointer to an interupt struct
 *
 * @return null for running on a thread (void *)
 */
void *controllerThread(void *interupt)
{
    pthread_mutex_lock(&controlMutex);
    pthread_t contollerThread;
    // Do thread initialization cus I'm dumb
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int rc;

    if( (rc = pthread_create( &contollerThread, &attr, &runTimerIOInterupt, interupt )) )
    {
        printf( "Thread creation failed\n" );
        return NULL;
    }
    pthread_mutex_unlock(&controlMutex);

    return NULL;
}
//
// resolveInterupts Function Implementation ///////////////////////////////////
//

/**
 * @brief This function empties the interupt queue.
 *
 * @details This function processes the interupt queue and resolves all
 *          until the queue is empty interupts.
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to a StringNode struct
 *
 * @post cfData and startNode remain unchanged at post
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @return nothing to return (void)
 */
void resolveInterupts( ConfigData *cfData, StringNode *startNode )
{
    char timeString[300];
    float time;

    pthread_mutex_lock(&interuptMutex);

    time = accessTimer(LAP_TIMER, timeString);
    logLine( cfData, startNode,
             "Time:%10.6lf, OS: Handling Interupts\n",
             time);

    for(; interuptQueueIterator > -1; interuptQueueIterator--)
    {
        handleInterupt(interuptQueue[interuptQueueIterator], cfData, startNode);
    }

    pthread_mutex_unlock(&interuptMutex);
}
//
// handleInterupt Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes an interupt in the queue.
 *
 * @details This function processes an interupt by logging its interupt time,
 *          the time took to come back, and setting the processes its
 *          associated with back into the ready state.
 *
 * @pre interupt a pointer to an interupt struct
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to a StringNode struct
 *
 * @post interupt, cfData, and startNode remain unchanged at post
 *
 * @param[in] interupt a pointer to an interupt struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @return nothing to return (void)
 */
void handleInterupt(Interupt interupt, ConfigData *cfData, StringNode *startNode)
{
    StringNode *newNode = malloc( sizeof(*newNode) );
    char timeString[300];
    float time;

    time = accessTimer(LAP_TIMER, timeString);

    logLine( cfData, startNode,
             "Time:%10.6lf, OS: Interupt, Process %d\n",
             time, interupt.process->processNum );

    time = accessTimer(LAP_TIMER, timeString);

    logLine( cfData, startNode,
             "Time:%10.6lf, Process %d, %s %s end\n",
             time, interupt.process->processNum,
                   interupt.opString,
                   interupt.opPrint );

    setState( cfData, startNode, interupt.process, READY );
}
//
// createInterupt Function Implementation ///////////////////////////////////
//

/**
 * @brief This function creates an interupt struct.
 *
 * @details This function allocates memory for an interupt, initializes all values
 *          and then returns said interupt.
 *
 * @pre process a pointer to a PCB struct
 *
 * @pre runTime total runtime in milliseconds
 *
 * @pre opString pointer the operation string
 *
 * @pre opPrint pointer to the print string
 *
 * @post process, runTime, opString, and opPrint remain unchanged at post
 *
 * @param[in] process a pointer to a PCB struct
 *
 * @param[in] runTime total runtime in milliseconds
 *
 * @param[in] opString pointer the operation string
 *
 * @param[in] opPrint pointer to the print string
 *
 * @return the pointer to a newly created interupt struct (Interupt *)
 */
Interupt *createInterupt( PCB *process,
                          int runTime,
                          char *opString,
                          char *opPrint )
{
    Interupt *newInterupt = malloc( sizeof(*newInterupt) );

    newInterupt->process = process;
    newInterupt->runTime = runTime;
    newInterupt->endTime = -1;
    newInterupt->opString = opString;
    newInterupt->opPrint = opPrint;

    return newInterupt;
}
//
// displayInterupt Function Implementation ///////////////////////////////////
//

/**
 * @brief This function displays an interupt.
 *
 * @details This function prints all relevant Information stored in an interupt.
 *
 * @pre interupt an interupt struct
 *
 * @post interupt remains unchanged at post
 *
 * @param[in] interupt an interupt struct
 *
 * @return nothing to return (void)
 */
void displayInterupt(Interupt interupt)
{
    printf("ProcessId: %d\nRun Time: %d\nEnd Time: %f\n",
            interupt.process->processNum, interupt.runTime, interupt.endTime);
}
