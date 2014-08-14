#include<string.h>
#include <stdio.h>
#include "assemblerTypes.h"

/* traslate ascii '1' or '0' to 1 or 0*/
#define BINVAL(C) ((C)=='1' ? 1 : 0 )
enum {SRC,DST};
extern Word_t g_dataSegment[];
extern Word_t g_programSegment[];
extern int g_ICWordCount[];
extern int g_ICFlag[];
extern int g_DC;
extern int g_IC;


int isCommentOrEmpty(const char*);
int isValidLabelChar(const char);
int storeString(const char *);
int storeData(const char *);
void calculateAdditionalWords(char*, int);
void incrementICforParam(char *);
int encodeUnaryOpr(char *, char* );
int encodeBinaryOpr(char *, char*);
int encodeNoParamOpr(char *, char*);
int parseDynamicOperand(char *, int *, int *, int );

/* get an operand in dynamic addressing method and return both its indexes*/
int parseDynamicOperand(char *opr, int *o_decAddress, int *o_index, int additionalWords){
	char *p,*p1, op[MAX_LABEL_SIZE+1];
	int index;

	p = strchr(opr, '{');
	if (!p)
			return reportError("Syntax error: invfalid syntax of dynamic addressing operand - missing { char\n",ERROR);
	*p = 0;
	if (sscanf(opr,"%s",op) < 1)
		return reportError("Syntax error: invalid syntax of dynamic addressing operand - missing label\n", ERROR);
	(*o_decAddress) = getSymbolDecimal(op);
	if (KNF == (*o_decAddress)){
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! label [%s] not founf in table\n", op);
		return reportError(msg, ERROR);
	}


	/*check if the symbol is external - if so insert it to the extrenal table */
	if (EXT_LABEL == getSymbolType(op)){
		insertExternalLabel(op, g_IC + additionalWords);
		g_ICFlag[g_IC + additionalWords] = EXTERNAL_ADDRESS;
	}
	else {
		g_ICFlag[g_IC + additionalWords] = RELATIVE_ADDRESS;
	}


	/*advance p passed \0 */
	p++;
	opr = p;
	p = strchr(opr = p, '!');
	if (!p)
		return reportError("Syntax error: invalid syntax of dynamic addressing operand - missing ! char\n", ERROR);
	/*advance passed ! */
	p++;
	opr = p;
	p = strchr(opr, '}');
	if (!p)
		return reportError("Syntax error: invalid syntax of dynamic addressing operand - missing } char\n", ERROR);
	
	/*null terminate*/
	*p= 0;

	if (sscanf(opr, "%s", op)<1)
		return reportError("Syntax error: invalid syntax of dynamic addressing operand - missing index label\n", ERROR);
	index = getSymbolDecimal(op);
	if (KNF == index){
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! index label [%s] not founf in table\n", op);
		return reportError(msg, ERROR);
	}

	/*trace back the real index of the instruction in the IC array*/
	(*o_index) = g_ICWordCount[index];

	return NORMAL;


}
/* check if the row string represents a comment or an empty line */
int isCommentOrEmpty(const char *row){

	char dummy[MSG_MAX_SIZE];

	/* if sscanf returned 0 then all chars were white spaces */
	return (( COMMENT_CHAR == row[0] )|| (1>sscanf(row,"%s", dummy)));

}

/* check that chars are valid ofr label i.e. alphanumeric*/
int isValidLabelChar(const char c){
	return ((c >= 'A' && c < 'z') || (c >= '0' && c <= '9'));
}


