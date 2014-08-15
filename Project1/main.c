#include<stdio.h>
#include "assemblerTypes.h"
#include <string.h>

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

void main(int argc, char **argv)
{
	int i;
	
	initPrgram("assembler");





	


	for (i = 1; i < argc; i++){
		/* TODO remove */
		/* ("argv[%d]=%s\n", i,argv[i]);  */

		processSingleFile(argv[i]);
	}
	
}