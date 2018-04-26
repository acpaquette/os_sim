// ConfigParser Implementation File Information ///////////////////////////////
/**
* @file configParser.c
*
* @brief Implementation for configParser
*
* @details Implements all functions for parsing a config file
*
* @version 1.00
* C.S student (29 January 2018)
* Initial creation of configParser
*
* @note Requires configParser.h
*/

#include "configParser.h"

const char *cpuSchedCodeLookUp[] =
{
    [0] = "NONE",
    [1] = "FCFS-N",
    [2] = "SJF-N",
    [3] = "SRTF-P",
    [4] = "FCFS-P",
    [5] = "RR-P"
};

//
// readConfig Function Implementation ///////////////////////////////////
//

/**
 * @brief This function reads in a configuration file and parses it.
 *
 * @details This function reads in and checks all elements of a configuration
 *          storing each element in a configuration struct (ConfigData).
 *
 * @pre filePointer a pointer to a file
 *
 * @post filePointer now points to the end of the file.
 *
 * @par Algorithm
 *      The algorithm tries to extract each element of a configuration struct.
 *      Going line by line then checking at the end if the file was correctly
 *      parsed.
 *
 * @exception invalidConfig
 *            On final check, if any element of the configuration file is bad,
 *            return a bad configuration struct.
 *
 * @param[in] filePointer a pointer to a file
 *
 * @return a newly constructed configuration struct (struct ConfigData *)
 */
struct ConfigData *readConfig( FILE *filePointer )
{
    Boolean validStart, validEnd;
    ConfigData *cfData = NULL;
    cfData = createConfig( );

    validStart = removeExpectedLine( filePointer,
                                     CONFIG_START_LINE_EXPECTED,
                                     NEW_LINE );

    cfData->versionPhase = getIntLine( filePointer,
                                       VERSION_PATH_EXPECTED,
                                       VERSION_PATH_MIN,
                                       VERSION_PATH_MAX );

    cfData->filePath = getStrLine( filePointer,
                                   FILE_PATH_EXPECTED );

    cfData->cpuSchedCode = getCpuSchedCode( filePointer );

    cfData->quantumTime = getIntLine( filePointer,
                                      QUANTUM_TIME_EXPECTED,
                                      QUANTUM_TIME_MIN,
                                      QUANTUM_TIME_MAX );

    cfData->memAvailable = getIntLine( filePointer,
                                       MEM_AVAILABLE_EXPECTED,
                                       MEM_AVAILABLE_MIN,
                                       MEM_AVAILABLE_MAX );

    cfData->processorCycleTime = getIntLine( filePointer,
                                             P_CYCLE_TIME_EXPECTED,
                                             P_CYCLE_TIME_MIN,
                                             P_CYCLE_TIME_MAX );

    cfData->ioCycleTime = getIntLine( filePointer,
                                      IO_CYCLE_TIME_EXPECTED,
                                      IO_CYCLE_TIME_MIN,
                                      IO_CYCLE_TIME_MAX );

    cfData->logTo = getLogTo( filePointer );

    cfData->logFilePath = getStrLine( filePointer,
                                      LOG_FILE_PATH_EXPECTED );

    validEnd = removeExpectedLine( filePointer,
                                   CONFIG_END_LINE_EXPECTED,
                                   NEW_LINE );

    if ( checkConfig( cfData ) == False
        || validStart == False
        || validEnd == False )
    {
        printf( "Corrupt Config File.\n" );
        freeConfig( cfData );

        return NULL;
    }

    return cfData;
}
//
// createConfig Function Implementation ///////////////////////////////////
//

/**
 * @brief This function creates a ConfigStruct
 *
 * @details This function creates a bad configuration struct. Making it
 *          useless until all fields are correctly set.
 *
 * @par Algorithm
 *      Initializes a configuration struct with all bad values for each field.
 *
 * @return a newly constructed bad configuration struct (struct ConfigData *)
 */
ConfigData *createConfig()
{
  ConfigData *cfData = malloc( sizeof( *cfData ) );
  cfData->versionPhase = -1;
  cfData->filePath = "BAD";
  cfData->cpuSchedCode = "BAD";
  cfData->quantumTime = -1;
  cfData->memAvailable = -1;
  cfData->processorCycleTime = -1;
  cfData->ioCycleTime = -1;
  cfData->logTo = "BAD";
  cfData->logFilePath = "BAD";
  return cfData;
}
//
// freeConfig Function Implementation ///////////////////////////////////
//

