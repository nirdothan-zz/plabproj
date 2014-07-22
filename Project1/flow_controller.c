/*
This module is the main flow controller which follows the algorithm 
depicted in page 28 of the course booklet
*/
#include "assemblerTypes.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Word_t g_programSegment[MAX_PC];	/* global Program Segment */
int g_IC = INIT_IC;					/* global instructions counter */
int g_DC = INIT_DC;					/* global data counter */
Symbol_t *g_symbolTable;			/* global symbol table */
int g_symbolTableSize=0;			/* global symbol table current size */
static char labelFlag = 0;			/* module level label idenfification flag */
static char isDatainstructionFlag = 0;	/* module level data instruction idenfification flag */

int parseRowFirst(const char *);

int firstPass(char *inputFile){
	int status;
	char *row;

	g_symbolTable = (Symbol_t*)malloc(sizeof(Symbol_t) * MAX_SYMBOLS);
	if (!g_symbolTable)
		return reportError("Fatal! Unable to allocate memory\n", FATAL);


	/* open input file for reading*/
	if (ERROR == initForFile(inputFile))
		return ERROR;


	/* process one row at a time*/
	while (END != readLine(&row)){
		status = parseRowFirst(row);
		free(row);
		if (FATAL == status)
			return FATAL;
		
	}

	



}

int secondPass()
{



	free(g_symbolTable);
}

int dummytests(){
	Fields_t fields;
	Word_t w;
	int a = pow(2.0,19.0)-1;

	memset(&fields, 0, sizeof(fields));
	fields.comb = 3;
	fields.target_addr = 3;
	fields.src_addr = 3;
		fields.type = 1;
		fields.dbl = 1;


	print20LSBs(&fields);


	mapword(w, a);
	print20LSBs(w);
	print20LSBs(&a);
	printHEX(w);
	printf("%d");

}

/* row level parsing activities for first pass */
int parseRowFirst(const char *row){
	int status;
	char label[MAX_LABEL_SIZE + 1];
	printf("row is <%s>\n", row);

	if (isCommentOrEmpty(row))
		return NORMAL;


	/* 
	step #3-4 on p 28 
	if label is found the labelFlag is set to 1
	*/
	if ((status = parseLabel(&row, &labelFlag, label)) != NORMAL)
		return status;


	/*
	step #5 on p 28
	if data instruction is found the isDatainstructionFlag is set to 1
	*/
	if ((status = parseInstruction(&row, &isDatainstructionFlag)) != NORMAL )
		return status;

	/* 	step #6 on p 28 */
	if (labelFlag && isDatainstructionFlag) {
		
		strcpy((g_symbolTable[g_symbolTableSize]).label, label);
		(g_symbolTable[g_symbolTableSize]).decimal = g_DC;
		(g_symbolTable[g_symbolTableSize]).octal = getOctal(g_DC);
		(g_symbolTable[g_symbolTableSize]).type = DATA_LABEL;
		g_symbolTableSize++;

	}

	return NORMAL;

}
