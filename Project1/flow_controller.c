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
Word_t g_dataSegment[MAX_PC];		/* global Data Segment */
int g_IC = INIT_IC;					/* global instructions counter */
int g_DC = INIT_DC;					/* global data counter */
Symbol_t *g_symbolTable;			/* global symbol table */
int g_symbolTableSize=0;			/* global symbol table current size */
Symbol_t *g_externalSymbolTable;			/* global symbol table */
int g_externalSymbolTableSize = 0;			/* global symbol table current size */
static char labelFlag = 0;			/* module level label idenfification flag */


int parseRowFirst(const char *);
int insertLabel(char *, int);

int firstPass(char *inputFile){
	int status;
	char *row=NULL;

	g_symbolTable = (Symbol_t*)malloc(sizeof(Symbol_t) * MAX_SYMBOLS);
	if (!g_symbolTable)
		return reportError("Fatal! Unable to allocate memory\n", FATAL);

	g_externalSymbolTable = (Symbol_t*)malloc(sizeof(Symbol_t) * MAX_SYMBOLS);
	if (!g_externalSymbolTable)
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
	free(g_externalSymbolTable);
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


}

/* row level parsing activities for first pass */
int parseRowFirst(const char *row){
	int status;
	char label[MAX_LABEL_SIZE + 1];
	int instructionFlag = 0;	 /*  instruction idenfification flag */

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
	step #5 and #8 on p 28
	if data instruction is found the instructionFlag is set per enum
	*/
	if ((status = parseInstruction(&row, &instructionFlag)) != NORMAL)
		return status;



	/* 	step #6 on p 28 */
	if (labelFlag && 
		( DATA_INSTRUCTION_FLAG == instructionFlag ||
		  STRING_INSTRUCTION_FLAG == instructionFlag))
			return insertLabel(label, DATA_LABEL);

	/* 	step #9 on p 28 */
	if (EXTERN_INSTRUCTION_FLAG == instructionFlag)
		return insertExtrenalLabel(row);

	

	return NORMAL;

}


/* insert a label into the label table */
int insertLabel(char *label, int type)
{
	/* make sure no memory overflow */
	if (MAX_SYMBOLS == g_symbolTableSize)
		return reportError("Error! Maximum # of labels exceeded\n", FATAL);

	strcpy((g_symbolTable[g_symbolTableSize]).label, label);
	(g_symbolTable[g_symbolTableSize]).decimal = g_DC;
	(g_symbolTable[g_symbolTableSize]).octal = getOctal(g_DC);
	(g_symbolTable[g_symbolTableSize]).type = type;
	g_symbolTableSize++;

	return NORMAL;
}


/* insert an external label into the the external label table, with 0 address */
insertExtrenalLabel(char *row)
{
	char label[MAX_LABEL_SIZE + 1];
	/* make sure no memory overflow */
	if (MAX_SYMBOLS == g_externalSymbolTableSize)
		return reportError("Fatal! Maximum # of Extrenal labels exceeded\n", FATAL);
	if (sscanf(row, "%s",label) != 1 )
		return  reportError("Error! Illegal external instruction\n", ERROR);

	strcpy((g_externalSymbolTable[g_externalSymbolTableSize]).label, label);
	(g_externalSymbolTable[g_externalSymbolTableSize]).decimal = 0; /*init*/
	(g_externalSymbolTable[g_externalSymbolTableSize]).octal = 0; /*init*/
	(g_externalSymbolTable[g_externalSymbolTableSize]).type = EXT_LABEL;
	g_externalSymbolTableSize++;

	return NORMAL;
}