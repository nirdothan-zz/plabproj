#include "assemblerTypes.h"


/*	
	ref tables are encapsulated as static - 
	use functions to serach through
*/

/* mapping of opcodes to decimal and octal values */
static Opcodes_t opcodes[] = {
		{ "mov", 0, 0,   BINARY  },
		{ "cmp", 1, 1,   BINARY  },
		{ "add", 2, 2,   BINARY  },
		{ "sub", 3, 3,   BINARY  },
		{ "not", 4, 4,   UNARY   },
		{ "clr", 5, 5,   UNARY   },
		{ "lea", 6, 6,   BINARY  },
		{ "inc", 7, 7,   UNARY   },
		{ "dec", 8, 10,  UNARY   },
		{ "jmp", 9, 11,  UNARY   },
		{ "bne", 10, 12, UNARY   },
		{ "red", 11, 13, UNARY   },
		{ "prn", 12, 14, UNARY   },
		{ "jsr", 13, 15, UNARY   },
		{ "rts", 14, 16, NOPARAMS},
		{ "stop",15, 17, NOPARAMS}

};