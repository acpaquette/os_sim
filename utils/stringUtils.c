// StringUtils Implementation File Information ///////////////////////////////
/**
* @file stringUtils.c
*
* @brief Implementation for stringUtils
*
* @details Implements all utility functions used in modifying or manageing
* strings
*
* @version 1.00
* C.S student (29 January 2018)
* Initial creation of stringUtils
*
* @version 1.01
* C.S student (25 February 2018)
* Added stringLen function and the StringNode struct
*
* @note Requires stringUtils.h
*/

#include "stringUtils.h"

const char NEW_LINE = '\n';
const char END_OF_FILE = EOF;
const char NULL_TERMINAL = '\0';

//
// stringCpy Function Implementation ///////////////////////////////////
//

/**
 * @brief This function copies a string into another string
 *
 * @details This function copies one string into another up to a specified
 *          delimiter
 *
 * @pre originPtr a pointer to the origin string
 *
 * @pre destinationPtr a pointer to the destination string
 *
 * @pre terminal a character
 *
 * @post originPtr and terminal remain unchanged at post
 *
 * @post destinationPtr now holds the same string that originPtr held
 *
 * @param[in] originPtr a pointer to a string
 *
 * @param[in] terminal a character
 *
 * @param[out] destinationPtr a pointer to a string
 *
 * @return success of method operation (bool)
 */
Boolean stringCpy( char *originPtr, char *destinationPtr, char terminal )
{
    int iterator = 0;
    do
    {
        destinationPtr[ iterator ] = originPtr[ iterator ];
        iterator++;
    }
    while( originPtr[ iterator ] != terminal );
    destinationPtr[ iterator ] = terminal;

    return True;
}
//
// stringLen Function Implementation ///////////////////////////////////
//

/**
 * @brief This function gets the length of a string
 *
 * @details This function loops over a string adding to a counter until it
 *          hits a null terminal and returns the length of the string
 *
 * @pre string a pointer to a string
 *
 * @post string remains unchanged at post
 *
 * @param[in] string a pointer to a string
 *
 * @return The length of the string (int)
 */
int stringLen( char *string )
{
    int length = 0;

    do
    {
        length++;
    }
    while( string[length] != NULL_TERMINAL );

    return length;
}
//
// stringCmp Function Implementation ///////////////////////////////////
//

/**
 * @brief This function compares a string to another string
 *
 * @details This function compares one string to another string
 *
 * @pre stringOnePtr a pointer to the origin string
 *
 * @pre stringTwoPtr a pointer to the destination string
 *
 * @post stringOnePtr and stringTwoPtr remain the same at post
 *
 * @param[in] stringOnePtr a pointer to a string
 *
 * @param[in] stringTwoPtr a pointer to a string
 *
 * @return success of method operation (bool)
 */
Boolean stringCmp( char *stringOnePtr, char *stringTwoPtr )
{
    int iterator = 0;
    do
    {
        if( stringOnePtr[ iterator ] != stringTwoPtr[iterator] )
        {
          return False;
        }

        iterator++;
    }
    while( stringOnePtr[ iterator ] != NULL_TERMINAL
        || stringTwoPtr[ iterator ] != NULL_TERMINAL );

    return True;
}
//
// cmpExpected Function Implementation ///////////////////////////////////
//

/**
 * @brief This function compares a string to another string
 *
 * @details This function compares one string to another string
 *
 * @pre stringOnePtr a pointer to the origin string
 *
 * @pre stringTwoPtr a pointer to the destination string
 *
 * @post stringOnePtr and stringTwoPtr remain the same at post
 *
 * @param[in] stringOnePtr a pointer to a string
 *
 * @param[in] stringTwoPtr a pointer to a string
 *
 * @return success of method operation (bool)
 */
Boolean cmpExpected( char *actualPtr, const char *expectedPtr )
{
    int iterator = 0;
    do
    {
        if ( actualPtr[ iterator ] != expectedPtr[ iterator ] )
        {
          return False;
        }

        iterator++;
    }
    while( actualPtr[ iterator ] != NULL_TERMINAL
        && expectedPtr[ iterator ] != NULL_TERMINAL );

    return True;
}
//
// stringResize Function Implementation ///////////////////////////////////
//

/**
 * @brief This function resizes a given string
 *
 * @details This function takes a string and resizes it to the size
 *          specified in sizeOfArray
 *
 * @pre stringPtr a pointer to the origin string
 *
 * @pre sizeOfArray an integer that presents the size of the array
 *
 * @post sizeOfArray remains the same at post
 *
 * @post stringPtr is null at post
 *
 * @param[in] stringOnePtr a pointer to a string
 *
 * @param[in] sizeOfArray integer
 *
 * @return newly resized string (char *)
 */
char *stringResize( char *stringPtr, int sizeOfArray )
{
    char *tempStringPtr;

    tempStringPtr = calloc( sizeOfArray + 1, sizeof( char ) );
    stringCpy( stringPtr, tempStringPtr, NULL_TERMINAL );
    free( stringPtr );

    tempStringPtr[ sizeOfArray - 1 ] = NULL_TERMINAL;
    return tempStringPtr;
}
//
// stringToI Function Implementation ///////////////////////////////////
//

