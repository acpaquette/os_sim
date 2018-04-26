// Processor Implementation File Information ///////////////////////////////
/**
* @file processor.c
*
* @brief Implementation for processor
*
* @details Implements all functions for processing metadata files using
*          various scheduling algorithms
*
* @version 1.00
* C.S student (25 February 2018)
* Initial creation of processor
*
* @version 1.10
* C.S student (24 March 2018)
* Addition of SJF-N algorithm
*
* @version 1.20
* C.S student (24 April 2018)
* Added interupt system and made necessary changes/inclusions for preemptive
* stuffs
*
* @note Requires processor.h
*/

#include "processor.h"

//
// beginProcessing Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a MetaNode linked list based on data
 *        in a ConfigData struct.
 *
 * @details This function processes a MetaNode linked list and creates processes
 *          control blocks (PCB's) and then executes the operations in those
 *          PCB's using the data found in a ConfigData struct.
 *
 * @pre currentNode a pointer to the head of a MetaNode linked list
 *
 * @pre cfData a pointer to ConfigData struct
 *
 * @post currentNode and cfData remain unchanged at post
 *
 * @param[in] currentNode a pointer to a MetaNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @return the head of a StringNode linked list (StringNode *)
 */
StringNode *beginProcessing( MetaNode *currentNode, ConfigData *cfData )
{
    double time;
    int pcbCode;
    char *timeString = malloc( sizeof( char ) * 200 );

    StringNode *startNode = malloc( sizeof( *startNode ) );
    startNode->nextNode = NULL;
    startNode->string = NULL;

    time = accessTimer( ZERO_TIMER, timeString );
    logLine( cfData, startNode, "Time:%10.6lf, System start\n", time );

    time  = accessTimer( LAP_TIMER, timeString );
    logLine( cfData, startNode, "Time:%10.6lf, OS: Begin PCB Creation\n", time );

    PCB *currentProcess = createPCB();

    MMU *memoryManagementUnit = createMMU(cfData->memAvailable);

    pcbCode = buildPcbList( currentProcess, currentNode, cfData );
    checkPcbCreation( cfData, startNode, pcbCode );

    if( pcbCode != 0 )
    {
        logLine( cfData, startNode, "Unable to create PCB list. Exiting\n" );
        freePcbList( currentProcess );
        free( timeString );
        return startNode;
    }

    time = accessTimer( LAP_TIMER, timeString );
    logLine( cfData, startNode,
             "Time:%10.6lf, OS: All processes initialized in New state\n", time );

    setStateRecursive( currentProcess, READY );
    time = accessTimer( LAP_TIMER, timeString );
    logLine( cfData, startNode,
             "Time:%10.6lf, OS: All processes now set in Ready state\n", time);

    if( stringCmp( cfData->cpuSchedCode, "FCFS-N" ) == True
     || stringCmp( cfData->cpuSchedCode, "NONE" ) == True )
    {
        processAsFCFSN( currentProcess, cfData, startNode, memoryManagementUnit );
    }
    else if( stringCmp( cfData->cpuSchedCode, "SJF-N" ) == True )
    {
        processAsSJFN( currentProcess, cfData, startNode, memoryManagementUnit );
    }
    else if( stringCmp( cfData->cpuSchedCode, "FCFS-P" ) == True )
    {
        processAsFCFSP( currentProcess, cfData, startNode, memoryManagementUnit );
    }
    else if( stringCmp( cfData->cpuSchedCode, "SRTF-P" ) )
    {
        processAsSRTFP( currentProcess, cfData, startNode, memoryManagementUnit );
    }
    else if( stringCmp( cfData->cpuSchedCode, "RR-P" ) )
    {
        processAsRRP( currentProcess, cfData, startNode, memoryManagementUnit );
    }
    else
    {
        logLine( cfData,
                 startNode,
                 "Unimplemented processing schedule code: \n" );
        freePcbList( currentProcess );
        free( timeString );
        return startNode;
    }

    time = accessTimer( LAP_TIMER, timeString );
    logLine( cfData, startNode, "Time:%10.6lf, System stop\n", time );

    freePcbList( currentProcess );
    free( memoryManagementUnit );
    free( timeString );
    return startNode;
}
//
// buildPcbList Function Implementation ///////////////////////////////////
//

/**
 * @brief This function builds a PCB linked list given a MetaNode
 *
 * @details This function builds a PCB linked list by following a MetaNode
 *          linked list until it hits the S(start)end command where it finishs
 *          PCB creation. Ideally, the MetaNode pointer it is given at the
 *          begging is the head of the linked list.
 *
 * @pre currentPCB a pointer to the head of a PCB linked list
 *
 * @pre currentNode a pointer to the head of a MetaNode linked list
 *
 * @post currentNode and cfData remain unchanged at post
 *
 * @param[in] currentPCB a pointer to a PCB struct
 *
 * @param[in] currentNode a pointer to a MetaNode struct
 *
 * @return code for the operation exit status, 0 for success and anything
 *         else is some kind of failure (int)
 */
int buildPcbList( PCB *currentPCB, MetaNode *currentNode, ConfigData *cfData )
{
    int processNum = 0, processFlag = 0, processTime = 0;
    PCB *previous;

    // Bad start node
    if( currentNode->commandLetter != 'S'
     || stringCmp( currentNode->opString, "start" ) == False )
    {
        return 1;
    }

    currentNode = currentNode->nextNode;

    // Begin looping and creating PCBs
    do
    {
        if( currentNode == NULL )
        {
            return 2;
        }

        // Make sure we see an A
        if( currentNode->commandLetter == 'A' )
        {
            // Make sure we see a start
            if ( stringCmp( currentNode->opString, "start" ) == True )
            {
                if (processFlag == 0)
                {
                    currentPCB->processNum = processNum++;
                    // THIS ONE
                    currentPCB->startNode = currentNode->nextNode;
                    currentPCB->state = NEW;
                    processFlag = 1;
                }
                else
                {
                    return 3;
                }
            }
            // Make sure if we have seen a start we see and end
            else if ( stringCmp( currentNode->opString, "end" ) == True )
            {
                if ( processFlag == 1 )
                {
                    currentPCB->processTime = processTime;
                    PCB *newPCB = malloc( sizeof( *newPCB ) );
                    previous = currentPCB;
                    currentPCB->nextProcess = newPCB;
                    currentPCB = newPCB;
                    processFlag = 0;
                    processTime = 0;
                }
                else
                {
                    return 4;
                }
            }
        }
        else
        {
            processTime += getOpTime( currentNode, cfData );
        }

        currentNode = currentNode->nextNode;

    }

    while( currentNode->commandLetter != 'S'
        || stringCmp( currentNode->opString, "end" ) == False);

    previous->nextProcess = NULL;
    free( currentPCB );

    if( processFlag == 1 )
    {
        return 5;
    }

    return 0;
}
//
// processAsFCFSN Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a PCB linked list using a First Come First
 *        Serve algorithm.
 *
 * @details This function processes a PCB linked list using a First Come First
 *          Serve algorithm, where the first PCB to come in is the first PCB
 *          that is processed by the system.
 *
 * @pre currentProcess a pointer to the head of a PCB linked list
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @post startNode, cfData, and memoryManagementUnit remain unchanged at post
 *
 * @post currentProcess is a NULL pointer at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @return Nothing to return (void)
 */
void processAsFCFSN( PCB *currentProcess,
                     ConfigData *cfData,
                     StringNode *startNode,
                     MMU *memoryManagementUnit )
{
    double time = 0;
    int processesCode = 0;
    char *timeString = malloc( sizeof(char *) * 200 );

    while( currentProcess != NULL )
    {
        time = accessTimer( LAP_TIMER, timeString );
        logLine( cfData, startNode,
                 "Time:%10.6lf, OS: FCFS-N Strategy selects " \
                 "Process %d with time: %d mSec\n",
                 time, currentProcess->processNum, currentProcess->processTime  );

        setState( cfData, startNode, currentProcess, RUNNING );

        processesCode = runProcess( currentProcess,
                                    cfData,
                                    startNode,
                                    memoryManagementUnit );

        if( processesCode == 2 )
        {
            time = accessTimer( LAP_TIMER, timeString );
            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: Process %d, " \
                     "Segmentation Fault - Process ended\n",
                     time, currentProcess->processNum );
        }

        deallocate( memoryManagementUnit, currentProcess->processNum );

        setState( cfData, startNode, currentProcess, EXIT );

        currentProcess = currentProcess->nextProcess;
    }

    free( timeString );
}
//
// processAsSJFN Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a PCB linked list using a Shortest Job
 *        First Nonpreemptive algorithm.
 *
 * @details This function processes a PCB linked list using a Shortest Job
 *          First Nonpreemptive algorithm, where processes are executed from
 *          shortest to longest process based on total processing time.
 *
 * @pre currentProcess a pointer to the head of a PCB linked list
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @post startNode, cfData, and memoryManagementUnit remain unchanged at post
 *
 * @post currentProcess is a NULL pointer at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @return Nothing to return (void)
 */
