// MetadataParser Implementation File Information ///////////////////////////////
/**
* @file metaLinkedList.c
*
* @brief Implementation for metaLinkedList
*
* @details Implements all functions for creating and displaying a
*          metadata linked list
*
* @version 1.00
* C.S student (31 January 2018)
* Initial creation of metaLinkedList
*
* @note Requires metaLinkedList.h
*/

#include "metaLinkedList.h"

const char *commandLookUp[10] =
{
    [0] = "start",
    [1] = "end",

    [2] = "run",

    [3] = "access",
    [4] = "allocate",

    [5] = "hard drive",
    [6] = "keyboard",

    [7] = "hard drive",
    [8] = "printer",
    [9] = "monitor"
};

//
// displayNode Function Implementation ///////////////////////////////////
//

/**
 * @brief This function displays a MetaNode struct
 *
 * @details The function checks if the struct is not NULL
 *          then prints out the three main fields
 *
 * @pre node is a pointer to a MetaNode struct
 *
 * @post node pointer is unchanged
 *
 * @exception nullPointer
 *            Function returns False if the given node is null
 *
 * @param[in] node Pointer to a given node struct
 *
 * @return success of method operation (bool)
 */
Boolean displayNode( MetaNode *node )
{
    if( node != NULL )
    {
        printf( "Command Letter: %c\n", node->commandLetter );
        printf( "Operation String: %s\n", node->opString );
        printf( "Cycle Time: %d\n", node->cycleTime );
    }
    else
    {
        return False;
    }

    return True;
}
//
// displayNodeRecursive Function Implementation ///////////////////////////////////
//

/**
 * @brief This function displays a list of MetaNode structs
 *
 * @details The function checks if the struct is not NULL
 *          then recursively prints all nodes in the list
 *
 * @pre node is a pointer to a MetaNode struct
 *
 * @post node pointer is unchanged
 *
 * @exception nullPointer
 *            Function returns False if the given node is null
 *
 * @param[in] node Pointer to a given node struct
 *
 * @return success of method operation (bool)
 */
Boolean displayNodeRecursive( MetaNode *node )
{
    if( node != NULL)
    {
        printf( "The data item component letter is: %c\n", node->commandLetter );
        printf( "The data item operation string is: %s\n", node->opString );
        printf( "The data item cycle time is      : %d\n", node->cycleTime );
        displayNodeRecursive( node->nextNode );
    }
    else
    {
        return False;
    }

    return True;
}
//
// displayList Function Implementation ///////////////////////////////////
//

/**
 * @brief This function displays a list of MetaNode structs
 *
 * @details The function checks if the struct is not NULL
 *          then prints a short preamble before recursively printing
 *          all nodes in the list
 *
 * @pre node is a pointer to a MetaNode struct
 *
 * @post node pointer is unchanged
 *
 * @par Algorithm
 *      The algorithm first checks if the node it was given is null.
 *
 * @par Algorithm
 *      The algorithm then checks if it is at the head of the list. If it is
 *      not it rewinds the list.
 *
 * @par Algorithm
 *      The list is then recursively displayed from the head to the tail
 *
 * @exception nullPointer
 *            Function returns False if the given node is null
 *
 * @param[in] node Pointer to a given node struct
 *
 * @return success of method operation (bool)
 */
Boolean displayList( MetaNode *node )
{
    if( node != NULL )
    {
        if( node->previousNode == NULL )
        {
            node = rewindList( node );
        }

        printf( "Meta-Data File Dump\n" );
        printf( "===================\n" );
        displayNodeRecursive( node );
    }
    else
    {
      return False;
    }

    return True;
}
//
// extractNode Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a node from a given string
 *
 * @details This function parses a string for all of the components it
 *          would need to build a MetaNode.
 *
 * @pre metaLinePtr a pointer to a metadata string
 *
 * @post metaLinePtr remains unchanged
 *
 * @par Algorithm
 *      Algorithm starts by making sure the string pointer is not null.
 *
 * @par Algorithm
 *      The algorithm then extracts the command letter, operation string
 *      and cycle time from the given metadata string.
 *
 * @par Algorithm
 *      These are then checked for correctness. If all aspects are correct
 *      a new MetaNode is constructed.
 *
 * @exception invalid metadata
 *            The check for the metadata was invalid, thus the function returns
 *            a null pointer.
 *
 * @param[in] metaLinePtr pointer to a string
 *
 * @return new MetaNode (MetaNode *)
 *
 * @note Function provides three output quantities
 */
