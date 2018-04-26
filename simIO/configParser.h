// ConfigParser Header Information ////////////////////////////////////////
/**
* @file configParser.h
*
* @brief Header file for configParser
*
* @details Specifies constants, simple data structures, included libraries,
* and function declarations used in configParser
*
* @version 1.00
* C.S student (29 January 2018)
* Initial creation of configParser
*
* @note None
*/

// Program Description/Support /////////////////////////////////////
/*
  This file provides the functionality to read in a configuration file line by
  line and potentially store it in a configuration struct
*/
// Precompiler Directives //////////////////////////////////////////
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H
//
// Header Files ///////////////////////////////////////////////////
//
#include <stdlib.h>
#include <stdio.h>

#include "stringUtils.h"
#include "ioUtils.h"
#include "basicStructs.h"
//
// Global Constant Definitions ////////////////////////////////////
//
static const char CONFIG_START_LINE_EXPECTED[] = "Start Simulator Configuration File";
static const char CONFIG_END_LINE_EXPECTED[] = "End Simulator Configuration File.";

static const int VERSION_PATH_MIN = 0;
static const int VERSION_PATH_MAX = 10;
static const char VERSION_PATH_EXPECTED[] = "Version/Phase";

static const char FILE_PATH_EXPECTED[] = "File Path";

static const char CPU_SCHED_CODE_EXPECTED[] = "CPU Scheduling Code";

static const int QUANTUM_TIME_MIN = 0;
static const int QUANTUM_TIME_MAX = 100;
static const char QUANTUM_TIME_EXPECTED[] = "Quantum Time (cycles)";

static const int MEM_AVAILABLE_MIN = 0;
static const int MEM_AVAILABLE_MAX = 1048576;
static const char MEM_AVAILABLE_EXPECTED[] = "Memory Available (KB)";

static const int P_CYCLE_TIME_MIN = 0;
static const int P_CYCLE_TIME_MAX = 1000;
static const char P_CYCLE_TIME_EXPECTED[] = "Processor Cycle Time (msec)";

static const int IO_CYCLE_TIME_MIN = 0;
static const int IO_CYCLE_TIME_MAX = 100;
static const char IO_CYCLE_TIME_EXPECTED[] = "I/O Cycle Time (msec)";

static const char LOG_TO_EXPECTED[] = "Log To";

static const char LOG_FILE_PATH_EXPECTED[] = "Log File Path";

extern const char *cpuSchedCodeLookUp[];
//
// Data Structure Definitions (structs, enums, etc.)////////////////
//
typedef struct ConfigData
{
  int versionPhase;
  char *filePath;
  char *cpuSchedCode;
  int quantumTime;
  int memAvailable;
  int processorCycleTime;
  int ioCycleTime;
  char *logTo;
  char *logFilePath;
} ConfigData;
//
// Free Function Prototypes ///////////////////////////////////////
//
ConfigData *readConfig( FILE *filePointer );

ConfigData *createConfig();

Boolean freeConfig( ConfigData *cfData );

Boolean checkConfig( ConfigData *cfData );

Boolean displayConfig( ConfigData *config );

int getStartLine( FILE *filePointer, char delimiter );

int getIntLine( FILE *filePointer, const char *expected, int min, int max );

int extractInt( char *dataAsStr, int min, int max );

char *getStrLine( FILE *filePointer, const char *expected );

int getVersionPhase( FILE *filePointer );

char *getCpuSchedCode( FILE *filePointer );

Boolean cpuSchedCodeCheck( char *code );

char *getLogTo( FILE *filePointer );

Boolean logToCodeCheck( char *type );
//
// Terminating Precompiler Directives ///////////////////////////////
//
 #endif // CONFIG_PARSER_H
//
