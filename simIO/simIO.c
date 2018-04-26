// StringUtils Implementation File Information ///////////////////////////////
/**
* @file simIO.c
*
* @brief Implementation for simIO
*
* @details Implements all functions for high level input of the configuration
* file and the metadata file
*
* @version 1.00
* C.S student (25 February 2018)
* Initial creation of simIO
*
* @note Requires simIO.h
*/
#include "simIO.h"

//
// getConfig Function Implementation ///////////////////////////////////
//

/**
 * @brief This function reads a configuration file in as a ConfigData struct
 *
 * @details This function reads data in from a configuration file
 *          parses it for potential errors and returns a ConfigData struct on
 *          success
 *
 * @pre fileName a pointer to a string containing a files name
 *
 * @post fileName remains unchanged at post
 *
 * @param[in] fileName a pointer to a string
 *
 * @return a pointer to a newly create ConfigData struct or NULL
 *         on failure (ConfigData *)
 */
ConfigData *getConfig( char *fileName)
{
    FILE *configFilePointer;
    configFilePointer = fopen( fileName, "r" );   // Try to open the config file

    if( configFilePointer == NULL )
    {
          printf("Unable to open config file.\n");
          return NULL;
    }

    ConfigData *cfData = readConfig( configFilePointer );    // Parse the config file
    fclose( configFilePointer );

    if( cfData == NULL )
    {
          return NULL;
    }                                     // Display the config file

    return cfData;
}
//
// getMetadata Function Implementation ///////////////////////////////////
//

/**
 * @brief This function reads a metadata file in as a linked list of MetaNode
 *        structs
 *
 * @details This function reads data in from a metadata file
 *          parses it for potential errors and returns a linked list of MetaNode
 *          structs on success
 *
 * @pre cfData a pointer to a ConfigData struct
 *
 * @post cfData remains unchanged at post
 *
 * @param[in] cfData a pointer to a ConfigData struct
 *
 * @return a pointer to a newly create Linked List
 *         MetaNode struct or NULL on failure (MetaNode *)
 */
MetaNode *getMetadata( ConfigData *cfData )
{
    FILE *metadataFilePointer;
    metadataFilePointer = fopen( cfData->filePath, "r" );     // Try to open the metadata file

    if( metadataFilePointer == NULL )
    {
        printf( "Unable to open metadata file.\n" );
        return NULL;
    }

    MetaNode *startNode = readMetadata(metadataFilePointer);  // Parse the metadata
    fclose( metadataFilePointer );

    if (startNode == NULL)
    {
        return NULL;
    }
    
    return startNode;
}