MetaNode *extractNode( char *metaLinePtr )
{
    MetaNode *newNode = NULL;
    char commandLetter = NULL_TERMINAL;
    char *opStringPtr = NULL;
    char *cycleTimePtr = NULL;
    int validNode = 0;

    if (metaLinePtr == NULL)
    {
        return NULL;
    }

    commandLetter = extractCommandLetter( metaLinePtr );
    opStringPtr = extractOpString( metaLinePtr );
    cycleTimePtr = extractCycleTime( metaLinePtr );
    
    validNode = checkNode( commandLetter, opStringPtr, cycleTimePtr );

    if( validNode == True )
    {
        newNode = createNode( commandLetter,
                              opStringPtr,
                              stringToI( cycleTimePtr ) );
    }
    else
    {
        free( opStringPtr );
    }

    free( cycleTimePtr );
    return newNode;
}
//
// extractCommandLetter Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a command letter from a given string
 *
 * @details This function parses a string for a command letter that preceeds
 *          an open parenthese '('.
 *
 * @pre metaLinePtr a pointer to a metadata string
 *
 * @post metaLinePtr remains unchanged
 *
 * @par Algorithm
 *      The algorithm extracts the command letter region, by spliting the metaLine
 *      on an open parenthese.
 *
 * @par Algorithm
 *      The algorithm then moves an interator to the last character in the
 *      newly parsed command letter region.
 *
 * @par Algorithm
 *      It then retuns whatever character was found at the end of the metadata
 *      region.
 *
 * @param[in] metaLinePtr pointer to a string
 *
 * @return the last character from the metadata region (char)
 */
char extractCommandLetter( char *metaLine )
{
    int beforeIterator = 0;
    int afterIterator = 0;
    char commandLetter = NULL_TERMINAL;
    char *letterStrPtr = NULL;

    letterStrPtr = stringSplitBefore( metaLine, '(' );

    while( letterStrPtr[ beforeIterator ] == ' '
        || letterStrPtr[ beforeIterator ] == '\n')
    {
        beforeIterator++;
    }

    while (letterStrPtr[ afterIterator + 1] != 0)
    {
        afterIterator++;
    }

    if (letterStrPtr[ beforeIterator ] == letterStrPtr[ afterIterator ])
    {
        commandLetter = letterStrPtr[ afterIterator ];
    }
    else
    {
        commandLetter = 'Z';
    }

    free( letterStrPtr );
    return commandLetter;
}
//
// extractOpString Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a operation string from a given string
 *
 * @details This function parses a string for an operation string that is
 *          preceeded by an open parenthese '(' and is followed by a close
 *          parenthese ')'.
 *
 * @pre metaLinePtr a pointer to a metadata string
 *
 * @post metaLinePtr remains unchanged
 *
 * @par Algorithm
 *      The algorithm extracts the operation string region, by spliting the
 *      metaLine to everything after an open parenthese but before a close
 *      parenthese.
 *
 * @par Algorithm
 *      It then retuns whatever was found between the parentheses.
 *
 * @param[in] metaLinePtr pointer to a string
 *
 * @return newly extracted operation string (char *)
 */
char *extractOpString( char *metaLine )
{
    char *tempStringPtr;
    char *opStringPtr;

    tempStringPtr = stringSplitAfter( metaLine, '(' );
    opStringPtr = stringSplitBefore( tempStringPtr, ')' );
    free( tempStringPtr );

    return opStringPtr;
}
//
// extractCycleTime Function Implementation ///////////////////////////////////
//

/**
 * @brief This function extracts a cycle time from a given string
 *
 * @details This function parses a string for a cycle time that is
 *          preceeded by a closed parenthese ')' and is followed by a
 *          semicolon ';'.
 *
 * @pre metaLinePtr a pointer to a metadata string
 *
 * @post metaLinePtr remains unchanged
 *
 * @par Algorithm
 *      The algorithm extracts the cycle time region, by spliting the
 *      metaLine to everything after a closed parenthese but before semicolon.
 *
 * @par Algorithm
 *      It then retuns whatever was found in that region.
 *
 * @param[in] metaLinePtr pointer to a string
 *
 * @return newly extracted cycle time (char *)
 */
