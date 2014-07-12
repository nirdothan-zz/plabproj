#include "assemblerTypes.h"

/*	
	ref tables are encapsulated as static - 
	use functions to serach through
*/

/* mapping of opcodes to decimal and octal values */
static Opcodes_t opcodes[] = {
		{ "mov", 0, 0 },
		{ "cmp", 1, 1 },
		{ "add", 2, 2 },
		{ "sub", 3, 3 },
		{ "not", 4, 4 },
		{ "clr", 5, 5 },
		{ "lea", 6, 6 },
		{ "inc", 7, 7 },
		{ "dec", 8, 10 },
		{ "jmp", 9, 11 },
		{ "bne", 10, 12 },
		{ "red", 11, 13 }
};