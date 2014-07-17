#include<string.h>
#include <stdio.h>
#include "assemblerTypes.h"

extern Symbol_t *g_symbolTable;
extern int g_symbolTableSize;
extern int g_PC;

/* check if the row string represents a comment or an empty line */
int isCommentOrEmpty(const char *row){
	char dummy[MSG_MAX_SIZE];
	/* if sscanf returned 0 then all chars were white spaces */
	return (( COMMENT_CHAR == row[0] )|| (!sscanf(row,"%s", dummy)));

}

/* check that chars are valid ofr label i.e. alphanumeric*/
int isValidLabelChar(const char c){
	return ((c >= 'A' && c < 'z') || (c >= '0' && c <= '9'));
}


/*
 parse the possible label part of the row
 the pointer is moved to after the label if exists 
*/
int parseLabel(const char **row){
	const int labelIndication = ':';
	int labelLength, i;
	char *label[MAX_LABEL_SIZE + 1], *pl = label, *end;

	/* find if this row begins with a label*/
	end = strchr(*row, labelIndication);

	/* no label in string */
	if (!end)
		return NORMAL;

	/* length of label must be under MAX_LABEL_SIZE =30 */
	if ((labelLength = (end - *row)) > MAX_LABEL_SIZE){
		char msg[MSG_MAX_SIZE];
		sprintf(msg, "Error! illegal label length =%d\n", labelLength);
		return reportError(msg,ERROR);
	}

	/* copy label chars to label array, only if they're legal */
	for (i = 0; i < labelLength; i++)
		if (isValidLabelChar((*row)[i]))
			*pl++ = (*row)[i];
		else{

			char msg[MSG_MAX_SIZE];
			sprintf(msg, "Error! illegal label character %c\n", ((*row)[i]));
			return reportError(msg, ERROR);

		}
		
		*pl = 0;
				


		/* write label to symbol table */
		strcpy((g_symbolTable[g_symbolTableSize].label), label);
		g_symbolTable[g_symbolTableSize].decimal = g_PC;
		g_symbolTableSize++;


		//printf("label is <%s>\n", g_symbolTable[g_symbolTableSize-1].label);


		/* move the pointer till after the label, so that the calling program can processs the rest of the row */
		(*row) += labelLength + 1;

		return NORMAL;


}

int parseRow(const char *row){

	printf("row is <%s>\n", row);

	if (isCommentOrEmpty(row))
		return NORMAL;
	parseLabel(&row);

	return NORMAL;
	
}
