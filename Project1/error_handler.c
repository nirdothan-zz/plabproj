#include <stdio.h>
#include <string.h>
#include "assemblerTypes.h"
/*

Simple STDOUT error handler
Implementation prints out error messages to stfout as they are reported
*/
static int lineNo,pass, severity;
static char progName[100]={ "N/A" }, inFileName[100] = { "N/A" };


/*
Call this function to initalize the error handler
provide the calling program name as prog paframeter 
*/
void initPrgram(char *prog){
	strcpy(progName, prog);
	/* theoretically we would want to initalize all counters here
	but since they're global they are initialized */
}


/*
Call this function when the main program starts processing a new row
from input file.
specify row number in line parameter
*/
void initLine(int line){
	lineNo = line;
}

/*
Call this function when the main program starts processing a new row
from input file
function increments line from the previous
*/
void incrementLine(){
	lineNo++;
}

/*
Call this function when the main program starts processing a 
new input file
spscify file name in file parameter
*/
void initFile(char *file){

	strcpy(inFileName, file);

}

/*
Call this function when the an error occurs,
Provide the sprcific error message in the errorMsg param
provide the error severity using the enum in sev argument
*/
int reportError(char *errorMsg, int sev ){

	/* update gloibal severity only if it is worse than current severity*/
	if (sev > severity)
		severity = sev;

	printf("[%s] [file: %s] [line: %d] <%s>\n",progName, inFileName, lineNo, errorMsg);
	
	return sev;
}

/*
get the global severity level that the program is at
Can be used as an indication of abort
*/

int getSeverity(){
	return severity;
}
/*
When implemented as a stack, this method would flush the stack 
Since not required by MMN#14 specs, not implemeneted
*/
void flush(){


}