void processAsSJFN( PCB *currentProcess,
                    ConfigData *cfData,
                    StringNode *startNode,
                    MMU *memoryManagementUnit )
{
    double time = 0;
    int processesCode = 0;
    char *timeString = malloc( sizeof(char *) * 200 );
    PCB *head = currentProcess, *selectedProcess = NULL;

    while( currentProcess != NULL )
    {
        selectedProcess = getShortestProcess(head);

        time = accessTimer( LAP_TIMER, timeString );
        logLine( cfData, startNode,
                 "Time:%10.6lf, OS: SJF-N Strategy selects " \
                 "Process %d with time: %d mSec\n",
                 time, selectedProcess->processNum, selectedProcess->processTime );

        setState( cfData, startNode, selectedProcess, RUNNING );

        processesCode = runProcess( selectedProcess,
                                    cfData,
                                    startNode,
                                    memoryManagementUnit );

        if( processesCode == 2 )
        {
            time = accessTimer( LAP_TIMER, timeString );
            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: Process %d, " \
                     "Segmentation Fault - Process ended\n",
                     time, selectedProcess->processNum );
        }

        deallocate( memoryManagementUnit, selectedProcess->processNum );

        setState( cfData, startNode, selectedProcess, EXIT );

        currentProcess = currentProcess->nextProcess;
    }

    free( timeString );
}
//
// processAsFCFSP Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a PCB linked list using a First Come First
 *        Serve Preemptive algorithm.
 *
 * @details This function processes a PCB linked list using a First Come First
 *          Serve Preemptive algorithm, where the first PCB to come in is the first PCB
 *          that is processed by the system and can be preempted.
 *
 * @pre currentProcess a pointer to the head of a PCB linked list
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @post startNode, cfData, and memoryManagementUnit remain unchanged at post
 *
 * @post currentProcess is a NULL pointer at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @return Nothing to return (void)
 */
