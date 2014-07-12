#include<stdio.h>
#include "assemblerTypes.h"
void main()
{

	printf(" opcode %d\n", getDecOpcode("jms"));
	
	firstPass();
}