/*
 parse the possible label part of the row
 the pointer is moved to after the label if exists 
*/
int parseLabel(const char **row, char *o_labelFlag, char *o_label){


	const int labelIndication = ':';
	int labelLength, i;
	char label[MAX_LABEL_SIZE + 1], *pl = label, *end;

	*o_labelFlag = 0;
	/* find if this row begins with a label*/
	end = strchr(*row, labelIndication);

	/* no label in string */
	if (!end)
		return NORMAL;

	/* length of label must be under MAX_LABEL_SIZE =30 */
	if ((labelLength = (end - *row)) > MAX_LABEL_SIZE){
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! illegal label length =%d\n", labelLength);
		return reportError(msg,ERROR);
	}

	/* copy label chars to label array, only if they're legal */
	for (i = 0; i < labelLength; i++)
		if (isValidLabelChar((*row)[i]))
			*pl++ = (*row)[i];
		else{

			char msg[MSG_MAX_SIZE];
			sprintf(msg, "Error! illegal label character %c\n", ((*row)[i]));
			return reportError(msg, ERROR);

		}
		
		*pl = 0;
				
		strcpy(o_label, label);
		
		/* set label flag on */
		(*o_labelFlag) = 1;

		/* move the pointer till after the label, so that the calling program can processs the rest of the row */
		(*row) += labelLength + 1;

		return NORMAL;
}

/*
parse the possible data instruction part of the row
the pointer is moved to after the instruction if exists
*/ 
int parseInstruction(const char **row, int  *o_labelFlag, int *o_address){
	
	int status;
	char instruction[MAX_LABEL_SIZE];
	char instructionPrefix = '.';
	status = sscanf(*row, "%s", instruction);


	/*init instruction flag to be used as by the calling function */
	(*o_labelFlag) = NO_INST_FLAG;
	if (!status)
		return reportError("Error! Illegal or missing instruction\n", ERROR);

	
	/* is an instruction */
	if (instructionPrefix == instruction[0]){
		
		
		
		/* advance pointer to the '.' char */
		(*row) = strchr(*row, instructionPrefix);
		(*o_address) = g_DC; /* backup g_DC before advancing it */
		/* is a data instruction */
		if (!strcmp(instruction, DATA_INSTRUCTION))
		{
			(*o_labelFlag) = DATA_INSTRUCTION_FLAG;
			/* advance pointer to after the ".data" string */
			(*row) += strlen(DATA_INSTRUCTION);
			return storeData(*row);
		}
		/* is a string instruction */
		else if (!strcmp(instruction, STRING_INSTRUCTION))
		{
			(*o_labelFlag) = STRING_INSTRUCTION_FLAG;
			/* advance pointer to after the ".string" string */
			(*row) += strlen(STRING_INSTRUCTION);
			return storeString(*row);
		}
		/* is an extern instruction */
		else if (!strcmp(instruction, EXTERN_INSTRUCTION))
		{
			(*o_labelFlag) = EXTERN_INSTRUCTION_FLAG;
			/* advance pointer to after the ".extren" string */
			(*row) += strlen(EXTERN_INSTRUCTION);
			return NORMAL;
		}
		/* is an extern instruction */
		else if (!strcmp(instruction, ENTRY_INSTRUCTION))
		{
			(*o_labelFlag) = ENTRY_INSTRUCTION_FLAG;
			/* advance pointer to after the ".entry" string */
			(*row) += strlen(ENTRY_INSTRUCTION);
			return NORMAL;
		}
			
		/* it begins with a '.' but is neither .string nor .data, .entry, .extern */
		else {
			
			return reportError("Error!, illegal instruction\n", ERROR);

		}
	}


	/* not a data instruction */
	else {
		(*o_address) = g_IC; /* backup g_IC before advancing it */
		(*row) = strchr(*row, '/');
		if (!(*row))
			return reportError("Error!, illegal syntax, could not find '/' after opcode \n", ERROR);

		/*increment one word for base row */
		g_ICWordCount[g_IC]++;

		/* calculate the size of the instruction */
		switch (getOpcodeGroup(instruction)){
		case KNF:
			return reportError("Error!, illegal operation code\n", ERROR);
			break;
		case UNARY:
			calculateAdditionalWords(*row, UNARY);
			break;
		case BINARY:
			calculateAdditionalWords(*row, BINARY);
			break;
		case NOPARAMS:
			/* the operation has no additional words */
			break;
		}
	}
	
		/* Increment IC by the number of words that the instruction includes*/
		g_IC += g_ICWordCount[g_IC];

		return NORMAL;
}