void processAsFCFSP( PCB *currentProcess,
                     ConfigData *cfData,
                     StringNode *startNode,
                     MMU *memoryManagementUnit )
{
    double time = 0;
    int processesCode = 0;
    char *timeString = malloc( sizeof(char *) * 200 );
    Boolean allExit = False;

    PCB *head = currentProcess;
    PCB *tail = getFinalProcess(currentProcess);
    tail->nextProcess = head;

    while( allExit == False )
    {
        if( checkState(currentProcess, BLOCKED) )
        {
            time = accessTimer( LAP_TIMER, timeString );

            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: CPU Idle\n",
                     time );

            while(interuptQueueIterator < 0);

            resolveInterupts(cfData, startNode);
            currentProcess = head;
        }

        if( currentProcess->state == READY )
        {
            time = accessTimer( LAP_TIMER, timeString );
            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: FCFS-N Strategy selects " \
                     "Process %d with time: %d mSec\n",
                     time, currentProcess->processNum, currentProcess->processTime );

            setState( cfData, startNode, currentProcess, RUNNING );

            processesCode = runProcess( currentProcess,
                                        cfData,
                                        startNode,
                                        memoryManagementUnit );

            // Final metadata process was found. Exit the PCB
            if( processesCode == 0 )
            {
                deallocate( memoryManagementUnit, currentProcess->processNum );

                setState( cfData, startNode, currentProcess, EXIT );
            }
            // End quantum or end of run process came through.
            // Reset the head and keep going
            else if( processesCode == 1 )
            {
                setState( cfData, startNode, currentProcess, READY );

                currentProcess = head;
                continue;
            }
            // Memory segmentation fault happened. Handle appropriately
            else if( processesCode == 2 )
            {
                  time = accessTimer( LAP_TIMER, timeString );
                  logLine( cfData, startNode,
                           "Time:%10.6lf, OS: Process %d, " \
                           "Segmentation Fault - Process ended\n",
                           time, currentProcess->processNum );

                  setState( cfData, startNode, currentProcess, EXIT );
            }
            // System has interupts, handle them
            else if( processesCode == 3 )
            {
                  setState( cfData, startNode, currentProcess, READY );

                  resolveInterupts(cfData, startNode);
                  currentProcess = head;
                  continue;
            }
            // System sent out an IO thread, set state to blocked
            else if( processesCode == 4 )
            {
                  setState( cfData, startNode, currentProcess, BLOCKED );
            }
        }

        allExit = checkState(currentProcess, EXIT);
        currentProcess = currentProcess->nextProcess;
    }

    free( timeString );
}
//
// processAsSRTFP Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a PCB linked list using a Shortest Remaining
 *        Time First Preemptive algorithm.
 *
 * @details This function processes a PCB linked list using a Shortest Remaining
 *          Time First Preemptive algorithm, where the job to processes is the
 *          shortest job currently in the ready state.
 *
 * @pre currentProcess a pointer to the head of a PCB linked list
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @post startNode, cfData, and memoryManagementUnit remain unchanged at post
 *
 * @post currentProcess is a NULL pointer at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @return Nothing to return (void)
 */
void processAsSRTFP( PCB *currentProcess,
                     ConfigData *cfData,
                     StringNode *startNode,
                     MMU *memoryManagementUnit )
{
    double time = 0;
    int processesCode = 0;
    char *timeString = malloc( sizeof(char *) * 200 );
    Boolean allExit = False;

    PCB *head = currentProcess;
    PCB *tail = getFinalProcess(currentProcess);
    tail->nextProcess = head;

    while( allExit == False )
    {
        if( checkState(currentProcess, BLOCKED) )
        {
            time = accessTimer( LAP_TIMER, timeString );

            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: CPU Idle\n",
                     time );

            while(interuptQueueIterator < 0);

            resolveInterupts(cfData, startNode);
        }

        currentProcess = getShortestProcess(head);

        time = accessTimer( LAP_TIMER, timeString );
        logLine( cfData, startNode,
                 "Time:%10.6lf, OS: FCFS-N Strategy selects " \
                 "Process %d with time: %d mSec\n",
                 time, currentProcess->processNum, currentProcess->processTime );

        setState( cfData, startNode, currentProcess, RUNNING );

        processesCode = runProcess( currentProcess,
                                    cfData,
                                    startNode,
                                    memoryManagementUnit );

        // Final metadata process was found. Exit the PCB
        if( processesCode == 0 )
        {
            deallocate( memoryManagementUnit, currentProcess->processNum );

            setState( cfData, startNode, currentProcess, EXIT );
        }
        // End quantum or end of run process came through.
        // Reset the head and keep going
        else if( processesCode == 1 )
        {
            setState( cfData, startNode, currentProcess, READY );
        }
        // Memory segmentation fault happened. Handle appropriately
        else if( processesCode == 2 )
        {
            time = accessTimer( LAP_TIMER, timeString );
            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: Process %d, " \
                     "Segmentation Fault - Process ended\n",
                     time, currentProcess->processNum );

            setState( cfData, startNode, currentProcess, EXIT );
        }
        // System has interupts, handle them
        else if( processesCode == 3 )
        {
            setState( cfData, startNode, currentProcess, READY );

            resolveInterupts(cfData, startNode);
        }
        // System sent out an IO thread, set state to blocked
        else if( processesCode == 4 )
        {
            setState( cfData, startNode, currentProcess, BLOCKED );
        }

          allExit = checkState(currentProcess, EXIT);
    }

    free( timeString );
}

