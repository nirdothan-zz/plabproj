﻿#include "assemblerTypes.h"


/*	
	ref tables are encapsulated as static - 
	use functions to serach through
*/

/* mapping of opcodes to decimal and octal values */
/*
static Opcodes_t opcodes[] = {
		{ "mov", 0, 0, BINARY,  { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "cmp", 1, 1, BINARY,  { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "add", 2, 2, BINARY,  { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "sub", 3, 3, BINARY,  { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "not", 4, 4, UNARY,   { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "clr", 5, 5, UNARY,   { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "lea", 6, 6, BINARY,  { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "inc", 7, 7, UNARY,   { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "dec", 8, 10, UNARY,  { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "jmp", 9, 11, UNARY,  { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "bne", 10, 12, UNARY, { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "red", 11, 13, UNARY, { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "prn", 12, 14, UNARY, { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "jsr", 13, 15, UNARY, { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "rts", 14, 16, NOPARAMS, { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } },
		{ "stop", 15, 17, NOPARAMS, { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 } }

};
*/


static Opcodes_t opcodes[] = {
		{ "mov", 0,  0,	 BINARY,	{ 1, 1, 1, 1 }, { 0, 1, 1, 1 } },
		{ "cmp", 1,  1,	 BINARY,	{ 1, 1, 1, 1 }, { 1, 1, 1, 1 } },
		{ "add", 2,  2,	 BINARY,	{ 1, 1, 1, 1 }, { 0, 1, 1, 1 } },
		{ "sub", 3,  3,	 BINARY,	{ 1, 1, 1, 1 }, { 0, 1, 1, 1 } },
		{ "not", 4,  4,	 UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 1, 1 } },
		{ "clr", 5,  5,	 UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 1, 1 } },
		{ "lea", 6,  6,	 BINARY,	{ 0, 1, 1, 1 }, { 0, 1, 1, 1 } },
		{ "inc", 7,  7,	 UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 1, 1 } },
		{ "dec", 8,  10, UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 1, 1 } },
		{ "jmp", 9,	 11, UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 1, 1 } },
		{ "bne", 10, 12, UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 1, 1 } },
		{ "red", 11, 13, UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 1, 1 } },
		{ "prn", 12, 14, UNARY,		{ 0, 0, 0, 0 }, { 1, 1, 1, 1 } },
		{ "jsr", 13, 15, UNARY,		{ 0, 0, 0, 0 }, { 0, 1, 0, 0 } },
		{ "rts", 14, 16, NOPARAMS,	{ 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
		{ "stop", 15, 17, NOPARAMS, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } }
};
