// Processor Header Information ////////////////////////////////////////
/**
* @file processor.h
*
* @brief Header file for processor
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in processor
*
* @version 1.00
* C.S student (25 February 2018)
* Initial creation of processor
*
* @version 1.10
* C.S student (24 April 2018)
* Added interupt system and made necessary changes/inclusions for preemptive
* stuffs
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides the basic operations for processing metadata
  operations
*/
// Precompiler Directives //////////////////////////////////////////
//
#ifndef PROCESSOR_H
#define PROCESSOR_H
//
// Header Files ///////////////////////////////////////////////////
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>
#include "stringUtils.h"
#include "simTimer.h"
#include "memory.h"
#include "interupt.h"

//
// Global Constant Definitions ////////////////////////////////////
//

//// None

//
// Data Structure Definitions (structs, enums, etc.)////////////////
//
typedef enum
{
  NEW = 0,
  READY,
  BLOCKED,
  EXIT,
  RUNNING
} STATE;

typedef struct PCB
{
  int processNum;
  int processTime;
  MetaNode *startNode;
  struct PCB *nextProcess;
  STATE state;
} PCB;
//
// Function Prototypes ///////////////////////////////////////
//
StringNode *beginProcessing( MetaNode *currentNode, ConfigData *cfData );

int buildPcbList( PCB *currentPCB, MetaNode *currentNode, ConfigData *cfData );

void processAsFCFSN( PCB *currentProcess,
                     ConfigData *cfData,
                     StringNode *startNode,
                     MMU *memoryManagementUnit );

void processAsSJFN( PCB *currentProcess,
                    ConfigData *cfData,
                    StringNode *startNode,
                    MMU *memoryManagementUnit );

void processAsFCFSP( PCB *currentProcess,
                     ConfigData *cfData,
                     StringNode *startNode,
                     MMU *memoryManagementUnit );

void processAsSRTFP( PCB *currentProcess,
                     ConfigData *cfData,
                     StringNode *startNode,
                     MMU *memoryManagementUnit );

void processAsRRP( PCB *currentProcess,
                    ConfigData *cfData,
                    StringNode *startNode,
                    MMU *memoryManagementUnit );

int runProcess( PCB *currentProcess,
                ConfigData *cfData,
                StringNode *startNode,
                MMU *memoryManagementUnit );

int executeOp( MetaNode *currentNode,
                PCB *currentProcess,
                ConfigData *cfData,
                char *opPrint,
                StringNode *startNode );

int executeOpIO( MetaNode *currentNode,
                  PCB *currentProcess,
                  ConfigData *cfData,
                  char *opPrint,
                  StringNode *startNode );

int executeOpMem( MetaNode *currentNode,
                  PCB *currentProcess,
                  ConfigData *cfData,
                  StringNode *startNode,
                  MMU *memoryManagementUnit );

int getOpTime( MetaNode *currentNode, ConfigData *cfData );

void setState( ConfigData *cfData,
               StringNode *startNode,
               PCB *currentPCB,
               STATE state );

void setStateRecursive( PCB *currentPCB, STATE state );

void checkPcbCreation( ConfigData *cfData, StringNode *startNode, int pcbCode);

void displayProcess( PCB *currentPCB );

void logLine( ConfigData *cfData, StringNode *startNode, char* format, ... );

PCB *createPCB();

PCB *getFinalProcess(PCB *currentProcess);

Boolean checkState(PCB *currentProcess, STATE state);

PCB *getShortestProcess(PCB *currentProcess);

void freePcbList( PCB *startProcess );

void writeToLog( StringNode *startNode, char *filename );

Boolean checkPremptive(ConfigData *cfData);
//
// Terminating Precompiler Directives ///////////////////////////////
//
#endif // PROCESSOR_H
//