//
// processAsRRP Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a PCB linked list using a Round Robin
 *        Preemptive algorithm.
 *
 * @details This function processes a PCB linked list using a Round Robin
 *          Preemptive algorithm, where the system executes processes in order
 *          and only gives each process a specific quantum to processes then
 *          goes to the next process.
 *
 * @pre currentProcess a pointer to the head of a PCB linked list
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @post startNode, cfData, and memoryManagementUnit remain unchanged at post
 *
 * @post currentProcess is a NULL pointer at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @return Nothing to return (void)
 */
void processAsRRP( PCB *currentProcess,
                     ConfigData *cfData,
                     StringNode *startNode,
                     MMU *memoryManagementUnit )
{
    double time = 0;
    int processesCode = 0;
    char *timeString = malloc( sizeof(char *) * 200 );
    Boolean allExit = False;

    PCB *head = currentProcess;
    PCB *tail = getFinalProcess(currentProcess);
    tail->nextProcess = head;

    while( allExit == False )
    {
        if( checkState(currentProcess, BLOCKED) )
        {
            time = accessTimer( LAP_TIMER, timeString );

            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: CPU Idle\n",
                     time );

            while(interuptQueueIterator < 0);

            resolveInterupts(cfData, startNode);
            currentProcess = head;
        }

        if( currentProcess->state == READY )
        {
            time = accessTimer( LAP_TIMER, timeString );
            logLine( cfData, startNode,
                     "Time:%10.6lf, OS: FCFS-N Strategy selects " \
                     "Process %d with time: %d mSec\n",
                     time, currentProcess->processNum, currentProcess->processTime );

            setState( cfData, startNode, currentProcess, RUNNING );

            processesCode = runProcess( currentProcess,
                                        cfData,
                                        startNode,
                                        memoryManagementUnit );

            // Final metadata process was found. Exit the PCB
            if( processesCode == 0 )
            {
              deallocate( memoryManagementUnit, currentProcess->processNum );

              setState( cfData, startNode, currentProcess, EXIT );
            }
            // End quantum or end of run process came through.
            // Reset the head and keep going
            else if( processesCode == 1 )
            {
              setState( cfData, startNode, currentProcess, READY );
            }
            // Memory segmentation fault happened. Handle appropriately
            else if( processesCode == 2 )
            {
                time = accessTimer( LAP_TIMER, timeString );
                logLine( cfData, startNode,
                         "Time:%10.6lf, OS: Process %d, " \
                         "Segmentation Fault - Process ended\n",
                         time, currentProcess->processNum );

                setState( cfData, startNode, currentProcess, EXIT );
            }
            // System has interupts, handle them
            else if( processesCode == 3 )
            {
              setState( cfData, startNode, currentProcess, READY );

              resolveInterupts(cfData, startNode);
            }
            // System sent out an IO thread, set state to blocked
            else if( processesCode == 4 )
            {
              setState( cfData, startNode, currentProcess, BLOCKED );
            }
        }

        allExit = checkState(currentProcess, EXIT);
        currentProcess = currentProcess->nextProcess;
    }

  free( timeString );
}
//
// runProcess Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a PCB.
 *
 * @details This function processes a PCB from its A(start)0 command to its
 *          A(end)0 command running all operations inbetween unless the
 *          execution of a command returns something other than 0.
 *
 * @pre currentProcess a pointer to a PCB struct
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @post startNode, currentProcess, cfData, and memoryManagementUnit
 *       remain unchanged at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @return Code for process execution status (int)
 */
int runProcess( PCB *currentProcess,
                ConfigData *cfData,
                StringNode *startNode,
                MMU *memoryManagementUnit )
{
    char *opPrint = NULL;
    int executionCode = 0;
    MetaNode *currentNode = currentProcess->startNode;

    while ( currentNode->commandLetter != 'A'
         || stringCmp( currentNode->opString, "end" ) == False )
    {
        switch ( currentNode->commandLetter )
        {
          case 'I':
            opPrint = "input";
            executionCode = executeOpIO( currentNode, currentProcess, cfData,
                                         opPrint, startNode );
            break;

          case 'O':
            opPrint = "output";
            executionCode = executeOpIO( currentNode, currentProcess, cfData,
                                         opPrint, startNode );
            break;

          case 'P':
            opPrint = "Run operation";
            executionCode = executeOp( currentNode, currentProcess, cfData,
                                       opPrint, startNode );
            break;

          case 'M':
            executionCode = executeOpMem( currentNode,
                                          currentProcess,
                                          cfData,
                                          startNode,
                                          memoryManagementUnit );
            break;
        }

        if( executionCode == 4 )
        {
            currentProcess->startNode = currentNode->nextNode;
        }
        else if( executionCode == 1 )
        {
            currentProcess->startNode = currentNode;
        }

        if( executionCode != 0 )
        {
            return executionCode;
        }
        else if( executionCode == 0 && stringCmp(cfData->cpuSchedCode, "RR-P") )
        {
            currentProcess->startNode = currentNode->nextNode;
            return 1;
        }

        currentNode = currentNode->nextNode;

    }

    return executionCode;
}
//
// executeOp Function Implementation ///////////////////////////////////
//