/* parse the ".data" instruction and stores its values in the data image */
int storeData(char *data){
	int integer;
	/* sscanf format needs a variable because we add ',' after the first time */
	char format[] = " %d";

	do { 

		if (sscanf(data, format, &integer) != 1){
			return reportError("Error! Ilegal data for .data instruction\n", ERROR);
		}
		/* from now on we look for integers that come after commas*/
		format[0] = ',';


		/* store the integer that followed ".data" in a new row in the global data segment */
		/* data segment rows are 20 bits long, and require mapping from int */
		/* increment data dounter */
		mapword(&(g_dataSegment[g_DC++]), integer);

		/* look for the next number - advance the pointer to the next comma */
		/* advance the pointer to th enext comma */
		data = strchr(++data, ',');
	} while (data);

	return NORMAL;
}

/* parse the ".string" instruction and stores its chars in the data image */
int storeString(char *data){
	char *string;
	int i=0;

	/*get to opening " */
	while (*data == ' ' ||  *data=='\t')
		data++;

	if (*data != '"')
		return reportError("Error! Ilegal string value - could not find opening quotes\n", ERROR);
	else
		data++;

	string = strchr(data, '"');
	if (!string)
		return reportError("Error! Ilegal string value - could not find closing quotes\n", ERROR);

	/*put a null terminator where the closing quotes were*/
	*string = '\0';



	while (data[i]){
			/*map each chars ascii value to a separate row in the data image*/
			mapword(&(g_dataSegment[g_DC++]), data[i++]);
			//TODO remove
			//print20LSBs(&(g_dataSegment[g_DC-1]));
			
		}



	/*apend a null terminator row at the end of the string data image and increment counter*/
	memset((&(g_dataSegment[g_DC++])),0,sizeof(Word_t));
	

	
	return NORMAL;
}

void calculateAdditionalWords(char *row, int paramNo )
{
	char *f1=0, *f2=0, arr[MAX_ROW_SIZE];
	/* get us to the dbl field */
	row = strchr(row, ',');
	if (!row)
	{
		reportError("Error!, illegal syntax, could not find ',' after '/' \n", ERROR);
		return;
	}

	/*pass the dbl filed*/
	row+=2;

	switch (paramNo)
	{
	case UNARY:
		if (1 != sscanf(row, "%s", arr)){
			reportError("Error!, illegal syntax for unary operation, could not find operation param \n", ERROR);
			return;
		}

		incrementICforParam(arr);
		break;
	case BINARY:
		
		
		f1=strtok(row, ",");
		if (!f1)
		{
			reportError("Error!, illegal syntax for binary operation, could not find operation param 1 \n", ERROR);
			return;
		}
		f2=strtok(NULL, ",");
		if (!f2)
		{
			reportError("Error!, illegal syntax for binary operation, could not find operation param 2 \n", ERROR);
			return;
		}
		incrementICforParam(f1);
		incrementICforParam(f2);
		
	}
		
	

}


void incrementICforParam(char *param)
{
	/*advance pointer if white spaces */
	while (*param == ' ' || *param == '\t' )
		param++;

	/* register addressing */
	if (param[0] == 'r' && param[1] >= '0' && param[1] <= '7')
	{
		/* no additional words needed for register addressing method*/
		
		/* dynamic addressing*/
	}
	else if (strchr(param, '{'))
	{
		/*dynamic addressing has 2 additional words*/

		g_ICWordCount[g_IC]+=2;
	}
	/* direct or immediate addressing */
	else {
		g_ICWordCount[g_IC] ++;
	}

}



