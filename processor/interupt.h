// Interupt Header Information ////////////////////////////////////////
/**
* @file interupt.h
*
* @brief Header file for interupt
*
* @details Specifies constants, simple data structures, included libraries,
*          and function declarations used in handling interupts
*
* @version 1.00
* C.S student (22 April 2018)
* Initial creation of interupt
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides the basic operations for handling interupts created
  by incoming I/O operations
*/
// Precompiler Directives //////////////////////////////////////////
//
#ifndef INTERUPT_H
#define INTERUPT_H
//
// Header Files ///////////////////////////////////////////////////
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "simIO.h"
#include "processor.h"
#include "stringUtils.h"
#include "simTimer.h"
#include "basicStructs.h"
//
// Data Structure Definitions (structs, enums, etc.)////////////////
//
typedef struct Interupt
{
  struct PCB *process;
  int runTime;
  float endTime;
  char *opString;
  char *opPrint;
} Interupt;

//
// Global Constant Definitions ////////////////////////////////////
//

extern Interupt interuptQueue[500];
extern int interuptQueueIterator;
extern pthread_mutex_t interuptMutex;
extern pthread_mutex_t controlMutex;
extern pthread_mutex_t stringMutex;
extern pthread_mutex_t timerMutex;
//
// Function Prototypes ///////////////////////////////////////
//
void *runTimerIOInterupt(void* interupt);

void *controllerThread(void *interupt);

void resolveInterupts(ConfigData *cfData, StringNode *startNode);

void handleInterupt(Interupt interupt, ConfigData *cfData, StringNode *startNode);

Interupt *createInterupt( struct PCB *processId,
                          int runTime,
                          char *opString,
                          char *opPrint );

void displayInterupt(Interupt interupt);
//
// Terminating Precompiler Directives ///////////////////////////////
//
#endif // INTERUPT_H
//