/**
 * @brief This function executes a MetaNode struct
 *
 * @details This function executes all MetaNode structs that contain either
 *          memory allocation operations or process operations by running a timer
 *          for a number of miliseconds.
 *
 * @pre currentNode a pointer to the MetaNode struct being processed
 *
 * @pre currentProcess a pointer to a PCB struct
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre opPrint a pointer to a string
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @post currentNode, currentProcess, startNode, opPrint, and cfData
 *       remain unchanged at post
 *
 * @param[in] currentNode a pointer to a MetaNode struct
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] opPrint a pointer to a string
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @return Nothing to return (void)
 */
int executeOp( MetaNode *currentNode,
                PCB *currentProcess,
                ConfigData *cfData,
                char *opPrint,
                StringNode *startNode )
{
    int newCycleTime, runTime;
    float time;
    char *timeString = malloc( sizeof( char ) * 200 );

    time = accessTimer( LAP_TIMER, timeString );
    logLine( cfData, startNode,
             "Time:%10.6lf, Process %d, %s start\n",
             time, currentProcess->processNum, opPrint );

    if( checkPremptive( cfData ) )
    {
        newCycleTime  = currentNode->cycleTime - cfData->quantumTime;
        if( newCycleTime < 0 )
        {
            runTime = currentNode->cycleTime * cfData->processorCycleTime;
        }
        else
        {
            runTime = cfData->quantumTime * cfData->processorCycleTime;
        }

        if( interuptQueueIterator > -1 )
        {
            // return of 3 means there is an interupt in the queue
            return 3;
        }

        currentNode->cycleTime = newCycleTime;
        currentProcess->processTime -= runTime;
    }
    else
    {
        runTime = getOpTime( currentNode,  cfData );
        currentNode->cycleTime = 0;
    }

    runTimer( runTime );

    time = accessTimer( LAP_TIMER, timeString );

    if( currentNode->cycleTime > 0 )
    {
        logLine( cfData, startNode,
                 "Time:%10.6lf, Process %d, quantum time out\n",
                 time, currentProcess->processNum);

        return 1;
    }
    else
    {
        logLine( cfData, startNode,
                 "Time:%10.6lf, Process %d, %s end\n",
                 time, currentProcess->processNum, opPrint );
    }

    free( timeString );

    return 0;
}
//
// executeOpIO Function Implementation ///////////////////////////////////
//

/**
 * @brief This function executes a MetaNode struct
 *
 * @details This function executes all MetaNode structs that contain input or
 *          output operations by running a timer on a pthread for a number of
 *          milliseconds.
 *
 * @pre currentNode a pointer to the MetaNode struct being processed
 *
 * @pre currentProcess a pointer to a PCB struct
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre opPrint a pointer to a string
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @post currentNode, currentProcess, startNode, opPrint, and cfData
 *       remain unchanged at post
 *
 * @param[in] currentNode a pointer to a MetaNode struct
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] opPrint a pointer to a string
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @return Nothing to return (void)
 */
int executeOpIO( MetaNode *currentNode,
                  PCB *currentProcess,
                  ConfigData *cfData,
                  char *opPrint,
                  StringNode *startNode )
{
    float time;
    pthread_t thread;
    void *timeInMilliSecPtr;
    int rc1, timeInMilliSec;
    char *timeString = malloc( sizeof( char ) * 200 );

    time = accessTimer( LAP_TIMER, timeString );
    logLine( cfData, startNode,
             "Time:%10.6lf, Process %d, %s %s start\n",
             time, currentProcess->processNum, currentNode->opString, opPrint );

    timeInMilliSec = getOpTime( currentNode, cfData );
    timeInMilliSecPtr = &timeInMilliSec;

    if( checkPremptive(cfData) == False )
    {
        if( (rc1 = pthread_create( &thread, NULL, &runTimerIO, timeInMilliSecPtr)) )
        {
            logLine( cfData, startNode, "Thread creation failed\n" );
            return 1;
        }

        pthread_join( thread, NULL );

        time = accessTimer( LAP_TIMER, timeString );

        logLine( cfData, startNode,
                 "Time:%10.6lf, Process %d, %s %s end\n",
                 time, currentProcess->processNum, currentNode->opString, opPrint );

        free( timeString );

        return 0;
    }
    else
    {
        Interupt *interupt = createInterupt( currentProcess,
                                             timeInMilliSec,
                                             currentNode->opString,
                                             opPrint );

        pthread_create( &thread, NULL, &controllerThread, (void *)interupt);

        free( timeString );

        return 4;
    }
}
//
// executeOpMem Function Implementation ///////////////////////////////////
//