/*
parse the possible data instruction part of the row
the pointer is moved to after the instruction if exists
*/
int parseInstructionSecondPass(const char **row, int  *o_labelFlag, int *o_address){

	int status;
	char instruction[MAX_LABEL_SIZE];
	char instructionPrefix = '.';
	status = sscanf(*row, "%s", instruction);



	/*init instruction flag to be used as by the calling function */
	(*o_labelFlag) = NO_INST_FLAG;
	if (!status)
		return reportError("Error! Illegal or missing instruction\n", ERROR);


	/* is an instruction */
	if (instructionPrefix == instruction[0]){



		/* advance pointer to the '.' char */
		(*row) = strchr(*row, instructionPrefix);

		/* step #4 - page 28*/
		/* is a data instruction */
		if (!strcmp(instruction, DATA_INSTRUCTION) || !strcmp(instruction, STRING_INSTRUCTION))
			return NORMAL;

		/* step #5-6 - page 28*/
		if (!strcmp(instruction, ENTRY_INSTRUCTION)){
	
			/* advance pointer to after the ".entry" string */
			(*row) += strlen(ENTRY_INSTRUCTION);
			sscanf(*row, "%s", *row);
			int dec = getSymbolDecimal(*row);
			insertLabel(*row, ENT_LABEL, dec);
		}
		return NORMAL;
	}

	/* not a . instruction */
	else {

		trimSlash(instruction, instruction);

		(*row) = strchr(*row, '/');
		if (!(*row))
			return reportError("Error!, illegal syntax, could not find '/' after opcode \n", ERROR);



		/* calculate the size of the instruction */
		switch (getOpcodeGroup(instruction)){
			case KNF:
				return reportError("Error!, illegal operation code\n", ERROR);
				break;
			case UNARY:
				encodeUnaryOpr(*row, instruction);
				break;
			case BINARY:
				encodeBinaryOpr(*row, instruction);
				break;
			case NOPARAMS:
				encodeNoParamOpr(*row, instruction);
				break;
				}
		}

		
		
		g_IC += g_ICWordCount[g_IC];
		
	return NORMAL;
}



/* encode a unary operation */
/* e.g.   clr/1/0,0 r1  */
int encodeUnaryOpr(char *row, char *op){

	
	char  type, dbl,src_opr[MAX_ROW_SIZE], *helper;
	
	int status, addressingMethod, additionalWordsOffset;
	int opcode = getDecOpcode(op);
	/* init the row*/
	initword(&(g_programSegment[g_IC]));
	/*set the opcode*/
	set_opcode(&(g_programSegment[g_IC]), opcode);


	/*advance pointer past slash */
	row++; /*e.g.   1/0/1,0 x,r1  */
	status = sscanf(row, "%c", &type);

	if ('0' == type || '1' == type || status<1)
		set_type(&(g_programSegment[g_IC]), BINVAL(type));
	else{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Syntax Error! illegal type identified [%d]\n", type);
		return reportError(msg, ERROR);
	}



	/* type 1 means that comb needs to be set */
	if ('1' == type){
		char combL , dbl;
		row = strchr(row, '/');
		if (!row)
			return reportError("Error - slash not found\n", ERROR);

		/*advance pointer passed slash*/
		row++; /*now:   0,0 x,r1   */
		status = sscanf(row, "%c", &combL );
		if (('0' != combL && '1' != combL) ||  status < 1)
		{

			char msg[MSG_MAX_SIZE];
			sprintf(msg, "Syntax Error! illegal comb indication identified [%c]\n", combL);
			return reportError(msg, ERROR);
		}
		else {

			int comb = BINVAL(combL);
			set_comb(&(g_programSegment[g_IC]), comb);
		}

	}

	/* advance to the dbl field */
	row = strchr(row, ',');
	if (!row)
		return reportError("Error - comma not found\n", ERROR);



	/*advance pointer passed comma to double filed*/
	row++; /*e.g.   0 x,r1  */
	status = sscanf(row, "%c", &dbl);
	if ('0' == dbl || '1' == dbl || status<1)
		set_double(&(g_programSegment[g_IC]), BINVAL(dbl));
	else{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Syntax Error! illegal double indication identified [%d]\n", dbl);
		return reportError(msg, ERROR);
	}


	/*advance pointer passed the double filed to the white space before first operand*/
	row++; /*e.g.    x,r1  */
	/*traverse white spaces*/
	while ((*row == ' ') || (*row) == '\t')
		row++;


	if (sscanf(row, "%s", src_opr) < 1)
		return reportError("Error, could not parse operands\n", ERROR);

	/*if src_opr includes a comma, then there's no space beween
	the operand and the comma -> null terminate the operand at the comma sign */
	helper = strchr(src_opr, ',');
	if (helper)
		(*helper) = 0;


	addressingMethod = getAddressingMethod(src_opr);
	if (!isDstAddressingMethodValid(op, addressingMethod))
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! [%s] is an illegal source addressing method  for opr [%s]\n", src_opr, op);
		return reportError(msg, ERROR);

	}

	/*encode the addressing method for source operand*/
	set_target_addr(&(g_programSegment[g_IC]), addressingMethod);
	additionalWordsOffset = 0;

	status = encodeOperand(src_opr, addressingMethod, DST, &additionalWordsOffset);
	if (NORMAL != status)
		return status;


	return NORMAL;
}





