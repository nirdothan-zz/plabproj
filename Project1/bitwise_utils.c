#include "assemblerTypes.h"

void print20LSBs( const void *bitStream)
{
	unsigned int i, *intform, mask=1;
	mask <<= (WORD_BITS - 1);

	intform = (bitStream);
	
	for (i = 0; i < WORD_BITS; i++)
	{
		printf("%c", (*intform & mask) == 0 ? '0' : '1');
		mask >>= 1;
	}

	printf("\n");
}

void printHEX(const Word_t row)
{
	unsigned int  maskMSB = 0xf, mask = 0xff;
	
	printf("%X %X %X \n", (row[2] & maskMSB), row[1] & mask , row[0] & mask );
	

}

/* mapword maps an integer into the 20 LSBs of 24 bit word */

void mapword(Word_t target, const int src)
{
	
	target[0] = (src & 0xff);	  /* 8 LSBs of src are mapped to target[0] byte*/
	target[1] = (src>>8 & 0xff);  /* 8 next bits of src are mapped to target[1] byte*/
	target[2] = (src>>16 & 0x3f); /* 4 MSBs of src are mapped to target[2] 4 LSBs*/
	print20LSBs(target);
}

/*
	return the octal value for a given decimal
*/
int getOctal(int decimal){
	int oct=0;
	int resid=0, whole=0, tens=1;
	whole = decimal;

	while (whole > 0) {
		resid = whole % 8;
		oct = oct+(tens * resid);
		whole = whole / 8;
		tens *= 10;
	}

	return oct;
}