/**
 * @brief This function executes a MetaNode struct
 *
 * @details This function executes all MetaNode structs that contain memory
 *          operations by running a either an allocation or access operation
 *          using a memoryManagementUnit and a new memory segment
 *
 * @pre currentNode a pointer to the MetaNode struct being processed
 *
 * @pre currentProcess a pointer to a PCB struct
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @post currentNode, currentProcess, startNode, memoryManagementUnit,
 *       and cfData remain unchanged at post
 *
 * @param[in] currentNode a pointer to a MetaNode struct
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @return  Code for execution status (int)
 */
int executeOpMem( MetaNode *currentNode,
                  PCB *currentProcess,
                  ConfigData *cfData,
                  StringNode *startNode,
                  MMU *memoryManagementUnit )
{
  float time;
  int memoryOpCode = 0;
  char *timeString = malloc( sizeof( char ) * 200 );
  char *operation = NULL, *memoryOpStatus = "Success";
  Memory *memory = NULL;

  if( stringCmp(currentNode->opString, "access") == True )
  {
      operation = "MMU Access:";
  }
  else if( stringCmp(currentNode->opString, "allocate") == True )
  {
      operation = "MMU Allocation:";
  }

  memory = malloc( sizeof( *memory ) );
  createMemory(currentNode, memory, currentProcess->processNum);

  time = accessTimer( LAP_TIMER, timeString );
  logLine( cfData, startNode,
           "Time:%10.6lf, Process %d, %s %d/%d/%d\n",
           time, currentProcess->processNum, operation,
           memory->segment, memory->startPosition, memory->offset);

  if( stringCmp(currentNode->opString, "access") == True )
  {
      memoryOpCode = access( memoryManagementUnit, memory );
  }
  else if( stringCmp(currentNode->opString, "allocate") == True )
  {
      memoryOpCode = allocate( memoryManagementUnit, memory );
  }

  if( memoryOpCode != 0 )
  {
      memoryOpStatus = "Failed";
      free( memory );
  }
  else if( stringCmp(currentNode->opString, "access") == True )
  {
      free( memory );
  }

  time = accessTimer( LAP_TIMER, timeString );
  logLine( cfData, startNode,
           "Time:%10.6lf, Process %d, %s %s\n",
           time, currentProcess->processNum, operation, memoryOpStatus );

  free( timeString );

  return memoryOpCode;
}
//
// getOpTime Function Implementation ///////////////////////////////////
//

/**
 * @brief This function estimates the run time in milliseconds for a given
 *        MetaNode
 *
 * @details This function estimates the run time in milliseconds for a given MetaNode
 *          by multiplying the ioCycleTime by the MetaNodes cycleTime for I/O
 *          operations or by multiplying the processorCycleTime by the MetaNode
 *          cycleTime which is then returned
 *
 * @pre currentNode a pointer to the MetaNode struct being estimated
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @post currentNode, and cfData remain unchanged at post
 *
 * @param[in] currentNode a pointer to a MetaNode struct
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @return the estimated time of a MetaNode operation runtime in
 *         milliseconds (int)
 */
int getOpTime( MetaNode *currentNode, ConfigData *cfData )
{
    int runTime = 0;

    switch ( currentNode->commandLetter )
    {
      case 'I':
      case 'O':
        runTime = cfData->ioCycleTime * currentNode->cycleTime;
        break;

      case 'S':
      case 'A':
      case 'P':
        runTime = cfData->processorCycleTime * currentNode->cycleTime;
        break;

      case 'M':
        runTime = 0;
        break;
    }

    return runTime;
}
//
// setState Function Implementation ///////////////////////////////////
//

/**
 * @brief This function sets the state of a given PCB to the given state
 *
 * @details This function sets the state of a given PCB to the given state where
 *          the state is defined in the STATE enumeration
 *
 * @pre currentPCB a pointer to a PCB struct
 *
 * @pre state a state value defined in the STATE enum
 *
 * @post currentPCB has its state set to the state variable
 *
 * @post cfData remains unchanged at post
 *
 * @param[in] currentPCB a pointer to a PCB struct
 *
 * @param[in] state a state value
 *
 * @return  Nothing to return (void)
 */
void setState( ConfigData *cfData, StringNode *startNode, PCB *currentPCB, STATE state )
{
  currentPCB->state = state;
  char timeString[300], currentState[10];
  float time;

  time = accessTimer( LAP_TIMER, timeString );

  switch( currentPCB->state ) {
    case 0:
      sprintf(currentState, "New");
      break;

    case 1:
      sprintf(currentState, "Ready");
      break;

    case 2:
      sprintf(currentState, "Blocked");
      break;

    case 3:
      sprintf(currentState, "Exit");
      break;

    case 4:
      sprintf(currentState, "Running");
      break;
  }

  logLine( cfData, startNode,
           "Time:%10.6lf, OS: Process %d set in %s state\n",
           time, currentPCB->processNum, currentState);
}

//
// setStateRecursive Function Implementation ///////////////////////////////////
//

/**
 * @brief This function sets the state of a given PCB linked list to the given
 *        state recruvisely
 *
 * @details This function sets the state of a all PCBs in a linked list to the
 *          given state where the state is defined in the STATE enumeration
 *
 * @pre currentPCB a pointer to a PCB struct
 *
 * @pre state a state value defined in the STATE enum
 *
 * @post currentPCB has its state set to the state variable
 *
 * @post cfData remains unchanged at post
 *
 * @param[in] currentPCB a pointer to a PCB struct
 *
 * @param[in] state a state value
 *
 * @return  Nothing to return (void)
 */
