// MetadataParser Header Information ////////////////////////////////////////
/**
* @file metaLinkedList.h
*
* @brief Header file for metaLinkedList
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in metaLinkedList
*
* @version 1.00
* C.S student (31 January 2018)
* Initial creation of metaLinkedList
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides the functionality to read in a metadata file line by
  line and potentially store it in a linked list of metadata nodes
*/
//

//
// Precompiler Directives //////////////////////////////////////////
//
#ifndef META_LINKED_LIST_H
#define META_LINKED_LIST_H
//
// Header Files ///////////////////////////////////////////////////
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stringUtils.h"
#include "ioUtils.h"
#include "basicStructs.h"
//
// Global Constant Definitions ////////////////////////////////////
//
extern const char *commandLookUp[];
//
// Data Structure Definitions (structs, enums, etc.)////////////////
//
typedef struct MetaNode
{
  char commandLetter;
  char *opString;
  int cycleTime;
  struct MetaNode *nextNode;
  struct MetaNode *previousNode;
} MetaNode;
//
// Free Function Prototypes ///////////////////////////////////////
//
Boolean displayNode( MetaNode *node );

Boolean displayNodeRecursive( MetaNode *node );

Boolean displayList( MetaNode *node );

MetaNode *extractNode( char *metaLine );

MetaNode *createNode( char commandLetter, char *opString, int cycleTime );

char extractCommandLetter( char *metaLine );

char *extractOpString( char *metaLine );

char *extractCycleTime( char *metaLine );

Boolean checkNode( char commandLetter, char *opString, char *cycleTime );

char extractCommandLetter( char *metaLine );

Boolean lookUp( const char *table[10], char *string, int tableStart, int tableEnd );

MetaNode *rewindList( MetaNode *node );

Boolean freeList( MetaNode *start );

Boolean freeNode( MetaNode *node );

MetaNode *addNode( MetaNode *head, MetaNode *node );

MetaNode *copyNode( MetaNode *node );

//
// Terminating Precompiler Directives ///////////////////////////////
//
#endif // META_LINKED_LIST_H
//
