#include<stdio.h>
#include "assemblerTypes.h"
#include <string.h>

/*
main program module
responsible for traversing input files and feeding them to the processing modules
*/


/*run the main flow - per input file*/
int processSingleFile(char *inputFile)
{
	int status, outputFlag=1;
	printf("file: %s\n", inputFile);
	if ((status=firstPass(inputFile)) == FATAL)
		return FATAL;
	if (status != NORMAL)
		outputFlag = 0;
	updateDataSegemnt();
	if ((status = secondPass()) == FATAL)
		return FATAL;
	if (status != NORMAL)
		outputFlag = 0;
	if (outputFlag)
		writeOutput();
	cleanup();
}

/* entry point into the program*/
void main(int argc, char **argv)
{
	int i;
	
	/* inialize the error handler with program name */
	initPrgram("assembler");

	/* traverse input files*/
	for (i = 1; i < argc; i++){
	
		processSingleFile(argv[i]);
	}
	
}