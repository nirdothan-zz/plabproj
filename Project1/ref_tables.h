#include "assemblerTypes.h"


/*	
	ref tables are encapsulated as static - 
	use functions to serach through
*/

/* mapping of opcodes to decimal and octal values */
static Opcodes_t opcodes[] = {
		{ "mov", 0,  0,2},
		{ "cmp", 1,  1,2},
		{ "add", 2,  2,2},
		{ "sub", 3,  3,2},
		{ "not", 4,  4,1},
		{ "clr", 5,  5,1},
		{ "lea", 6,  6,2},
		{ "inc", 7,  7,1},
		{ "dec", 8, 10,1},
		{ "jmp", 9, 11,1},
		{ "bne", 10,12,1},
		{ "red", 11,13,1},
		{ "prn", 12,14,1},
		{ "jsr", 13,15,1},
		{ "rts", 14,16,0},
		{ "stop",15,17,0}

};