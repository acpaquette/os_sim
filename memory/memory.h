// Memory Header Information ////////////////////////////////////////
/**
* @file memory.h
*
* @brief Header file for memory
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in memory
*
* @version 1.00
* C.S student (26 March 2018)
* Initial creation of memory
*
* @note None
*/

// Memory Description/Support /////////////////////////////////////
/*
  This file provides the basic operations for allocating, accessing
  and deallocating memory.
*/
// Precompiler Directives //////////////////////////////////////////
//
#ifndef MEMORY_H
#define MEMORY_H
//
// Header Files ///////////////////////////////////////////////////
//

#include <stdio.h>
#include <stdlib.h>
#include "metaLinkedList.h"

//
// Global Constant Definitions ////////////////////////////////////
//

//// None

//
// Data Structure Definitions (structs, enums, etc.)////////////////
//
typedef struct Memory
{
  int startPosition;
  int offset;
  int segment;
  int totalSize;
  int processesId;
  struct Memory *nextNode;
} Memory;

typedef struct MMU
{
  int totalMemory;
  int memAvailable;
  struct Memory *headMemory;
} MMU;
//
// Function Prototypes ///////////////////////////////////////
//
MMU *createMMU( int memSize );

void createMemory( MetaNode *memoryNode, Memory *memory, int processesId );

int access( MMU *memoryManagementUnit, Memory *memoryNode );

int allocate( MMU *memoryManagementUnit, Memory *memoryNode );

void deallocate( MMU *memoryManagementUnit, int processesId );

Boolean checkOverlap( Memory *headMemory, Memory *memoryNode );

void displayMemory( Memory *memoryNode );

void displayMemoryBlock( MMU *memoryManagementUnit );
//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // MEMORY_H
//
