#include "assemblerTypes.h"
#include <stdio.h>
#include <string.h>

static FILE *ifd, *ofd, *exfd, *enfd;
char fileBaseName[100];

/*open outout files */
int initOutput(){
	char ofile[100];
	char exfile[100];
	char enfile[100];


	strcpy(ofile, fileBaseName);
	strcpy(exfile, fileBaseName);
	strcpy(enfile, fileBaseName);

	ofd = fopen(strcat(ofile, OBJ_SUFFIX), "w+");
	if (!ofd)
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "couldn't open file <%s>\n", ofile);
		return reportError(msg, FATAL);
	}


printf("fopen file [%s]\n",ofile);
	exfd = fopen(strcat(exfile, EXT_SUFFIX), "w+");
	if (!exfd)
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "couldn't open file <%s>\n", exfile);
		return reportError(msg, FATAL);
	}

printf("fopen file [%s]\n",exfile);
	enfd = fopen(strcat(enfile, ENT_SUFFIX), "w+");
	if (!enfd)
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "couldn't open file <%s>\n", enfile);
		return reportError(msg, FATAL);
	}

printf("fopen file [%s]\n",enfile);
	return NORMAL;
}



/*
File handling module. Includes all file open close, read and write utilities
*/
int initForFile(const char * inputFile)
{
	char infile[100];


	strcpy(infile, inputFile);
	strcpy(fileBaseName, inputFile);
	initFile("infile");
	ifd = fopen(strcat(infile, INP_SUFFIX), "r");
	if (!ifd)
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "couldn't open file <%s>\n", infile);
		return reportError(msg, FATAL);
	}
}


/*
read a single line from input file
the return string is passed to calling function in the argument
Calling function must free memory after use
*/
int readLine(char **line){
	int status;
	*line = (char*)malloc(MAX_ROW_SIZE+1);
	if (!(*line))
		return reportError("Fatal! Unable to allocate memory\n", FATAL);

	status = fgets(*line, MAX_ROW_SIZE, ifd);

	
	if (!status)
		return END;
	
	/* remove \n at end of line and make sure no string functions use that char*/
	/*  (*line)[strlen(*line) - 1] = 0; */
	/* increment line count in error handler */
	incrementLine();
	return NORMAL;																				
}

/* rewind the input file */
void rewindInputFile(){
	rewind(ifd);
}

int writeObjLine(char *line){
	return writeLine(line, ofd);

}
int writeExtLine(char *line){
	return writeLine(line, exfd);

}
int writeEntLine(char *line){
	return writeLine(line, enfd);

}
int writeLine(char *line, FILE  *fileDesc){
	int status;
	FILE *fd;


	if (EOF==fputs(line, fileDesc))
		return reportError("Fatal! Unable to write to output file", FATAL);

	fputs("\n", fileDesc);
	return NORMAL;
}

void closeFiles(){
	fclose(ifd);
	fclose(ofd);
	fclose(enfd);
	fclose(exfd);

}