/**
 * @brief This function converts a given string into an integer
 *
 * @details This function converts a given string into an integer if all values
 *          within the string are numerical.
 *
 * @pre stringPtr a pointer to a string
 *
 * @post stringPtr is unchanged at post
 *
 * @par Algorithm
 *      The algorithm loops over the string adding each numerical value
 *      from the string into a return value. Where after each loop past
 *      the first are multiplied by 10.
 *
 * @param[in] stringPtr a pointer to a integer as a string.
 *
 * @return newly converted integer from the given string (int)
 */
int stringToI( char *stringPtr )
{
    int iterator = 0;
    int strAsNumber = 0;
    char decimal = '.';

    do
    {
        if( stringPtr[ iterator ] == decimal )
        {
            return strAsNumber;
        }

        if( iterator != 0 )
        {
            strAsNumber *= 10;
        }

        strAsNumber += stringPtr[ iterator ] - '0';
        iterator++;

    }
    while( stringPtr[ iterator ] != NULL_TERMINAL );

    return strAsNumber;
}
//
// stringToF Function Implementation ///////////////////////////////////
//

/**
 * @brief This function converts a given string into an integer
 *
 * @details This function converts a given string into an integer if all values
 *          within the string are numerical.
 *
 * @pre stringPtr a pointer to a string
 *
 * @post stringPtr is unchanged at post
 *
 * @par Algorithm
 *      The algorithm loops over the string adding each numerical value
 *      from the string into a return value. Where after each loop past
 *      the first are multiplied by 10.
 *
 * @par Algorithm
 *      Once it sees a decimal the function begins counting the decimal places.
 *      After the funciton finishs looping the total is then dividied by
 *      10^decimal count.
 *
 * @param[in] stringPtr a pointer to a float as a string.
 *
 * @return newly converted float from the given string (float)
 */
float stringToF(  char *stringPtr )
{
    int iterator = 0;
    float strAsNumber = 0;
    float strAsFloat = 0;
    int decimalCount = 0;
    int decimalExponent = 1;
    char decimal = '.';

    do
    {
        if( stringPtr[iterator] == decimal )
        {
            decimalCount++;
            iterator++;
            continue;
        }

        if( decimalCount )
        {
            decimalCount++;
        }

        if( iterator != 0 )
        {
            strAsNumber *= 10;
        }

        strAsNumber += stringPtr[iterator] - '0';
        iterator++;

    }
    while( stringPtr[iterator] != NULL_TERMINAL );

    for( decimalCount -= 1; decimalCount != 0; decimalCount-- )
    {
        decimalExponent *= 10;
    }

    strAsFloat = strAsNumber / decimalExponent;
    return strAsFloat;
}
//
// stringSplitBefore Function Implementation ///////////////////////////////////
//

/**
 * @brief This function returns the left side of a delimiter
 *
 * @details This function splits a string on a given delimiter and returns
 *          the left side of the delimiter.
 *
 * @pre originPtr a pointer to a string
 *
 * @pre delimiter a character
 *
 * @post all parameters are the same at post
 *
 * @par Algorithm
 *      The algoithm copies character by character into a new array until it
 *      hits the delimiter character. This destination array resizes
 *      dynamically as needed.
 *
 * @param[in] originPtr a pointer to a string
 *
 * @param[in] delimiter a character
 *
 * @return newly split string (char *)
 */
char *stringSplitBefore( char *originPtr, char delimiter )
{
    size_t sizeOfArray = 10;
    int iterator = 0;
    char *destinationPtr;
    destinationPtr = calloc( sizeOfArray + 1, sizeof( char ) );

    do
    {
        if( iterator == sizeOfArray )
        {
            sizeOfArray *= 2;
            destinationPtr = stringResize( destinationPtr, sizeOfArray );
        }

        destinationPtr[ iterator ] = originPtr[ iterator ];
        iterator++;

    }
    while( originPtr[ iterator ] != delimiter
        && originPtr[ iterator ] != NULL_TERMINAL
        && originPtr[ iterator ] != END_OF_FILE );

    return destinationPtr;
}
//
// stringSplitAfter Function Implementation ///////////////////////////////////
//

/**
 * @brief This function returns the right side of a delimiter
 *
 * @details This function splits a string on a given delimiter and returns
 *          the right side of the delimiter.
 *
 * @pre originPtr a pointer to a string
 *
 * @pre delimiter a character
 *
 * @post all parameters are the same at post
 *
 * @par Algorithm
 *      The algoithm first reads over everything until it gets to the
 *      given delimiter, returning the iterator.
 *
 * @par Algorithm
 *      The algoithm then copies character by character into a new array
 *      until it hits the delimiter character. This destination array resizes
 *      dynamically as needed.
 *
 * @param[in] originPtr a pointer to a string
 *
 * @param[in] delimiter a character
 *
 * @return newly split string (char *)
 */
