#include "maintest.h"

int main( int argc, char *argv[] )
{
    ConfigData *cfData = NULL;
    MetaNode *currentNode = NULL;
    StringNode *startNode = NULL;

    if( argc < 2 )
    {
      printf( "No config file given.\n" );
      return EXIT_FAILURE;
    }

    cfData = getConfig( argv[argc - 1] );

    if( cfData != NULL )
    {
        currentNode = getMetadata( cfData );
    }
    else
    {
        printf( "Bad config file: %s\n", argv[argc - 1] );
        return EXIT_FAILURE;
    }

    if( currentNode == NULL )
    {
        printf( "Bad metadata file: %s\n", cfData->filePath );
        freeConfig( cfData );
        return EXIT_FAILURE;
    }

    startNode = beginProcessing(currentNode, cfData);

    if( stringCmp(cfData->logTo, "File") == True
     || stringCmp(cfData->logTo, "Both") == True )
    {
      writeToLog( startNode, cfData->logFilePath );
    }

    freeStringList( startNode );
    freeList( currentNode );
    freeConfig( cfData );

    return EXIT_SUCCESS;
}
