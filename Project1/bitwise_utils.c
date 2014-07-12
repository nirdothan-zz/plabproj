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

void mapword(Word_t target, const int src)
{
	target[0] = (src & 0xff);
	target[1] = (src>>8 & 0xff);
	target[2] = (src>>16 & 0x3f);
	
}