/* encode a binary operation */
/* e.g.   mov/1/0/1,0 x,r1  */
int encodeBinaryOpr(char *row, char *op){
	char type, dbl;
	char src_opr[MAX_ROW_SIZE], dst_opr[MAX_ROW_SIZE], *helper;
	int status, addressingMethod, additionalWordsOffset;
	int opcode = getDecOpcode(op);
	/* init the row*/
	initword(&(g_programSegment[g_IC]));
	/*set the opcode*/
	set_opcode(&(g_programSegment[g_IC]), opcode);


	/***********************************************/
	/*      encode type                           */
	/***********************************************/


	/*advance pointer past slash */
	row++; /*e.g.   1/0/1,0 x,r1  */
	status = sscanf(row, "%c", &type);

	if ('0' == type || '1' == type || status<1)
		set_type(&(g_programSegment[g_IC]), BINVAL(type));
	else{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Syntax Error! illegal type identified [%d]\n", type);
		return reportError(msg, ERROR);
	}




	/***********************************************/
	/*      encode comb                            */
	/***********************************************/
	
	/* type 1 means that comb needs to be set */
	if ('1' == type){
		char combL, combR, dbl;
		row = strchr(row, '/');
		if (!row)
			return reportError("Error - slash not found\n", ERROR);

		/*advance pointer passed slash*/
		row++; /*now:   0/1,0 x,r1   */
		status = sscanf(row, "%c/%c", &combL, &combR);
		if (('0' != combL && '1' != combL) || ('0' != combR && '1' != combR) || status < 2)
		{

			char msg[MSG_MAX_SIZE];
			sprintf(msg, "Syntax Error! illegal comb indication identified [%c/%c]\n", combL, combR);
			return reportError(msg, ERROR);
		}
		else {

			int comb = (2 * (BINVAL(combL))) + BINVAL(combR);
			set_comb(&(g_programSegment[g_IC]), comb);
		}

	}



	/***********************************************/
	/*      encode double                          */
	/***********************************************/

	/* advance to the dbl field */
	row = strchr(row, ',');
	if (!row)
		return reportError("Error - comma not found\n", ERROR);


	/*advance pointer passed comma to double filed*/
	row++; /*e.g.   0 x,r1  */
	status = sscanf(row, "%c", &dbl);
	if ('0' == dbl || '1' == dbl || status<1)
		set_double(&(g_programSegment[g_IC]), BINVAL(dbl));
	else{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Syntax Error! illegal double indication identified [%d]\n", dbl);
		return reportError(msg, ERROR);
	}


	/*advance pointer passed the double filed to the white space before first operand*/
	row++; /*e.g.    x,r1  */
	/*traverse white spaces*/
	while ((*row == ' ') || (*row) == '\t')
		row++;



	/***********************************************/
	/*      encode source operand                  */
	/***********************************************/


	if (sscanf(row, "%s", src_opr) < 1)
		return reportError("Error, could not parse operands\n", ERROR);
	
	/*if src_opr includes a comma, then there's no space beween
	the operand and the comma -> null terminate the operand at the comma sign */
	helper = strchr(src_opr, ',');
	if (helper)
		(*helper) = 0;


	addressingMethod = getAddressingMethod(src_opr);
	if (!isSrcAddressingMethodValid(op, addressingMethod))
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! [%s] is an illegal source addressing method  for opr [%s]\n", src_opr, op);
		return reportError(msg, ERROR);

	}
	
	/*encode the addressing method for source operand*/
	set_src_addr(&(g_programSegment[g_IC]), addressingMethod);
	additionalWordsOffset = 0;
	
	status = encodeOperand(src_opr, addressingMethod, SRC, &additionalWordsOffset);
	if (NORMAL != status)
		return status;



	/*go to comma for last operand*/
	row = strchr(row, ',');
	if (!row)
		reportError("Syntax Error: comma not found for second operand\n",ERROR);
	
	row++;



	/***********************************************/
	/*      encode destination operand             */
	/***********************************************/

	if (sscanf(row, "%s", dst_opr) < 1)
		return reportError("Error, could not parse operands\n", ERROR);


	addressingMethod = getAddressingMethod(dst_opr);
	if (!isDstAddressingMethodValid(op, addressingMethod))
	{
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! [%s] is an illegal destination addressing method  for opr [%s]\n", dst_opr, op);
		return reportError(msg, ERROR);

	}
	/*encode the addressing method for target operand*/
	set_target_addr(&(g_programSegment[g_IC]), addressingMethod);
	status = encodeOperand(dst_opr, addressingMethod, DST,&additionalWordsOffset);




	return status;

	
}