/**
 * @brief This function frees a configuration struct
 *
 * @details This function checks if the given struct is null, if not, it
 *          frees the configuration struct and all of its assocaited elements.
 *
 * @pre cfData a ConfigData pointer
 *
 * @post cfData is a null pointer
 *
 * @par Algorithm
 *      The algorithm checks each of the necessary fields and frees them if
 *       necessary.
 *
 * @exception nullPointer
 *            If the pointer to the ConfigData is null, immediately return -1.
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @return success of method operation (bool)
 *
 * @note Function provides three output quantities
 */
Boolean freeConfig( ConfigData *cfData )
{
    if( cfData == NULL )
    {
        return False;
    }

    if( stringCmp( cfData->filePath, "BAD" ) == False )
    {
        free( cfData->filePath );
    }

    if ( stringCmp( cfData->cpuSchedCode, "BAD" ) == False )
    {
        free( cfData->cpuSchedCode );
    }

    if( stringCmp( cfData->logTo, "BAD" ) == False )
    {
        free( cfData->logTo );
    }

    if( stringCmp( cfData->logFilePath, "BAD" ) == False )
    {
        free( cfData->logFilePath );
    }

    free( cfData );

    return True;
}
//
// checkConfig Function Implementation ///////////////////////////////////
//

/**
 * @brief This function checks a config for bad values
 *
 * @details This function checks if the given struct is null, if not, it
 *          checks the assocaited elements of a configuration struct.
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @post cfData is unchanged at post
 *
 * @exception nullPointer
 *            If the given ConfigData pointer is null, immediately return -1.
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @return success of method operation (bool)
 */
Boolean checkConfig( ConfigData *cfData )
{
  if( cfData == NULL )
  {
      return False;
  }

  if( cfData->versionPhase == -1 )
  {
      return False;
  }
  else if( stringCmp( cfData->filePath, "BAD" ) == True )
  {
      return False;
  }
  else if( stringCmp( cfData->cpuSchedCode, "BAD" ) == True )
  {
      return False;
  }
  else if( cfData->quantumTime == -1 )
  {
      return False;
  }
  else if( cfData->memAvailable == -1 )
  {
      return False;
  }
  else if( cfData->processorCycleTime == -1 )
  {
      return False;
  }
  else if( cfData->ioCycleTime == -1 )
  {
      return False;
  }
  else if( stringCmp( cfData->logTo, "BAD" ) == True )
  {
      return False;
  }
  else if( stringCmp( cfData->logFilePath, "BAD" ) == True )
  {
      return False;
  }

  return True;
}
//
// displayConfig Function Implementation ///////////////////////////////////
//

/**
 * @brief This function displays a ConfigData struct
 *
 * @details This function checks if the given struct is null, if not, it
 *          prints the assocaited elements of a configuration struct.
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @post cfData is unchanged at post
 *
 * @exception nullPointer
 *            If the given ConfigData pointer is null, immediately return -1.
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @return success of method operation (bool)
 */
Boolean displayConfig( ConfigData *config )
{
    if (config == NULL)
    {
        return False;
    }

    printf( "Config File Dump\n" );
    printf( "================\n" );
    printf( "Version            : %d\n", config->versionPhase );
    printf( "Program file name  : %s\n", config->filePath );
    printf( "CPU schedule code  : %s\n", config->cpuSchedCode );
    printf( "Quantim Time       : %d\n", config->quantumTime );
    printf( "Memory size        : %d\n", config->memAvailable );
    printf( "Process cycle rate : %d\n", config->processorCycleTime );
    printf( "I/O cycle rate     : %d\n", config->ioCycleTime );
    printf( "Log to code        : %s\n", config->logTo );
    printf( "Log file name      : %s\n", config->logFilePath );
    return True;
}
//
// getIntLine Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a line from a given file.
 *
 * @details This function extracts a line from a configuration file whose
 *          data is an integer.
 *
 * @pre filePointer a pointer to a file
 *
 * @pre expected a pointer to a string
 *
 * @pre min an integer
 *
 * @pre max an integer
 *
 * @post expected, min, and max are unchanged
 *
 * @post filePointer now points at the next line in the file
 *
 * @par Algorithm
 *      The algorithm first extracts the line from the configuration file and
 *      checks to see if what was extracted is the same to what was expected.
 *
 * @par Algorithm
 *      If they are the same the algorithm then gets the data stored on the
 *      right side of the colon (:) in the line.
 *
 * @par Algorithm
 *      The algorithm then tries to extract an integer value from the right side
 *      of the colon. If it is able to, it returns that integer.
 *
 * @exception notLine
 *            If the line that was extracted was not the expected line, the
 *            algoithm immediately returns -1.
 *
 * @exception notInt
 *            If an integer between min and max was unable to be extracted,
 *            immediately return -1.
 *
 * @param[in] filePointer a pointer to a configuration file
 *
 * @param[in] expected a pointer to an expected string
 *
 * @param[in] min an integer for the minimum range of the return value
 *
 * @param[in] max an integer for the maximum range of the return value
 *
 * @return extracted integer data (int)
 */