void setStateRecursive( PCB *currentPCB, STATE state )
{
  while( currentPCB != NULL )
  {
    currentPCB->state = state;
    currentPCB = currentPCB->nextProcess;
  }
}
//
// checkPcbCreation Function Implementation ///////////////////////////////////
//

/**
 * @brief This function checks if the PCBs built correctly
 *
 * @details This function checks if the return from buildPcbList function resulted
 *          in an error or was successful
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to a StringNode struct
 *
 * @pre pcbCode the value of a buildPcbList return
 *
 * @post cfData, startNode, and pcbCode remain unchanged at post
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] pcbCode the value of a buildPcbList return
 *
 * @return  Nothing to return (void)
 */
void checkPcbCreation( ConfigData *cfData, StringNode *startNode, int pcbCode)
{
  switch ( pcbCode ) {
    case 0:
      break;

    case 1:
      logLine( cfData, startNode,
               "No S(start) command at the beginning of metadata.\n" );
      break;

    case 2:
      logLine( cfData, startNode,
               "Not S(end) command at the end of metadata.\n" );
      break;

    case 3:
      logLine( cfData, startNode,
               "Seen two A(start) commands without seeing an A(end).\n" );
      break;

    case 4:
      logLine(cfData, startNode, "Seen two A(end) commands \
                                  without seeing an A(start).\n");
      break;

    case 5:
      logLine( cfData, startNode,
               "Seen an A(start) command with no associated A(end).\n" );
      break;
  }
}
//
// displayProcess Function Implementation ///////////////////////////////////
//

/**
 * @brief This function displays a PCB struct
 *
 * @details This function displays a the process number, state, and the
 *          current MetaNode associated with a given PCB struct
 *
 * @pre currentPCB a pointer to a PCB struct
 *
 * @post currentPCB remains unchanged at post
 *
 * @param[in] currentPCB a pointer to a PCB struct
 *
 * @return Nothing to return (void)
 */
void displayProcess(PCB *currentPCB)
{
  if( currentPCB != NULL )
  {
    printf( "Process number: %d\n", currentPCB->processNum );

    printf( "Process time: %d\n", currentPCB->processTime );

    switch( currentPCB->state ) {
      case 0:
        printf( "Process state: NEW\n" );
        break;

      case 1:
        printf( "Process state: READY\n" );
        break;

      case 2:
        printf( "Process state: BLOCKED\n" );
        break;

      case 3:
        printf( "Process state: EXIT\n" );
        break;

      case 4:
        printf( "Process state: RUNNING\n" );
        break;
    }

    displayNode( currentPCB->startNode );
  }
  else
  {
    printf( "PROCESS IS NULL\n" );
  }
}
//
// logLine Function Implementation ///////////////////////////////////
//

/**
 * @brief This function logs a given string
 *
 * @details This function logs a given string to either a StringNode linked list
 *          to be logged to a file later, or prints directly to the monitor
 *          during process execution
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @pre startNode a pointer to the head of a StringNode linked list
 *
 * @pre format a pointer to a string with formating characters (%s, %d, etc.)
 *
 * @pre ... a list of variable arguments to use in the function
 *
 * @post startNode, format, ..., and cfData remain unchanged at post
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] format a pointer to a string with formating characters (%s, %d, etc.)
 *
 * @param[in] ... a list of variable arguments to use in the function
 *
 * @return Nothing to return (void)
 */
void logLine( ConfigData *cfData, StringNode *startNode, char* format, ... )
{
    char *outputBuffer = malloc(sizeof(char *) * 200);

    va_list argptr;
    va_start( argptr, format );
    vsprintf( outputBuffer, format, argptr );
    va_end( argptr );

    if( stringCmp( cfData->logTo, "Monitor" ) == True
     || stringCmp( cfData->logTo, "Both" ) == True )
    {
        printf( "%s", outputBuffer );
    }

    if( stringCmp( cfData->logTo, "File" ) == True
     || stringCmp( cfData->logTo, "Both" ) == True )
    {
        pthread_mutex_lock(&stringMutex);

        if( startNode->string == NULL )
        {
            startNode->string = outputBuffer;
            pthread_mutex_unlock(&stringMutex);
            return;
        }

        StringNode *newNode = malloc( sizeof( *newNode ) );
        newNode->string = outputBuffer;
        newNode->nextNode = NULL;
        addStringNode( startNode, newNode );

        pthread_mutex_unlock(&stringMutex);
    }
    else
    {
        free( outputBuffer );
    }
}
//
// createPCB Function Implementation ///////////////////////////////////
//

/**
 * @brief This function creates a new PCB
 *
 * @details This function allocates memory for a PCB, initializes all values
 *          and then return said PCB.
 *
 * @return new pointer to a process control block struct (PCB *)
 */
PCB *createPCB()
{
    PCB *newProcess = malloc( sizeof( *newProcess ) );
    newProcess->processNum = 0;
    newProcess->processTime = 0;
    newProcess->startNode = NULL;
    newProcess->nextProcess = NULL;
    newProcess->state = NEW;

    return newProcess;
}
//
// getFinalProcess Function Implementation ///////////////////////////////////
//

/**
 * @brief Gets the last process in the system.
 *
 * @details This function runs until the end of the list and returns the final
 *          PCB in the system.
 *
 * @pre currentProcess a pointer to a PCB struct
 *
 * @post currentProcess remains unchanged at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @return a pointer to a process control block (PCB *)
 */