char *extractCycleTime( char *metaLine )
{
    char *tempStringPtr;
    char *cycleTimeStr;

    tempStringPtr = stringSplitAfter( metaLine, ')' ) ;
    cycleTimeStr = stringSplitBefore( tempStringPtr, ';' ) ;
    free( tempStringPtr ) ;

    return cycleTimeStr;
}
//
// checkNode Function Implementation ///////////////////////////////////
//

/**
 * @brief This function checks if all atributes of a MetaNode are valid
 *
 * @details This function checks the command letter, operation string, and
 *          cycle time parameters for correctness.
 *
 * @pre commandLetter a character
 *
 * @pre opString a pointer to a string
 *
 * @pre cycleTime a pointer to a string
 *
 * @post all parameters are the same at post
 *
 * @par Algorithm
 *      The Algorithm first checks the command letter by using it in a switch
 *      clause.
 *
 * @par Algorithm
 *      If a case was found, the algorithm then checks if the operation string
 *      for the given command letter is valid.
 *
 * @par Algorithm
 *      Finally, if the above to values are correct, the cycle time is then
 *      checked to see if it is an integer or not. If all is correct the
 *      the algorithm return 0.
 *
 * @exception invalid commandLetter
 *            The command letter is not specified as a potential option
 *
 * @exception invalid opString
 *            The opString was not found in the potential opStrings assocaited
 *            with a given commandLetter
 *
 * @exception invalid cycleTime
 *            The cycleTime is not an integer
 *
 * @param[in] commandLetter a commandletter character
 *
 * @param[in] opString a string pointer to an operation string
 *
 * @param[in] cycleTime a string pointer to a cycle time string
 *
 * @return success of method operation (bool)
 */
Boolean checkNode( char commandLetter, char *opString, char *cycleTime )
{
    Boolean valid = False;
    int tableStart = 0, tableEnd = 0;

    switch( commandLetter )  {
        case 'S':
            tableStart = 0;
            tableEnd = 1;
            break;

        case 'A':
            tableStart = 0;
            tableEnd = 1;
            break;

        case 'P':
            tableStart = 2;
            tableEnd = 2;
            break;

        case 'M':
            tableStart = 3;
            tableEnd = 4;
            break;

        case 'I':
            tableStart = 5;
            tableEnd = 6;
            break;

        case 'O':
            tableStart = 7;
            tableEnd = 9;
            break;

        default :
            printf( "Bad command letter: %c\n", commandLetter ) ;
    }
    valid = lookUp( commandLookUp, opString, tableStart, tableEnd );

    if ( valid == True )
    {
        valid = checkInt( cycleTime ) ;
    }

    return valid;
}

//
// lookUp Function Implementation ///////////////////////////////////
//

/**
 * @brief This function checks a given string against a given table
 *
 * @details This function checks a string with each record in the table
 *          from tableStart to tableEnd to see if it is part of that table
 *
 * @pre table an array of string pointers to other strings
 *
 * @pre string a string pointer
 *
 * @pre tableStart an integer that represents the start of the search table
 *
 * @pre tableSize an integer that represents the end of the search table
 *
 * @post all parameters remain the same at post
 *
 * @par Algorithm
 *      The algorithm loops through every record checking if the given
 *      string is in the table.
 *
 * @exception notInTable
 *            If the string is not in the given table the function
 *            returns False
 *
 * @param[in] table an array of string pointers
 *
 * @param[in] string pointer to a string
 *
 * @param[in] tableStart an integer that represents the start
 *            of the search table
 *
 * @param[in] tableSize an integer that represents the end
 *            of the search table
 *
 * @return success of method operation (bool)
 */
Boolean lookUp( const char *table[10], char *string, int tableStart, int tableEnd )
{
    int iterator;

    for( iterator = tableStart ; iterator <= tableEnd ; iterator++ )
    {
          if( cmpExpected( string, table[iterator] ) == True )
          {
              return True;
          }
    }

    return False;
}
//
// createNode Function Implementation ///////////////////////////////////
//

/**
 * @brief This function creates a new MetaNode struct
 *
 * @details Given three parameters, this function constructs a new MetaNode
 *          struct
 *
 * @pre commandLetter a character
 *
 * @pre opString a string pointer
 *
 * @pre cycleTimea an integer
 *
 * @post all parameters remain the same at post
 *
 * @par Algorithm
 *      The algoithm creates a struct, places the parameters into the
 *      appropriate places into the struct, then returns said struct.
 *
 * @param[in] commandletter a character command letter
 *
 * @param[in] opString a string pointer to an operation string
 *
 * @param[in] cycleTime an int that prepresents cycle time
 *
 * @return a newly constructed MetaNode (MetaNode *)
 */
