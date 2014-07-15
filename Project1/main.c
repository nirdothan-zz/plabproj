#include<stdio.h>
#include "assemblerTypes.h"


void processSingleFile(char *inputFile)
{
	//printf("file: %s\n", inputFile);
	initFiles(inputFile);
}

void main(int argc, char **argv)
{
	int i;

	

	for (i = 1; i < argc; i++){
		printf("argv[%d]=%s\n", i,argv[i]);
		processSingleFile(argv[i]);
	}
	//printf(" opcode %d\n", getDecOpcode("jms"));
	//parseRow("hi");
	
	//firstPass();
}