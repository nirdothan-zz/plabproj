#include <string.h>
#include "ref_tables.h"

extern Symbol_t *g_symbolTable;
extern int g_symbolTableSize;
extern Symbol_t *g_externalTable;			
extern int g_externalTableSize;
extern Symbol_t *g_entryTable;			
extern int g_entryTableSize;


/* generic macro to lookup values in reference tables */
/* params:
	TABLE: the name of the array variable that is searched through
	TYPE: the type of each record in the table (used for number of rows calculation)
	KEY_FIELD: the name of the struct field in TABLE that is serached by
	KEY_VAL: the value that is serached by
	RESULT_FIELD: the name of the struct field in TABLE that is serached for
	RESULT_VAL: the value that is being searched for or KNF_VAL if not found
	
*/
/*  macro commented out becuase of compliation issues

#define FINDINREF(TABLE, TYPE, KEY_FIELD, KEY_VAL, RESULT_FIELD, RESULT_VAL)\
{\
	int i = 0;\
	RESULT_VAL=KNF;\
	while (i < (sizeof(TABLE) / sizeof(TYPE))) \
	{ \
		if (!strcmp(opcodes[i].##KEY_FIELD, KEY_VAL)) \
		{\
			RESULT_VAL=opcodes[i].##RESULT_FIELD; \
			break;\
		}\
		i++; \
	}\
}								

*/

void trimSlash(char*, char*);
int  getOpcodeGroup(char*);
int  getDecOpcode(char *op);
void incrementDataLabels(int);
int getSymbolType(char*);
int getEntryLabelOctall(char*);

/* symbol table printing utility */
void dumpSymbolTable(){
	int i;

	for (i = 0; i < g_symbolTableSize; i++)
		printf("%s <%d> <%d>\n", g_symbolTable[i].label, g_symbolTable[i].decimal, g_symbolTable[i].octal);

	
}
/* increment all data labels by value of increment parameter*/
void incrementDataLabels(int increment){
	int i = 0;

	while (i < g_symbolTableSize)
	{
		if (DATA_LABEL == g_symbolTable[i].type)
		{
			g_symbolTable[i].decimal += increment;
			g_symbolTable[i].octal = getOctal(g_symbolTable[i].decimal);
		}
			
		i++;
	}
}

/* get the decimal address of a symbol */
int getSymbolDecimal(char *symbol){
	int i = 0; 
		
	while (i < g_symbolTableSize) 
	{ 
		if (!strcmp(g_symbolTable[i].label, symbol))
			return g_symbolTable[i].decimal;
		i++; 
	}
	return KNF;
}



/* get the octal address of a symbol */
int getSymbolOctall(char *symbol){
	int i = 0;

	while (i < g_symbolTableSize)
	{
		if (!strcmp(g_symbolTable[i].label, symbol))
			return g_symbolTable[i].octal;
		i++;
	}
	return KNF;
}


/* get the ocatal address of an entry symbol */
int getEntryLabelOctall(char *symbol){
	int i = 0;

	while (i < g_entryTableSize)
	{
		if (!strcmp(g_entryTable[i].label, symbol))
			return g_entryTable[i].octal;
		i++;
	}
	return KNF;
}


/* get the type  address of a symbol */
int getSymbolType(char *symbol){
	int i = 0;

	while (i < g_symbolTableSize)
	{
		if (!strcmp(g_symbolTable[i].label, symbol))
			return g_symbolTable[i].type;
		i++;
	}
	return KNF;
}

/* not in use */
int getSymbolDecimalOfType(char *symbol, int sym_type){
	int i = 0;

	while (i < g_symbolTableSize)
	{
		if (!strcmp(g_symbolTable[i].label, symbol) && g_symbolTable[i].type == sym_type)
			return g_symbolTable[i].decimal;
		i++;
	}
	return KNF;
}


/* for given opcode and addressing method - checks if the source addressing method is valid 
for that opcode. 1-indicates valid whereas 0 indicates invalid */
int isSrcAddressingMethodValid(char *op, int method){

	int res;
	int i = 0; 
	res = KNF; 
	while (i < (sizeof(opcodes) / sizeof(Opcodes_t)))
	{ 
		if (!strcmp(opcodes[i].opcode, op))
		{
			res = opcodes[i].sourceAddressingMethods[method];
			break;
		}
		i++; 
	}
									
	return res;
}