/*encode an operation with no operands */
int encodeNoParamOpr(char *row, char *op){
	char tmp, status;
	int opcode = getDecOpcode(op);
	/* init the row*/
	initword(&(g_programSegment[g_IC]));
	/*set the opcode*/
	set_opcode(&(g_programSegment[g_IC]), opcode);
	g_IC++;
	return NORMAL;

}




int getAddressingMethod(char *operand){

	sscanf(operand, "s", operand);

	if (operand[0] == '#')
		return IMMEDIATE;

	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '7')
		return REGISTER;

	if (strchr(operand, '{'))
		return DYNAMIC_INDEX;

	return DIRECT;


}


/* 
encodes a given operand 
o_additionalWords indicates many additional words were populated 
*/
int encodeOperand(char *operand, int method, int srcdst, int *o_additionalWords){
	int address, addWordIndex=0; 

	/* base word is always absolute */
	g_ICFlag[g_IC ] = ABSOLUTE_ADDRESS;
	
	switch (method){
	case DYNAMIC_INDEX:
		{
		int status, decAddress, index;

		(*o_additionalWords)++;

		status = parseDynamicOperand(operand, &decAddress, &index, *o_additionalWords);
		if (NORMAL != status)
			return status;
		
		mapword(&(g_programSegment[g_IC + (*o_additionalWords)]), decAddress);

		(*o_additionalWords)++;

		mapword(&(g_programSegment[g_IC + (*o_additionalWords)]), index);
		g_ICFlag[g_IC + (*o_additionalWords)] = ABSOLUTE_ADDRESS;

		}
		break;
	case REGISTER:
		address = operand[1] - '0';
		if (SRC == srcdst)
			set_src_reg(&(g_programSegment[g_IC]), address);
		else if (DST == srcdst)
			set_target_reg(&(g_programSegment[g_IC]), address);
		else
			reportError("Error, invalid source or target indication\n", ERROR);
	
		break;
	case DIRECT:
		/*increment aditional words used  */
		
		(*o_additionalWords)++;
		addWordIndex = g_IC + (*o_additionalWords);
		address = getSymbolDecimal(operand);
		if (KNF == address)
			return reportError("Label Error!, label not found in table\n",ERROR);
		mapword(&(g_programSegment[addWordIndex]), address);
	
			
		/*check if the symbol is external - if so insert it to the extrenal table */
		if (EXT_LABEL ==  getSymbolType(operand)){
			insertExternalLabel(operand, addWordIndex);
			g_ICFlag[addWordIndex] = EXTERNAL_ADDRESS;
		}
		else {
			g_ICFlag[addWordIndex] = RELATIVE_ADDRESS;
		}
	

		//TODO remove print20LSBs(&(g_programSegment[addWordIndex]));

		break;
	case IMMEDIATE: /*example: mov #-1,r2 */
	
		/*increment aditional words used  */
		(*o_additionalWords)++;
		addWordIndex = g_IC + (*o_additionalWords);
		
		/*advance passed the # */
		operand++;

		if (sscanf(operand, "%d", &address) < 1)
			return reportError("Syntax Error!, Illegal operand syntax for immediate method\n", ERROR);

		
		mapword(&(g_programSegment[addWordIndex]), address);
		g_ICFlag[addWordIndex] = ABSOLUTE_ADDRESS;

		break;
	default:
		reportError("Error!, illegal addressing method\n", ERROR);
	}

	return NORMAL;

}
