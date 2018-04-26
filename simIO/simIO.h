// simIO Header Information ////////////////////////////////////////
/**
* @file simIO.h
*
* @brief Header file for simIO
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in simulator input/output
*
* @version 1.00
* C.S student (25 February 2018)
* Initial creation of simIO
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides any the basic operations for processing metadata
  operations
*/
// Precompiler Directives //////////////////////////////////////////
//
#ifndef SIM_IO_H
#define SIM_IO_H
//
// Header Files ///////////////////////////////////////////////////
//
#include <stdio.h>
#include <stdlib.h>
#include "metadataParser.h"
#include "configParser.h"
#include "metaLinkedList.h"
//
// Global Constant Definitions ////////////////////////////////////
//

//// None

//
// Data Structure Definitions (structs, enums, etc.)////////////////
//

//// None

//
// Function Prototypes ///////////////////////////////////////
//
ConfigData *getConfig( char *fileName );

MetaNode *getMetadata( ConfigData *cfData );
//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // SIM_IO_H
//
