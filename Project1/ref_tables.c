#include <string.h>
#include "ref_tables.h"

#define KNF -1

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