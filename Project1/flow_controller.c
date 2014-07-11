#include "assemblerTypes.h"
#include <string.h>

Word_t programSegment[MAX_PC];

int firstPass(){
	Fields_t fields;
	Word_t w;
	int a = -1;

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



}