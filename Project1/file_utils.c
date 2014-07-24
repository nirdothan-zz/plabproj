#include "assemblerTypes.h"
#include <stdio.h>
#include <string.h>

FILE *ifd, *ofd, *exfd, *enfd;

/*
File handling module. Includes all file open close, read and write utilities
*/
int initForFile(const char * inputFile)
{
	char infile[100];
	
	strcpy(infile, inputFile);
	initFile("infile");
	ifd = fopen(strcat(infile, INP_SUFFIX), "r");
	if (!ifd)
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "couldn't open file <%s>\n", infile);

		return reportError(msg, ERROR);
	}




}


/*
read a single line from input file
the return string is passed to calling function in the argument
Calling function must free memory after use
*/
int readLine(char **line){
	int status;
	*line = (char*)malloc(MSG_MAX_SIZE);
	if (!(*line))
		return reportError("Fatal! Unable to allocate memory\n", FATAL);

	status=fgets(*line, MSG_MAX_SIZE, ifd);

	/*if (!status){
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! could not read line from file <%s>\n", ifd->_tmpfname);

		return reportError(msg, ERROR);

	}*/

	if (!status)
		return END;
	
	/* remove \n at end of line and make sure no string functions use that char*/
	/*  (*line)[strlen(*line) - 1] = 0; */
	/* increment line count in error handler */
	incrementLine();
	return NORMAL;																				
}