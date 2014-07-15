#include "assemblerTypes.h"
#include <stdio.h>
#include <string.h>

FILE *ifd, *ofd, *exfd, *enfd;

/*
File handling module. Includes all file open close, read and write utilities
*/
int initFiles(char * inputFile)
{
	int status;
	char infile[100];
	

	

	strcpy(infile, inputFile);
	ifd = fopen(strcat(infile, INP_SUFFIX), "r");
	if (!ifd)
	{
		printf("couldn't open file <%s>\n", strcat(infile, INP_SUFFIX));
	}

	




}