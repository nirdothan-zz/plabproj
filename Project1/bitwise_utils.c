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

void mapword(Word_t word, int num)
{
	int i, mask = 1;

	word[0] = 0;
	word[1] = 0;
	word[2] = 0;
	for (i = 0; i < 8; i++)
	{
		word[0] |= (num & mask);
		mask <<= 1;
	}
	
	
	for (i = 0; i < 8; i++)
	{
		word[1] |= (num & mask);
		mask <<= 1;
	}


	
	for (i = 0; i < 8; i++)
	{
		word[2] |= (num & mask);
		mask <<= 1;
	}

}