/* for given opcode and addressing method - checks if the destination addressing method is valid
for that opcode. 1-indicates valid whereas 0 indicates invalid */
int isDstAddressingMethodValid(char *op, int method){
	int res;
	int i = 0;
	res = KNF;
	while (i < (sizeof(opcodes) / sizeof(Opcodes_t)))
	{
		if (!strcmp(opcodes[i].opcode, op))
		{
			res = opcodes[i].targetAddressingMethods[method];
			break;
		}
		i++;
	}

	return res;
}

/*returns the octal mapping of a given opcode*/
int  getOctOpcode(char *op){
	int res;

	int i = 0;
	res = KNF;
	while (i < (sizeof(opcodes) / sizeof(Opcodes_t)))
	{
		if (!strcmp(opcodes[i].opcode, op))
		{
			res = opcodes[i].octal;
			break;
		}
		i++;
	}

	return res;
}


/*returns the decimal mapping of a given opcode*/
int  getDecOpcode(char *op){
	int res;

	int i = 0;
	res = KNF;
	while (i < (sizeof(opcodes) / sizeof(Opcodes_t)))
	{
		if (!strcmp(opcodes[i].opcode, op))
		{
			res = opcodes[i].decimal;
			break;
		}
		i++;
	}

	return res;
}

/*returns the group mapping of a given opcode*/
int  getOpcodeGroup(char *op){
	int res, trimsize;
	char trimmedOp[MAX_ROW_SIZE];
	int i = 0;
	trimSlash(trimmedOp,op);


	res = KNF;
	while (i < (sizeof(opcodes) / sizeof(Opcodes_t)))
	{
		if (!strcmp(opcodes[i].opcode, trimmedOp))
		{
			res = opcodes[i].group;
			break;
		}
		i++;
	}

	return res;
}

/* used to trip the slash and everything after it from the opcode, if exists
if no slash exists, the dst string remains equal to the src */
void trimSlash(char *dst, char *src){
	int  trimsize;
	char  *p;

	p = strchr(src, '/');

	if (p)
		trimsize = (p - src);
	else
		trimsize = sizeof(src);

	strncpy(dst, src, trimsize);
	dst[trimsize] = '\0';

}

/* insert a label into the extrnal tables */
int insertExternalLabel(char *label, int label_dec_address)
{
	strcpy((g_externalTable[g_externalTableSize]).label, label);
	(g_externalTable[g_externalTableSize]).type = EXT_LABEL; /*not in use */
	(g_externalTable[g_externalTableSize]).decimal = label_dec_address;
	(g_externalTable[g_externalTableSize]).octal = getOctal(label_dec_address);
	g_externalTableSize++;
	return NORMAL;

}

/* insert a label into the label tables */
int insertLabel(char *label, int tableType, int label_dec_address)
{


	switch (tableType){
	case EXT_LABEL:
	case DATA_LABEL:
	case CODE_LABEL:

		/*check for duplicate definition*/
		if (KNF != getSymbolOctall(label)){
			char msg[MSG_MAX_SIZE];
			sprintf(msg, "Error! duplicate defintion of label [%s]\n", label);
			return reportError(msg, ERROR);
		}

		strcpy((g_symbolTable[g_symbolTableSize]).label, label);
		(g_symbolTable[g_symbolTableSize]).type = tableType;
		(g_symbolTable[g_symbolTableSize]).decimal = label_dec_address;
		(g_symbolTable[g_symbolTableSize]).octal = getOctal(label_dec_address);
		g_symbolTableSize++;
		break;
	case ENT_LABEL:
		/*check for duplicate definition*/
		if (KNF != getEntryLabelOctall(label)){
			char msg[MSG_MAX_SIZE];
			sprintf(msg, "Error! duplicate defintion of entry label [%s]\n", label);
			return reportError(msg, ERROR);
		}

		strcpy((g_entryTable[g_entryTableSize]).label, label);
		(g_entryTable[g_entryTableSize]).type = tableType;
		(g_entryTable[g_entryTableSize]).decimal = label_dec_address;
		(g_entryTable[g_entryTableSize]).octal = getOctal(label_dec_address);
		g_entryTableSize++;
		break;

	}

	return NORMAL;
}