MetaNode *createNode( char commandLetter, char *opString, int cycleTime )
{
    MetaNode *newNode = malloc( sizeof( *newNode ) );
    newNode->commandLetter = commandLetter;
    newNode->opString = opString;
    newNode->cycleTime = cycleTime;
    newNode->nextNode = NULL;
    newNode->previousNode = NULL;
    return newNode;
}
//
// rewindList Function Implementation ///////////////////////////////////
//

/**
 * @brief This function rewinds a given MetaNode linked list
 *
 * @details This function checks if the given node is null, if it's not
 *          it rewinds a MetaNode linked list all the way to the head of
 *          the list.
 *
 * @pre end a MetaNode pointer
 *
 * @post end is unchanged
 *
 * @exception nullPointer
 *            If the given node is null, the function immediately
 *            returns the original null node
 *
 * @param[in] end a pointer to MetaNode struct
 *
 * @return the head of the MetaNode linked list (MetaNode *)
 */
MetaNode *rewindList( MetaNode *node )
{
    MetaNode *next;
    MetaNode *current;

    if( node == NULL )
    {
        return node;
    }

    next = node->previousNode;
    current = node;

    while( next )
    {
        current = next;
        next = next->previousNode;
    }

    return current;
}
//
// deallocatList Function Implementation ///////////////////////////////////
//

/**
 * @brief This function free a MetaNode linked list
 *
 * @details This function checks if the given node is null, if it's not
 *          it frees all MetaNodes in the linked list from the head down.
 *
 * @pre start a MetaNode pointer
 *
 * @post start is free and becomes a null pointer
 *
 * @par Algorithm
 *      The algorithm starts at the head then progressively frees all the
 *      nodes in the list as it decends to the tail.
 *
 * @param[in] start a pointer to a MetaNode struct
 *
 * @return success of method operation (bool)
 */
Boolean freeList( MetaNode *start )
{
    MetaNode *current;

    while( start != NULL )
    {
        current = start;
        start = start->nextNode;
        freeNode( current ) ;
    }

    return True;
}
//
// freeNode Function Implementation ///////////////////////////////////
//

/**
 * @brief This function frees a MetaNode
 *
 * @details This function frees a MetaNode and all of its assocaited
 *          allocated components.
 *
 * @pre node a MetaNode pointer
 *
 * @post node becomes a null pointer
 *
 * @param[in] node a pointer to a MetaNode struct
 *
 * @return success of method operation (bool)
 */
Boolean freeNode( MetaNode *node )
{
    if( node == NULL)
    {
        return True;
    }

    free( node->opString ) ;
    free( node ) ;
    return True;
}
//
// addNode Function Implementation ///////////////////////////////////
//

/**
 * @brief This fucntion adds a node
 *
 * @details more detailed func desc
 *
 * @pre head a MetaNode pointer that is the head of a MetaNode linked list
 *
 * @pre node a MetaNode pointer
 *
 * @post head does not change at post
 *
 * @post node does not change at post
 *
 * @par Algorithm
 *      The algoithm first checks if the node to be added is null. If so,
 *      return the head.
 *
 * @par Algorithm
 *      The algoithm then checks if the head is NOT null, if it is, node is
 *      the new head.
 *
 * @par Algorithm
 *      The algoithm then traverses the nodes until it hits the last node.
 *      This is where the node is appended and the head is returned.
 *
 * @exception nullPointer
 *            The node to add is null, immediately return the head
 *
 * @exception nullPointer
 *            The head is null, immediately return the node
 *
 * @param[in] head a MetaNode pointer
 *
 * @param[in] node a MetaNode pointers
 *
 * @return a MetaNode Pointer pointing to the head node (MetaNode *)
 */
MetaNode *addNode( MetaNode *head, MetaNode *node )
{
    MetaNode *tempNode;

    if( node == NULL )
    {
        return head;
    }

    tempNode = head;

    if( head != NULL )
    {
        while( tempNode->nextNode != NULL )
        {
            tempNode = tempNode->nextNode;
        }

        tempNode->nextNode = node;
        node->previousNode = tempNode;
        return head;
    }

    return node;
}
