// Memory Implementation File Information ///////////////////////////////
/**
* @file memory.c
*
* @brief Implementation for memory
*
* @details Implements all functions for allocating, accessing
*          and deallocating memory
*
* @version 1.00
* C.S student (26 March 2018)
* Initial creation of memory
*
* @note Requires memory.h
*/

#include "memory.h"

//
// createMMU Function Implementation ///////////////////////////////////
//
/**
 * @brief This function creates a new memory management unit (MMU)
 *
 * @details This function takes in a given max memory size, mallocs the space
 *          for a new memoryManagementUnit, sets the inital values, then
 *          returns said unit.
 *
 * @pre memSize integer the is the total memory for the unit
 *
 * @post memSize remains unchanged at post
 *
 * @param[in] memSize integer the is the total memory for the unit
 *
 * @return a newly initialized MMU (MMU *)
 */
MMU *createMMU( int memSize )
{
    MMU *memoryManagementUnit = malloc( sizeof( *memoryManagementUnit ) );

    memoryManagementUnit->totalMemory = memSize;
    memoryManagementUnit->memAvailable = memSize;
    memoryManagementUnit->headMemory = NULL;

    return memoryManagementUnit;
}
//
// createMemory Function Implementation ///////////////////////////////////
//
/**
 * @brief This function processes a MetaNode into a memory struct
 *
 * @details This function processes a MetaNode that contains a memory operation
 *          into a given memory struct
 *
 * @pre memoryNode a pointer to a MetaNode struct
 *
 * @pre memory a pointer to a malloced memory struct
 *
 * @pre processesId integer value of a process
 *
 * @post memoryNode and processesId remain unchanged at post
 *
 * @post memory now contains values found in memoryNode
 *
 * @param[in] memoryNode a pointer to a MetaNode struct
 *
 * @param[out] memory a pointer to a malloced memory struct
 *
 * @param[in] processesId integer value of a process
 *
 * @return nothing to return (void)
 */
void createMemory( MetaNode *memoryNode, Memory *memory, int processesId )
{
    int segment, start, offset, initial = memoryNode->cycleTime;

    segment = initial / 1000000;
    start = ( initial / 1000 ) % 1000;
    offset = initial % 1000;

    memory->segment = segment;
    memory->startPosition = start;
    memory->offset = offset;
    memory->totalSize = (start + offset) - start;
    memory->processesId = processesId;
    memory->nextNode = NULL;
}
//
// access Function Implementation ///////////////////////////////////
//
/**
 * @brief This function accesses a given position in memory
 *
 * @details This function accesses a given position in the MMU memory node
 *          string by checking each existing segment for the same processid,
 *          segment, and that the area being accessed is allocated
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @pre memoryNode a pointer to a memory struct
 *
 * @post memoryManagementUnit and memoryNode remain unchanged at post
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @param[in] memoryNode a pointer to a memory struct
 *
 * @return Code for accessing status (int)
 */
int access( MMU *memoryManagementUnit, Memory *memoryNode )
{
  Memory *headMemory = memoryManagementUnit->headMemory;
  int headLow, headHigh, nodeLow, nodeHigh;

  while( headMemory != NULL )
  {
      headLow = headMemory->startPosition;
      headHigh = headMemory->startPosition + headMemory->offset;

      nodeLow = memoryNode->startPosition;
      nodeHigh = memoryNode->startPosition + memoryNode->offset;

      if( headLow <= nodeLow
       && nodeHigh <= headHigh
       && headMemory->segment == memoryNode->segment
       && headMemory->processesId == memoryNode->processesId )
      {
          return 0;
      }

      headMemory = headMemory->nextNode;
  }
  return 2;
}
//
// allocate Function Implementation ///////////////////////////////////
//
/**
 * @brief This function allocates a given memory struct
 *
 * @details This function allocates a given memory struct in the MMU memory
 *          linked list. The memory is allocated if the spaces exists, and
 *          the area being allocated is not already allocated to a process
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @pre memoryNode a pointer to a memory struct
 *
 * @post memoryManagementUnit and memoryNode remain unchanged at post
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @param[in] memoryNode a pointer to a memory struct
 *
 * @return Code for allocate status (int)
 */
int allocate( MMU *memoryManagementUnit, Memory *memoryNode )
{
    Boolean conflictCheck;
    int newMemAvailable = memoryManagementUnit->memAvailable - memoryNode->totalSize;
    Memory *headMemory = memoryManagementUnit->headMemory;

    if( newMemAvailable < 0 )
    {
        return 2;
    }

    if( headMemory == NULL )
    {
        memoryManagementUnit->headMemory = memoryNode;
        memoryManagementUnit->memAvailable = newMemAvailable;
        return 0;
    }

    while( headMemory->nextNode != NULL )
    {
        conflictCheck = checkOverlap(headMemory, memoryNode);

        if( conflictCheck == 1 )
        {
            return 2;
        }

        headMemory = headMemory->nextNode;
    }

    conflictCheck = checkOverlap(headMemory, memoryNode);

    if( conflictCheck == 1 )
    {
        return 2;
    }

    headMemory->nextNode = memoryNode;
    memoryManagementUnit->memAvailable = newMemAvailable;

    return 0;
}
//
// createMMU Function Implementation ///////////////////////////////////
//
/**
 * @brief This function deallocates all allocations associated with a given
 *        processid
 *
 * @details This function deallocates all memory segments that are associated
 *          with a given processid and updates the segment list of the MMU
 *          appropriately.
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @pre processesId integer assocated with a process
 *
 * @post memoryManagementUnit and processesId remain unchanged at post
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit
 *
 * @param[in] processesId integer assocated with a process
 *
 * @return nothin to return (void)
 */
