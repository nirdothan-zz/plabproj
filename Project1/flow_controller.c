#include "assemblerTypes.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

Word_t programSegment[MAX_PC];


int firstPass(char *inputFile){
	int status;
	char *row;

	/* open input file for reading*/
	if (ERROR == initForFile(inputFile))
		return ERROR;


	/* process one row at a time*/
	while (END != readLine(&row)){
		parseRow(row);
		free(row);

	}

	



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