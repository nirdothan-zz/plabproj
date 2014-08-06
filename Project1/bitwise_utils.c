#include "assemblerTypes.h"

void print20LSBs( const void *bitStream)
{
	
	char stops[] = { 1,2,3,7,9,12,14,17};
	unsigned int i,j, *intform, mask=1;
	mask <<= (WORD_BITS - 1);

	intform = (bitStream);
	j = 0;
	for (i = 0; i < WORD_BITS; i++)
	{
		printf("%c", (*intform & mask) == 0 ? '0' : '1');
		mask >>= 1;
		if (i == stops[j]){
			printf("%c", '-');
			j++;
		}

	}

	printf("\n");
}

void printHEX(const Word_t row)
{
	unsigned int  maskMSB = 0xf, mask = 0xff;
	
	printf("%X %X %X \n", (row[2] & maskMSB), row[1] & mask , row[0] & mask );
	

}


/* init a word to all zeros */
void initword(Word_t word){
	word[0] = 0;
	word[1] = 0;
	word[2] = 0;
}
/*convert 20 bits into an integer */
int mapwordtodecimal(Word_t src){
	
	int target = 0;
	unsigned int mask=1;
	 

	target = (src[2] & 0xf);
	

	/*if the 4th bit is on then negative number */
	mask <<= 3;
	if (target & mask) /*negative*/
	{
		 mask =0;
		mask = ~mask; /*all 1s*/

		mask <<= 4;
		/*compliment all more significant bits than the first 4 with 1s*/
		target |= mask;
	}

	target <<= 8;
	
	target |= (src[1] & 0xff);
	target <<= 8;
	target |= (src[0] & 0xff);
	return target;
}
/* mapword maps an integer into the 20 LSBs of 24 bit word */
void mapword(Word_t target, const int src)
{
	Word_t tmp;
	tmp[0] = (src & 0xff);	  /* 8 LSBs of src are mapped to target[0] byte*/
	tmp[1] = (src >> 8 & 0xff);  /* 8 next bits of src are mapped to target[1] byte*/
	tmp[2] = (src >> 16 & 0xf); /* 4 MSBs of src are mapped to target[2] 4 LSBs*/

	target[0] |= tmp[0];
	target[1] |= tmp[1];
	target[2] |= tmp[2];

	/*print20LSBs(target);*/
}

mapshifter(Word_t target,  int src, const int shiftSize){
	src <<= shiftSize;
	mapword(target, src);
}

void set_comb(Word_t target,  int src){
	mapshifter(target, src,0);
}

void set_target_reg(Word_t target, int src){
	mapshifter(target, src, 2);
}

void set_target_addr(Word_t target, int src){
	mapshifter(target, src, 5 );
}

void set_src_reg(Word_t target, int src){
	mapshifter(target, src, 7);
}


void set_src_addr(Word_t target, int src){
	mapshifter(target, src, 10);
}

void set_opcode(Word_t target, int src){
	mapshifter(target, src, 12);
}


void set_type(Word_t target, int src){
	mapshifter(target, src, 16);
}

void set_double(Word_t target, int src){
	mapshifter(target, src, 17);
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


