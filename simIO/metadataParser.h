// MetadataParser Header Information ////////////////////////////////////////
/**
* @file metadataParser.h
*
* @brief Header file for metedataParser
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in metadataParser
*
* @version 1.00
* C.S student (30 January 2018)
* Initial creation of metedataParser
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides the functionality to read in a metadata file line by
  line and potentially store it in a linked list of metadata nodes
*/
// Precompiler Directives //////////////////////////////////////////
//
#ifndef METADATA_PARSER_H
#define METADATA_PARSER_H
//
// Header Files ///////////////////////////////////////////////////
//
#include <stdlib.h>
#include <stdio.h>

#include "stringUtils.h"
#include "ioUtils.h"
#include "metaLinkedList.h"
//
// Global Constant Definitions ////////////////////////////////////
//
const static char META_START_LINE_EXPECTED[] = "Start Program Meta-Data Code:";
const static char META_END_LINE_EXPECTED[] = "End Program Meta-Data Code.";
//
// Data Structure Definitions (structs, enums, etc.)////////////////
//

//// None

//
// Free Function Prototypes ///////////////////////////////////////
//
struct MetaNode *readMetadata( FILE *filePointer );

int metaParseError( char *errorLine );
//
// Terminating Precompiler Directives ///////////////////////////////
//
#endif // METADATA_PARSER_H
//