char *stringSplitAfter( char *originPtr, char delimiter )
{
    size_t sizeOfArray = 10;
    int originIterator = 0;
    int destIterator = 0;
    char *destinationPtr;
    destinationPtr = calloc( sizeOfArray + 1, sizeof( char ) );
    destinationPtr[ sizeOfArray - 1 ] = NULL_TERMINAL;

    originIterator = readToDelimiter( originPtr, delimiter, originIterator );

    do
    {
        if( destIterator == sizeOfArray )
        {
            sizeOfArray *= 2;
            destinationPtr = stringResize( destinationPtr, sizeOfArray );
        }

        destinationPtr[ destIterator ] = originPtr[ originIterator ];
        destIterator++;
        originIterator++;

    }
    while( originPtr[ originIterator ] != NEW_LINE
        && originPtr[ originIterator ] != NULL_TERMINAL );

    destinationPtr[ destIterator ] = NULL_TERMINAL;
    return destinationPtr;
}
//
// readToDelimiter Function Implementation ///////////////////////////////////
//

/**
 * @brief This function reads up to delimiter
 *
 * @details This function reads up to a delimiter and returns the array
 *          position of the character after the delimiter
 *
 * @pre stringPtr a pointer to a string
 *
 * @pre delimiter is a character
 *
 * @pre iterator an integer
 *
 * @post all parameters remain the same at post
 *
 * @par Algorithm
 *      The algorithm reads up to a delimiter, updating the iterator until it
 *      it hits the delimiter.
 *
 * @param[in] stringPtr a pointer to a string
 *
 * @param[in] delimiter a character to split the string on
 *
 * @param[in] iterator integer represeting the position in the array
 *
 * @return position after the delimiter (int)
 *
 * @note Function provides three output quantities
 */
int readToDelimiter( char *stringPtr, char delimiter, int iterator )
{
    do
    {
        iterator++;
    }
    while( stringPtr[ iterator ] != delimiter
        && stringPtr[ iterator ] != 0 );

    iterator++;

    if( stringPtr[ iterator ] == ' ' )
    {
        iterator++;
    }

    return iterator;
}
//
// checkInt Function Implementation ///////////////////////////////////
//

/**
 * @brief This function checks if a given string is a number
 *
 * @details Thus function checks if a given string is made up only
 *          of numerical characters and a potential decimal
 *
 * @pre dataPtr a pointer to a string
 *
 * @post dataPtr remains the same at post
 *
 * @par Algorithm
 *      The algoithm reads each character in the string checking if its
 *      an integer character or a decimal.
 *
 * @exception notNumeric
 *            If any character in the string is not numerical or a decimal,
 *            immediately return -1.
 *
 * @param[in] dataPtr a pointer to a string
 *
 * @return success of method operation (bool)
 */
Boolean checkInt( char *dataPtr )
{
    int iterator = 0;

    do
    {
        if( '0' <= dataPtr[ iterator ]
            && dataPtr[ iterator ] <= '9' )
        {
            iterator++;
        }
        else if( dataPtr[ iterator ] == '.' )
        {
            iterator++;
        }
        else
        {
            return False;
        }

    }
    while( dataPtr[ iterator ] != NULL_TERMINAL );

    return True;
}
//
// addStringNode Function Implementation ///////////////////////////////////
//

/**
 * @brief This function adds a StringNode struct to a StringNode linked list
 *
 * @details This function adds a StringNode struct onto the tail end of a
 *          StringNode linked list.
 *
 * @pre head a pointer to a StringNode
 *
 * @pre node a pointer to a StringNode
 *
 * @post head and node remain unchanged at post
 *
 * @param[in] head a pointer to a StringNode
 *
 * @param[in] node a pointer to a StringNode
 *
 * @return nothing is returned as node is added to the tail of the list (void)
 */
void addStringNode( StringNode *head, StringNode *node )
{
    if( node == NULL )
    {
        return;
    }

    if( head != NULL )
    {
        while( head->nextNode != NULL )
        {
            head = head->nextNode;
        }

        head->nextNode = node;
        return;
    }
    return;
}
//
// freeStringList Function Implementation ///////////////////////////////////
//

/**
 * @brief This function deallocates a StringNode linked list
 *
 * @details This function deallocates a StringNode linked list by iterating
 *          over the nodes in the list and deallocating the appropriate fields
 *
 * @pre startNode a pointer to a StringNode struct. Ideally the head of the
 *                linked list
 *
 * @post startNode is now a NULL pointer
 *
 * @param[in] startNode a pointer to a StringNode struct
 *
 * @return nothing is returned as nothing needs to be returned
 *         from deallocation (void)
 */
void freeStringList( StringNode *startNode )
{
    StringNode *current;

    while( startNode != NULL )
    {
        current = startNode;
        startNode = startNode->nextNode;
        free( current->string );
        free( current );
    }
}
