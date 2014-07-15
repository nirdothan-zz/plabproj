#include<string.h>
#include <stdio.h>

int parseRow(char *ro){

	char  *row = "a:b:c";
	char *del= ":";
	char label[20], rest[20];
	char *pStr = (char*)malloc(100);


	//ret = strrchr(row, 'a');
	pStr = strtok(row,":");

	

	if (pStr)
		printf("label=%s\n\n", pStr);
	else
		printf("is null\n");
	return 0;
}
