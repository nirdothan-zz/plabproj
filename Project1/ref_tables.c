#include <string.h>
#include "ref_tables.h"

extern Symbol_t *g_symbolTable;
extern int g_symbolTableSize;


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