PCB *getFinalProcess(PCB *currentProcess)
{
    while( currentProcess->nextProcess != NULL )
    {
        currentProcess = currentProcess->nextProcess;
    }

    return currentProcess;
}
//
// checkState Function Implementation ///////////////////////////////////
//

/**
 * @brief Checks if all processes in the system are in a specified state.
 *
 * @details This function checks if all processes are in a given state ignoring
 *          processes in the exit state unless looking to see if all processses
 *          are in said state.
 *
 * @pre currentProcess a pointer to a PCB struct
 *
 * @pre state a state defined in process.h
 *
 * @post currentProcess, and state remain unchanged at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @param[in] state a code that defines a given state
 *
 * @return True if all processes in state, False otherwise (Boolean)
 */
Boolean checkState( PCB *currentProcess, STATE state )
{
    int numLoops = 0, initialProcess = currentProcess->processNum;
    Boolean allExit = True;

    while( True )
    {
        if( currentProcess->processNum == initialProcess && numLoops > 0 )
        {
            return allExit;
        }

        if( currentProcess->state != state
         && currentProcess->state != EXIT )
        {
            allExit = False;
        }

        currentProcess = currentProcess->nextProcess;
        numLoops++;
    }
}
//
// getShortestProcess Function Implementation ///////////////////////////////////
//

/**
 * @brief Gets the shortest process in the system.
 *
 * @details This function runs until the end of the list and returns the shortest
 *          PCB in the system.
 *
 * @pre currentProcess a pointer to a PCB struct
 *
 * @post currentProcess remains unchanged at post
 *
 * @param[in] currentProcess a pointer to a PCB struct
 *
 * @return a pointer to a process control block (PCB *)
 */
PCB *getShortestProcess(PCB *currentProcess)
{
    int numLoops = 0, initialProcess = currentProcess->processNum;
    PCB *shortestProcess = NULL;

    while( True )
    {
        // End condition
        if( currentProcess == NULL )
        {
            return shortestProcess;
        }

        if( currentProcess->processNum == initialProcess
         && numLoops > 0 )
        {
            return shortestProcess;
        }

        if( shortestProcess == NULL && currentProcess->state == READY )
        {
            shortestProcess = currentProcess;
        }
        if( shortestProcess != NULL )
        {
            if( currentProcess->processTime < shortestProcess->processTime
             && currentProcess->state == READY)
            {
                shortestProcess = currentProcess;
            }
        }

        currentProcess = currentProcess->nextProcess;
        numLoops++;
    }
}
//
// freePcbList Function Implementation ///////////////////////////////////
//

/**
 * @brief This function deallocates a PCB linked list
 *
 * @details This function deallocates a PCB linked list by iterating
 *          over the nodes in the list and deallocating the appropriate fields
 *
 * @pre startProcess a pointer to a PCB struct. Ideally the head of the
 *                   linked list
 *
 * @post startProcess is now a NULL pointer
 *
 * @param[in] startProcess a pointer to a StringNode struct
 *
 * @return Nothing to return (void)
 */
void freePcbList( PCB *startProcess )
{
    int initialProcess = startProcess->processNum, numLoops = 0;

    PCB *currentProcess = startProcess;

    while( startProcess != NULL )
    {
        currentProcess = startProcess;
        startProcess = startProcess->nextProcess;

        // Handle preemptive processing
        if( initialProcess == currentProcess->processNum
         && numLoops >= 0 )
        {
            return;
        }

        numLoops++;
        free( currentProcess );
    }
}
//
// writeToLog Function Implementation ///////////////////////////////////
//

/**
 * @brief This function writes a StringNode struct linked list to a file
 *
 * @details This function iterates over a StringNode struct linked list writing
 *          each structs string value to a givem file
 *
 * @pre startNode a pointer to a StringNode struct. Ideally the head of the
 *                linked list
 *
 * @pre filename a pointer to a string
 *
 * @post startNode is now a NULL pointer
 *
 * @post filename remains unchanged
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @param[in] filename a pointer to a string
 *
 * @return Nothing to return (void)
 */
void writeToLog( StringNode *startNode, char *filename )
{
    FILE *logFilePointer;
    logFilePointer = fopen( filename, "w" );

    while( startNode != NULL )
    {
        fprintf( logFilePointer, "%s", startNode->string );
        startNode = startNode->nextNode;
    }

    fclose( logFilePointer );
}
//
// checkPremptive Function Implementation ///////////////////////////////////
//

/**
 * @brief This function processes a PCB linked list using a First Come First
 *        Serve algorithm.
 *
 * @details This function processes a PCB linked list using a First Come First
 *          Serve algorithm, where the first PCB to come in is the first PCB
 *          that is processed by the system.
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @post cfData remains unchanged at post
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @return True if the current setup is preemptive, False otherwise (Boolean)
 */
Boolean checkPremptive(ConfigData *cfData)
{
    if( stringCmp(cfData->cpuSchedCode, "SRTF-P")
     || stringCmp(cfData->cpuSchedCode, "FCFS-P")
     || stringCmp(cfData->cpuSchedCode, "RR-P"))
    {
        return True;
    }
    else
    {
        return False;
    }
}
