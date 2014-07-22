#include<string.h>
#include <stdio.h>
#include "assemblerTypes.h"



int isCommentOrEmpty(const char*);
int isValidLabelChar(const char);

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
int parseLabel(const char **row, char *o_labelFlag, char *o_label){


	const int labelIndication = ':';
	int labelLength, i;
	char label[MAX_LABEL_SIZE + 1], *pl = label, *end;

	*o_labelFlag = 0;
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
				
		strcpy(o_label, label);
		
		(*o_labelFlag) = 1;
		
		///* write label to symbol table */
		//if (g_symbolTableSize <= MAX_SYMBOLS){
		//	strcpy((g_symbolTable[g_symbolTableSize].label), label);
		//	g_symbolTable[g_symbolTableSize].decimal = g_DC;
		//	g_symbolTableSize++;
		//}
		//else {
		//	return reportError("Error! Maximum # of labels exceeded\n",ERROR);
		//}


		

		/* move the pointer till after the label, so that the calling program can processs the rest of the row */
		(*row) += labelLength + 1;

		return NORMAL;
}

/*
parse the possible data instruction part of the row
the pointer is moved to after the instruction if exists
*/
int parseInstruction(const char **row, char *isDataFlag){
	
	int status;
	char instruction[MAX_LABEL_SIZE];
	status = sscanf(*row, "%s", instruction);

	if (!status)
		return reportError("Error! Illegal or missing instruction\n", ERROR);

	/* is a data instruction */
	if ('.' == instruction[0]){
		/* set a flag to let the calling function know that it is a data instruction*/
		*isDataFlag = 1;
		if (!strcmp(instruction, DATA_INSTRUCTION))
		{
			printf("data instrcution\n");
		}
		else if (!strcmp(instruction, STRING_INSTRUCTION))
		{
			printf("string instrcution\n");
		}
		else {
			reportError("Error!unknown data instruction\n",ERROR);
		}
	}
	else {
		printf("NOT data instrcution\n");
	}
	


		/* move the pointer till after the label, so that the calling program can processs the rest of the row */
	//	(*row) += labelLength + 1;

		return NORMAL;
}
