#include<stdio.h>
#include "assemblerTypes.h"
#include <string.h>

void processSingleFile(char *inputFile)
{
	printf("file: %s\n", inputFile);
	firstPass(inputFile);
	secondPass();
}

void main(int argc, char **argv)
{
	int i;
	initPrgram("assembler");

	

//	return;
	for (i = 1; i < argc; i++){
		//printf("argv[%d]=%s\n", i,argv[i]);

		processSingleFile(argv[i]);
	}
	//printf(" opcode %d\n", getDecOpcode("jms"));
	//parseRow("hi");
	
	//firstPass();
}