int getIntLine( FILE *filePointer, const char *expected, int min, int max )
{
    Boolean isValid;
    char delimiter = ':';
    char *fileLinePtr, *lineDataPtr;

    fileLinePtr = extractLine( filePointer );
    isValid = checkExtractedLine(  fileLinePtr, expected, delimiter );

    if ( isValid == True )
    {
        lineDataPtr = stringSplitAfter(  fileLinePtr, delimiter );
        free ( fileLinePtr );

        int lineData = extractInt( lineDataPtr, min, max );

        if( lineData != -1  )
        {
            return lineData;
        }

        free( lineDataPtr );
    }
    else
    {
        free( fileLinePtr );
    }

    printf( "Malformed input data or line for %s\n", expected );
    return -1;
}
//
// extractInt Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts an integer from a string
 *
 * @details If possible this function extracts an integer from a string, and
 *          checks if it is within the given bounds (min, max).
 *
 * @pre dataAsStr a pointer to a string
 *
 * @pre min an integer
 *
 * @pre max an integer
 *
 * @post all parameters remain the same at post
 *
 * @par Algorithm
 *      The algoithm first checks if the given string is an integer.
 *
 * @par Algorithm
 *      If it is an integer, the algoithm then converts the string into an
 *      integer and checks if it's within the appropriate bounds. If it is
 *      that intger is then returned
 *
 * @exception notInt
 *            If the given string was not an integer, immediately return -1.
 *
 * @exception boundsError
 *            If the found integer was < min or > than max, immediately
 *            return -1.
 *
 * @param[in] dataAsStr a pointer to a string
 *
 * @param[in] min an integer for the minimum range of the return value
 *
 * @param[in] max an integer for the maximum range of the return value
 *
 * @return the value extracted from the config. A -1 is returned if the
 *         value is bad.
 */
int extractInt( char *dataAsStr, int min, int max )
{
    int lineData;
    Boolean dataIsValid;

    dataIsValid = checkInt( dataAsStr );

    if ( dataIsValid == True )
    {
        lineData = stringToI( dataAsStr );

        if( min <= lineData && lineData <= max )
        {

            free ( dataAsStr );
            return lineData;
        }

        free ( dataAsStr );
        printf( "Data not between %d, and %d\n", min, max );
        return -1;
    }

    return -1;
}
//
// getStrLine Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a line from a given file.
 *
 * @details This function extracts a line from a configuration file whose
 *          data is a string.
 *
 * @pre filePointer a pointer to a file
 *
 * @pre expected a pointer to a string
 *
 * @post expected is unchanged
 *
 * @post filePointer now points at the next line in the file
 *
 * @par Algorithm
 *      The algorithm first extracts the line from the configuration file and
 *      checks to see if what was extracted is the same to what was expected.
 *
 * @par Algorithm
 *      If they are the same the algorithm then gets the data stored on the
 *      right side of the colon (:) in the line.
 *
 * @par Algorithm
 *      The algorithm then tries to extract a string from the right side
 *      of the colon. If it is able to, it returns that string.
 *
 * @exception notLine
 *            If the line that was extracted was not the expected line, the
 *            algoithm immediately returns -1.
 *
 * @param[in] filePointer a pointer to a configuration file
 *
 * @param[in] expected a pointer to an expected string
 *
 * @return extract string data (char *)
 */
char *getStrLine( FILE *filePointer, const char *expected )
{
    Boolean isValid;
    char delimiter = ':';
    char *fileLinePtr, *lineInfoPtr;

    fileLinePtr = extractLine( filePointer );
    isValid = checkExtractedLine( fileLinePtr, expected, ':' );

    if( isValid == True )
    {
        lineInfoPtr = stringSplitAfter( fileLinePtr, delimiter );
        free ( fileLinePtr );
        return lineInfoPtr;
    }

    free ( fileLinePtr );
    printf( "%s line is invalid.\n", expected );
    return "BAD";
}
//
// getCpuSchedCode Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a line from a given file.
 *
 * @details This function extracts a line from a configuration file whose
 *          data is a cpu schedule code.
 *
 * @pre filePointer a pointer to a file
 *
 * @post filePointer now points at the next line in the file
 *
 * @par Algorithm
 *      The algorithm first extracts the line from the configuration file and
 *      checks to see if what was extracted is the same to what was expected.
 *
 * @par Algorithm
 *      If they are the same the algorithm then gets the data stored on the
 *      right side of the colon (:) in the line.
 *
 * @par Algorithm
 *      The algorithm then checks the code found against all possible codes. If
 *      the code is one of the possible codes it is returned.
 *
 * @exception notLine
 *            If the line that was extracted was not the expected line, the
 *            algoithm immediately returns -1.
 *
 * @exception badCode
 *            If the code that was extracted was bad, immediately return "BAD".
 *
 * @param[in] filePointer a pointer to a configuration file
 *
 * @return extracted cpu schedule code (char *)
 */
