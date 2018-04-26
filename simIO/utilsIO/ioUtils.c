// IoUtils Implementation File Information ///////////////////////////////
/**
* @file ioUtils.c
*
* @brief Implementation for ioUtils
*
* @details Implements all utility functions used in parsing either the
* config file or metadata file
*
* @version 1.00
* C.S student (30 January 2018)
* Initial creation of ioUtils
*
* @note Requires ioUtils.h
*/

#include "ioUtils.h"

//
// extractLine Function Implementation ///////////////////////////////////
//

/**
 * @brief This function reads a line from a given file
 *
 * @details Given a pointer to the file, this function reads that file
 *          character by character until it hits either an end of file character
 *          or a new line character. Whichever comes first
 *
 * @pre filePointer is a pointer to a file
 *
 * @post filePointer now points to the next line in the file
 *
 * @param[in] filePointer
 *            Pointer to some file
 *
 * @return extract line from file (char *)
 *
 * @note requires the default C I/O tools
 */
char *extractLine( FILE *filePointer )
{
    int sizeOfArray = 10, iterator = 0;
    char *lineStringPtr, currentCharacter;
    lineStringPtr = calloc( sizeOfArray + 1, sizeof( char ) );
    lineStringPtr[ sizeOfArray - 1 ] = NULL_TERMINAL;
    do
    {
        if( iterator == sizeOfArray )
        {
            sizeOfArray *= 2;
            lineStringPtr = stringResize( lineStringPtr, sizeOfArray );
        }

        currentCharacter = fgetc( filePointer );
        lineStringPtr[ iterator ] = currentCharacter;
        iterator++;

    }
    while( currentCharacter != NEW_LINE
        && currentCharacter != END_OF_FILE);

    return lineStringPtr;
}
//
// checkExtractedLine Function Implementation ///////////////////////////////////
//

/**
 * @brief Checks if two strings are equal
 *
 * @details Given a string and an expected result, check that string for
 *          the expected result.
 *
 * @pre extractedLinePtr is a pointer to a string
 *
 * @pre expected is a constant string to compare to
 *
 * @pre delimiter a delimiter to crop the expected string to
 *
 * @post all input params are unchanged at post
 *
 * @par Algorithm
 *      The algorithm clips the extractedLinePtr based on the delimiter
 *      and returns the string infront of the delimiter.
 *
 * @par Algorithm
 *      The clipped string is then compared against the expected string for
 *      correctness. Returns 0 if the strings are equal.
 *
 * @param[in] extractedLinePtr
 *            None null pointer to a given string
 *
 * @param[in] expected
 *            None null pointer to a given const string
 *
 * @param[in] delimiter
 *            None null char
 *
 * @return success of method operation (bool)
 *
 */
Boolean checkExtractedLine( char *extractedLinePtr,
                        const char *expected,
                        char delimiter )
{
    Boolean isValid;
    char *lineActualPtr;

    lineActualPtr = stringSplitBefore( extractedLinePtr, delimiter );
    isValid = cmpExpected( lineActualPtr, expected );
    free( lineActualPtr );

    return isValid;
}
//
// getStartLine Function Implementation ///////////////////////////////////
//

/**
 * @brief Removes and checks a line from a given file
 *
 * @details Given a file and an expected string, extract a line from the file
 *          and compare to the expected string.
 *
 * @pre filePointer is a none null pointer to a file
 *
 * @pre expected none null pointer to a const string
 *
 * @pre delimiter a delimiter to crop the expected string to
 *
 * @post all input params are unchanged at post
 *
 * @par Algorithm
 *      The algorithm first extracts a line from the given file, clips that
 *      line based on the delimiter, and compares the clipped line
 *      against the expected line.
 *
 * @exception notEqual
 *            If the clipped string and the expected string are not the same
 *            the algorithm prints an error and returns -1.
 *
 * @param[in] filePointer
 * decs
 *
 * @param[in] expected
 * decs
 *
 * @param[in] delimiter
 * decs
 *
 * @return success of method operation (bool)
 */
Boolean removeExpectedLine( FILE *filePointer, const char *expected, char delimiter )
{
    char *linePtr;
    Boolean isValid;

    linePtr = extractLine( filePointer );
    isValid = checkExtractedLine( linePtr, expected, delimiter );
    free( linePtr );

    if ( isValid == True )
    {
        return True;
    }

    printf( "Line %s is invalid. Expected %s\n", linePtr, expected );
    return False;
}
