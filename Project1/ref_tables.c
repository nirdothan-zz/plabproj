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

void trimSlash(char *, char *);
int  getOpcodeGroup(char *);
int  getDecOpcode(char *op);
void incrementDataLabels(int);



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

int isSrcAddressingMethodValid(char *op, int method){
	int *res;
	FINDINREF(opcodes, Opcodes_t, opcode, op, sourceAddressingMethods, res)	
	return res[method];
}

int isDstAddressingMethodValid(char *op, int method){
	int *res;
	FINDINREF(opcodes, Opcodes_t, opcode, op, targetAddressingMethods, res)
	return res[method];
}

/*returns the octal mapping of a given opcode*/
int  getOctOpcode(char *op){
	int res;
	FINDINREF(opcodes, Opcodes_t, opcode, op, octal, res)
	return res;
}


/*returns the decimal mapping of a given opcode*/
int  getDecOpcode(char *op){
	int res;
	FINDINREF(opcodes, Opcodes_t, opcode, op, decimal, res);
	return res;
}

/*returns the group mapping of a given opcode*/
int  getOpcodeGroup(char *op){
	int res, trimsize;
	char trimmedOp[MAX_ROW_SIZE];

	trimSlash(trimmedOp,op);

	printf("checking opr <%s>\n", trimmedOp);
	
	FINDINREF(opcodes, Opcodes_t, opcode, trimmedOp, group, res);
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

/* insert a label into the label tables */
int insertLabel(char *label, int tableType, int label_dec_address)
{


	switch (tableType){
	case EXT_LABEL:
		strcpy((g_externalTable[g_externalTableSize]).label, label);
		(g_externalTable[g_externalTableSize]).type = tableType;
		(g_externalTable[g_externalTableSize]).decimal = label_dec_address;
		(g_externalTable[g_externalTableSize]).octal = getOctal(label_dec_address);
		g_externalTableSize++;

	case DATA_LABEL:
	case CODE_LABEL:
		strcpy((g_symbolTable[g_symbolTableSize]).label, label);
		(g_symbolTable[g_symbolTableSize]).type = tableType;
		(g_symbolTable[g_symbolTableSize]).decimal = label_dec_address;
		(g_symbolTable[g_symbolTableSize]).octal = getOctal(label_dec_address);
		g_symbolTableSize++;
		break;
	case ENT_LABEL:
		strcpy((g_entryTable[g_entryTableSize]).label, label);
		(g_entryTable[g_entryTableSize]).type = tableType;
		(g_entryTable[g_entryTableSize]).decimal = label_dec_address;
		(g_entryTable[g_entryTableSize]).octal = getOctal(label_dec_address);
		g_entryTableSize++;
		break;

	}

	return NORMAL;
}