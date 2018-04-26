// StringUtils Header Information ////////////////////////////////////////
/**
* @file stringUtils.h
*
* @brief Header file for stringUtils
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in stringUtils
*
* @version 1.00
* C.S student (29 January 2018)
* Initial creation of stringUtils
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides any file with string utilitey functions
*/
// Precompiler Directives //////////////////////////////////////////
//
#ifndef STRING_UTILS_H
#define STRING_UTILS_H
//
// Header Files ///////////////////////////////////////////////////
//

#include <stdlib.h>
#include <stdio.h>
#include "basicStructs.h"

//
// Global Constant Definitions ////////////////////////////////////
//
extern const char NEW_LINE;
extern const char END_OF_FILE;
extern const char NULL_TERMINAL;
//
// Data Structure Definitions (structs, enums, etc.)////////////////
//
typedef struct StringNode
{
  char *string;
  struct StringNode *nextNode;
} StringNode;
//
// Free Function Prototypes ///////////////////////////////////////
//
Boolean stringCpy( char *originPtr, char *destinationPtr, char terminal );

int stringLen( char *string);

Boolean stringCmp( char *string1Ptr, char *string2Ptr );

Boolean cmpExpected( char *actualPtr, const char *expectedPtr );

char *stringResize( char *stringPtr, int sizeOfArray );

int stringToI( char *stringPtr );

float stringToF( char *stringPtr );

char *stringSplitBefore( char *originPtr, char delimiter );

char *stringSplitAfter( char *originPtr, char delimiter );

int readToDelimiter( char *stringPtr, char delimiter, int iterator );

Boolean checkInt( char *dataPtr );

void addStringNode( StringNode *head, StringNode *node );

void freeStringList( StringNode *startNode );
//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // STRING_UTILS_H
//