char *getCpuSchedCode( FILE *filePointer )
{
    char *cpuSchedCodePtr;
    int codeCheckValue;
    cpuSchedCodePtr = getStrLine( filePointer,
                                  CPU_SCHED_CODE_EXPECTED );

    if ( stringCmp( cpuSchedCodePtr, "BAD" ) == True)
    {
      return "BAD";
    }

    codeCheckValue= cpuSchedCodeCheck( cpuSchedCodePtr );

    if ( codeCheckValue == True )
    {
      return cpuSchedCodePtr;
    }
    else
    {
        free( cpuSchedCodePtr );
        printf( "CPU Scheduling Code is invalid.\n" );
        return "BAD";
    }
}
//
// cpuSchedCodeCheck Function Implementation ///////////////////////////////////
//

/**
 * @brief This function checks a cpu schedule code.
 *
 * @details This function takes in a cpu schedule code, and checks it
 *          against all possible cpu schedule codes.
 *
 * @pre code a pointer to a string
 *
 * @post code On failure or if the values stored in the memory code
 *            is pointe to is NONE, code is null at post. Otherwise
 *            code remains unchanged.
 *
 * @par Algorithm
 *      The algorithm defines the table of codes to compare the code
 *      string to.
 *
 * @par Algorithm
 *      The algorithm then goes through each code to see if the given code is
 *      correct. If the code is NONE, FCFS-P is returned, otherwise return
 *      the given code.
 *
 * @exception badCode
 *            If the code is not a cpu schedule code, immediately return -1.
 *
 * @param[in] code a pointer to a cpu schedule code string.
 *
 * @return the new code (char *)
 */
Boolean cpuSchedCodeCheck(  char *code )
{
    int iterator;

    for( iterator = 5; iterator >= 0; iterator-- )
    {
        if( cmpExpected( code, cpuSchedCodeLookUp[ iterator ] ) == 0 )
        {
          return True;
        }
    }

    printf( "Bad code. Please revise the CPU Scheduling Code value\n" );
    return False;
}
//
// getLogTo Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a line from a given file.
 *
 * @details This function extracts a line from a configuration file whose
 *          data is the I/O location.
 *
 * @pre filePointer a pointer to a file
 *
 * @post filePointer now points at the next line in the file
 *
 * @par Algorithm
 *      The algorithm first extracts the line from the configuration file and
 *      checks to see if what was extracted is the same to what was expected.
 *
 * @par Algorithm
 *      If they are the same the algorithm then gets the data stored on the
 *      right side of the colon (:) in the line.
 *
 * @par Algorithm
 *      The algorithm then checks the logging type against all possible types. If
 *      the type is one of the possible types it is returned.
 *
 * @exception notLine
 *            If the line that was extracted was not the expected line, the
 *            algoithm immediately returns -1.
 *
 * @exception badType
 *            If the type that was extracted was bad, immediately return "BAD".
 *
 * @param[in] filePointer a pointer to a configuration file
 *
 * @return extracted logging type (char *)
 */
char *getLogTo( FILE *filePointer )
{
    char *logToCodePtr;
    Boolean codeIsValid;

    logToCodePtr = getStrLine( filePointer,
                                     LOG_TO_EXPECTED );

    if( stringCmp( logToCodePtr, "BAD") == True )
    {
      return "BAD";
    }

    codeIsValid = logToCodeCheck(logToCodePtr);

    if( codeIsValid == True )
    {
        return logToCodePtr;
    }

    free( logToCodePtr );
    return "BAD";
}
//
// logToCodeCheck Function Implementation ///////////////////////////////////
//

/**
 * @brief This function checks a logging type.
 *
 * @details This function takes in a logging type, and checks it
 *          against all possible clogging types.
 *
 * @pre type a pointer to a string
 *
 * @post type remains the same on post
 *
 * @par Algorithm
 *      The algorithm checks the given type against the three possible types.
 *
 * @exception badType
 *            If the type is not a logging type, immediately return -1.
 *
 * @param[in] type a pointer to logging type string.
 *
 * @return the validity of a given logTo code (bool)
 */
Boolean logToCodeCheck( char *type)
{
    if( stringCmp( type, "Both" ) == True
     || stringCmp( type, "Monitor" ) == True
     || stringCmp( type, "File" ) == True )
    {
        return True;
    }

    printf( "Bad Log To code: %s\n", type );
    return False;
}
