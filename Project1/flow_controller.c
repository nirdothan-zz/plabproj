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
Symbol_t *g_externalTable;			/* global external table */
int g_externalTableSize = 0;			/* global external table current size */
Symbol_t *g_entryTable;			/* global entry table */
int g_entryTableSize = 0;			/* global entry table current size */

static char labelFlag = 0;			/* module level label idenfification flag */


int parseRowFirst(const char *);
int parseRowSecond(const char *);


int firstPass(char *inputFile){
	int status;
	char *row=NULL;

	g_symbolTable = (Symbol_t*)malloc(sizeof(Symbol_t) * MAX_SYMBOLS);
	if (!g_symbolTable)
		return reportError("Fatal! Unable to allocate memory\n", FATAL);
	g_entryTable = (Symbol_t*)malloc(sizeof(Symbol_t) * MAX_SYMBOLS);
	if (!g_entryTable)
		return reportError("Fatal! Unable to allocate memory\n", FATAL);
	g_externalTable = (Symbol_t*)malloc(sizeof(Symbol_t) * MAX_SYMBOLS);
	if (!g_entryTable)
		return reportError("Fatal! Unable to allocate memory\n", FATAL);



	/* open input file for reading*/
	if (FATAL == initForFile(inputFile))
		return FATAL;


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
	int status;
	char *row = NULL;

	int lastIC = g_IC;
	/* 	step #1 on p 28	*/
	g_IC = 0;

	rewindInputFile();

	/* process one row at a time*/
	while (END != readLine(&row)){
		status = parseRowSecond(row);
		free(row);
		if (FATAL == status)
			return FATAL;

	}



	free(g_symbolTable);
	free(g_entryTable);
	free(g_externalTable);

}

int dummytests(){
	Fields_t fields;
	Word_t w,w1;
	int a = pow(2.0,19.0)-1;

	memset(&fields, 0, sizeof(fields));
	//fields.opcode = 6;
	//fields.comb = 3;

//	fields.target_reg = 7;
//fields.target_addr = 3;
fields.src_reg = 7;
	fields.src_addr = 0;
	fields.type = 0;
	fields.dbl = 0;
//	mapword(w1, fields);

	a = 7;
	a <<= 7;
	//mapword(w1, a);
	set_comb(w1, 3);
	print20LSBs(w1);
	



}

/* row level parsing activities for first pass */
int parseRowFirst(const char *row){
	int status, orig_address=0;
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
	if ((status = parseInstruction(&row, &instructionFlag, &orig_address)) != NORMAL)
		return status;



	/* 	step #6 on p 28 */
	if (labelFlag) {
		if (DATA_INSTRUCTION_FLAG == instructionFlag ||
			STRING_INSTRUCTION_FLAG == instructionFlag)
			return insertLabel(label, DATA_LABEL, orig_address);
		else
		{
			
			/*step #11 on p 28 */
			return insertLabel(label, CODE_LABEL, orig_address);
		}
	}
		

	/* 	step #9 on p 28 */
	if (EXTERN_INSTRUCTION_FLAG == instructionFlag){
		char label[MAX_LABEL_SIZE + 1];
		if (sscanf(row, "%s", label) != 1)
			return  reportError("Error! Illegal external instruction\n", ERROR);
		return insertLabel(label, EXT_LABEL, 0);
	}
		
	
	return NORMAL;

}



/* row level parsing activities for first pass */
int parseRowSecond(const char *row){
	int status, orig_address = 0;
	char label[MAX_LABEL_SIZE + 1];
	int instructionFlag = 0;	 /*  instruction idenfification flag */


	printf("row is <%s>\n", row);

	if (isCommentOrEmpty(row))
		return NORMAL;


	/*
	step #2 on p 28 - second pass
	*/
	if ((status = parseLabel(&row, &labelFlag, label)) != NORMAL)
		return status;


	/*
	step #5 and #8 on p 28
	if data instruction is found the instructionFlag is set per enum
	*/
	if ((status = parseInstructionSecondPass(&row, &instructionFlag, &orig_address)) != NORMAL)
		return status;





	return NORMAL;

}



