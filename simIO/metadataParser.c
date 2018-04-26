// MetadataParser Implementation File Information ///////////////////////////////
/**
* @file metadataParser.c
*
* @brief Implementation for metadataParser
*
* @details Implements all functions for parsing a metadata file
*
* @version 1.00
* C.S student (30 January 2018)
* Initial creation of metadataParser
*
* @note Requires metadataParser.h
*/

#include "metadataParser.h"
//
// readMetadata Function Implementation ///////////////////////////////////
//

/**
 * @brief This function reads in a metadata file
 *
 * @details This function reads in a metadata file, parsing each element
 *          into a MetaNode linked list which it then returns
 *
 * @pre filePointer a pointer to a file
 *
 * @post filePointer should point to an end of file character at post
 *
 * @par Algorithm
 *      The algorithm first checks if the beginning line is correct, if
 *      it is it begins parsing the metadata.
 *
 * @par Algorithm
 *      It begins reading and placing characters into a string until it
 *      hits a semicolon (;). This indicates that it has read a metadata
 *      component that needs to be parsed.
 *
 * @par Algorithm
 *      After extracting the node, it adds it to the linked list, then resets
 *      the string to read in the next component of metadata.
 *
 * @par Algorithm
 *      This repeats until either the end of the file is reached or a period
 *      (.) is reached. The last node is then read in.
 *
 * @par Algorithm
 *      The algorithm then checks the final line. If all is successful
 *      the head of the list is returned.
 *
 * @exception badStart
 *            If the start line is incorrect, immidiately return null.
 *
 * @exception badNode
 *            If an extracted node is null, the metadata component was bad
 *            and return a null pointer.
 *
 * @exception badEnd
 *            If the end line is incorrect, immidiately return null.
 *
 * @param[in] filePointer a pointer to a file
 *
 * @return the head to the newly built metadata linked list (MetaNode *)
 */
MetaNode *readMetadata( FILE *filePointer )
{
    int validStartLine;
    int validEndLine;
    validStartLine = removeExpectedLine( filePointer, META_START_LINE_EXPECTED, '\n' );

    if(  validStartLine == False )
    {
        printf( "Bad startline in metadata file.\n" );
        return NULL;
    }

    MetaNode *currentNode = NULL;
    MetaNode *newNode = NULL;
    size_t sizeOfArray = 10;
    int iterator = 0;
    char currentCharacter = NULL_TERMINAL;
    char *linePtr = calloc( sizeOfArray + 1, sizeof( char ) );
    linePtr[sizeOfArray - 1] = NULL_TERMINAL;

    do
    {
        currentCharacter = fgetc( filePointer );
        linePtr[ iterator ] = currentCharacter;
        iterator++;

        if( iterator == sizeOfArray )
        {
            sizeOfArray *= 2;
            linePtr = stringResize( linePtr, sizeOfArray );
        }

        if( currentCharacter == ';' )
        {
            newNode = extractNode( linePtr );
            currentNode = addNode( currentNode, newNode );
            iterator = 0;
            free( linePtr );

            if ( newNode == NULL )
            {
              metaParseError( linePtr );
              freeList( currentNode );
              return NULL;
            }

            sizeOfArray = 10;
            linePtr = calloc( sizeOfArray + 1, sizeof( char ) );
        }

    }
    while( currentCharacter != '.' && currentCharacter != END_OF_FILE );

    newNode = extractNode( linePtr );

    if( newNode == NULL )
    {
      metaParseError( linePtr );
      free( linePtr );
      freeList( currentNode );
      return NULL;
    }

    free( linePtr );
    currentNode = addNode( currentNode, newNode );
    fgetc( filePointer );
    validEndLine = removeExpectedLine( filePointer, META_END_LINE_EXPECTED, '.' );

    if( validEndLine == False )
    {
        printf( "Bad end line in metadata file.\n" );
        freeList( currentNode );
        return NULL;
    }
    return currentNode;
}
//
// readMetadata Function Implementation ///////////////////////////////////
//

/**
 * @brief This function displays a metadata parsing error
 *
 * @details This function displays a metadata parsing error, and displays
 *          the line that it error on.
 *
 * @pre errorLine a pointer to a string
 *
 * @post errorLine remains the same at post
 *
 * @param[in] errorLine a pointer to a string
 *
 * @return success of method operation (bool)
 */
int metaParseError( char *errorLine )
{
  printf( "==========================\n" );
  printf( "Bad metadata file on command: %s. Dumping.\n", errorLine );
  printf( "==========================\n" );
  return 0;
}