void deallocate(MMU *memoryManagementUnit, int processesId)
{
    int mmuAvailableMemory = memoryManagementUnit->memAvailable;
    int totalHeadMemory;
    Memory *headMemory = memoryManagementUnit->headMemory;
    Memory *previous = NULL, *next = NULL, *temp = NULL;

    while( headMemory != NULL )
    {
        next = headMemory->nextNode;

        if(headMemory->processesId == processesId)
        {
            totalHeadMemory = headMemory->totalSize;
            memoryManagementUnit->memAvailable = mmuAvailableMemory + totalHeadMemory;
            temp = headMemory;

            if( previous != NULL )
            {
                headMemory = next;
                previous->nextNode = next;
            }
            else if( previous == NULL && next != NULL )
            {
                headMemory = next;
                next = headMemory->nextNode;
                memoryManagementUnit->headMemory = headMemory;
            }
            else
            {
                headMemory = NULL;
                memoryManagementUnit->headMemory = headMemory;
            }

            temp->nextNode = NULL;
            free( temp );

        }
        else
        {
            previous = headMemory;
            headMemory = headMemory->nextNode;

            if(headMemory != NULL)
            {
                next = headMemory->nextNode;
            }
            else
            {
                next = NULL;
            }

        }
    }
}
//
// checkOverlap Function Implementation ///////////////////////////////////
//
/**
 * @brief This function checks if two memory structs overlap.
 *
 * @details This function checks if two memory structs overlap, returning
 *          True if they do overlap in any fashion and false if they do not.
 *
 * @pre headMemory a pointer to a memory struct
 *
 * @pre memoryNode a pointer to a memory struct
 *
 * @post headMemory and memoryNode remain unchanged at post
 *
 * @param[in] headMemory a pointer to a memory struct
 *
 * @param[in] memoryNode a pointer to a memory struct
 *
 * @return boolean for overlap of the two memory structs (bool)
 */
Boolean checkOverlap( Memory *headMemory, Memory *memoryNode )
{
    int headLow, headHigh, nodeLow, nodeHigh;
    headLow = headMemory->startPosition;
    headHigh = headMemory->startPosition + headMemory->offset;

    nodeLow = memoryNode->startPosition;
    nodeHigh = memoryNode->startPosition + memoryNode->offset;

    if( headLow <= nodeLow && nodeLow <= headHigh )
    {
        return True;
    }
    else if( headLow <= nodeHigh && nodeHigh <= headHigh )
    {
        return True;
    }
    else if( nodeLow <= headLow && headHigh <= nodeHigh )
    {
        return True;
    }
    else
    {
        return False;
    }
}
//
// displayMemory Function Implementation ///////////////////////////////////
//
/**
 * @brief Displays a given memory struct.
 *
 * @details Prints the segment, offset, start, size, and processid for a given
 *          memory struct.
 *
 * @pre memoryNode a pointer to a memory struct
 *
 * @post memoryNode remains unchanged at post
 *
 * @param[in] memoryNode a pointer to a memory struct
 *
 * @return nothing to return (void)
 */
void displayMemory( Memory *memoryNode )
{
    if(memoryNode != NULL)
    {
        printf("Segment: %d\nOffset: %d\nStart: %d\nSize: %d\nProcessId: %d\n",
               memoryNode->segment,
               memoryNode->offset,
               memoryNode->startPosition,
               memoryNode->totalSize,
               memoryNode->processesId);
    }
}
//
// displayMemoryBlock Function Implementation ///////////////////////////////////
//
/**
 * @brief Displays a given MMU segment linked list.
 *
 * @details Displays each memory struct in a linked list held by the MMU.
 *
 * @pre memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @post memoryManagementUnit remains unchanged at post
 *
 * @param[in] memoryManagementUnit a pointer to a memoryManagementUnit struct
 *
 * @return nothing to return (void)
 */
void displayMemoryBlock( MMU *memoryManagementUnit )
{
    Memory *headMemory = memoryManagementUnit->headMemory;

    printf("=====MEMORY DUMP=====\n");
    printf("Memory Available: %d\n", memoryManagementUnit->memAvailable);

    while ( headMemory != NULL )
    {
        displayMemory( headMemory );
        headMemory = headMemory->nextNode;
    }
}
