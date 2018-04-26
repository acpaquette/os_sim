// IoUtils Header Information ////////////////////////////////////////
/**
* @file ioUtils.h
*
* @brief Header file for ioUtils
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in ioUtils
*
* @version 1.00
* C.S student (30 January 2018)
* Initial creation of ioUtils
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides configParser and metadataParser with utilitey functions
  that can be used in either.
*/
// Precompiler Directives //////////////////////////////////////////
//
#ifndef IO_UTILS_H
#define IO_UTILS_H
//
// Header Files ///////////////////////////////////////////////////
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stringUtils.h"
#include "basicStructs.h"
//
// Global Constant Definitions ////////////////////////////////////
//
//// None
//
// Data Structure Definitions (structs, enums, etc.)////////////////
//
///// NONE
//
// Free Function Prototypes ///////////////////////////////////////
//
char *extractLine( FILE *filePointer );

Boolean checkExtractedLine( char *extractedLinePtr,
                        const char *expected,
                        char delimiter );

Boolean removeExpectedLine( FILE *filePointer, const char *expected, char delimiter );
//
// Terminating Precompiler Directives ///////////////////////////////
//
#endif // IO_UTILS_H
//
