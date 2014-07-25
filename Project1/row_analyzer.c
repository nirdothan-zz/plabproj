#include<string.h>
#include <stdio.h>
#include "assemblerTypes.h"

extern Word_t g_dataSegment;
extern int g_DC;


int isCommentOrEmpty(const char*);
int isValidLabelChar(const char);
int storeString(const char *);
int storeData(const char *);

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
		
		/* set label flag on */
		(*o_labelFlag) = 1;

		/* move the pointer till after the label, so that the calling program can processs the rest of the row */
		(*row) += labelLength + 1;

		return NORMAL;
}

/*
parse the possible data instruction part of the row
the pointer is moved to after the instruction if exists
*/ 
int parseInstruction(const char **row, int  *o_labelFlag){
	
	int status;
	char instruction[MAX_LABEL_SIZE];
	char instructionPrefix = '.';
	status = sscanf(*row, "%s", instruction);


	/*init instruction flag to be used as by the calling function */
	(*o_labelFlag) = NO_INST_FLAG;
	if (!status)
		return reportError("Error! Illegal or missing instruction\n", ERROR);

	
	/* is an instruction */
	if (instructionPrefix == instruction[0]){
		
		
		
		/* advance pointer to the '.' char */
		(*row) = strchr(*row, instructionPrefix);

		/* is a data instruction */
		if (!strcmp(instruction, DATA_INSTRUCTION))
		{
			(*o_labelFlag) = DATA_INSTRUCTION_FLAG;
			/* advance pointer to after the ".data" string */
			(*row) += strlen(DATA_INSTRUCTION);
			return storeData(*row);
		}
		/* is a string instruction */
		else if (!strcmp(instruction, STRING_INSTRUCTION))
		{
			(*o_labelFlag) = STRING_INSTRUCTION_FLAG;
			/* advance pointer to after the ".string" string */
			(*row) += strlen(STRING_INSTRUCTION);
			return storeString(*row);
		}
		/* is an extern instruction */
		else if (!strcmp(instruction, EXTERN_INSTRUCTION))
		{
			(*o_labelFlag) = EXTERN_INSTRUCTION_FLAG;
			/* advance pointer to after the ".extren" string */
			(*row) += strlen(EXTERN_INSTRUCTION);
			return NORMAL;
		}
		/* is an extern instruction */
		else if (!strcmp(instruction, ENTRY_INSTRUCTION))
		{
			(*o_labelFlag) = ENTRY_INSTRUCTION_FLAG;
			/* advance pointer to after the ".entry" string */
			(*row) += strlen(ENTRY_INSTRUCTION);
			return NORMAL;
		}
			
		/* it begins with a '.' but is neither .string nor .data, .entry, .extern */
		else {
			reportError("Error!unknown data instruction\n",ERROR);
		}
	}

	/*TODO remove */
	/* not a data instruction */
	else {
		printf("NOT data instrcution\n");
	}
	

		return NORMAL;
}

/* parse the ".data" instruction and stores its values in the data image */
int storeData(char *data){
	int integer;
	/* sscanf format needs a variable because we add ',' after the first time */
	char format[] = " %d";

	do { 

		if (sscanf(data, format, &integer) != 1){
			return reportError("Error! Ilegal data for .data instruction\n", ERROR);
		}
		/* from now on we look for integers that come after commas*/
		format[0] = ',';


		/* store the integer that followed ".data" in a new row in the global data segment */
		/* data segment rows are 20 bits long, and require mapping from int */
		/* increment data dounter */
		mapword(&(g_dataSegment[g_DC++]), integer);

		/* look for the next number - advance the pointer to the next comma */
		/* advance the pointer to th enext comma */
		data = strchr(++data, ',');
	} while (data);

	return NORMAL;
}

/* parse the ".string" instruction and stores its chars in the data image */
int storeString(char *data){
	char string[MAX_ROW_SIZE];
	int i=0;
	if (sscanf(data, "%s", &string) != 1){
		return reportError("Error! Ilegal string value\n", ERROR);
	}

	
	/* traverse string chars until null terminator*/
	while (string[i]){
		/*map each chars ascii value to a separate row in the data image*/
		mapword(&(g_dataSegment[g_DC++]), string[i++]);
	}

	/*apend a null terminator row at the end of the string data image and increment counter*/
	memset((&(g_dataSegment[g_DC++])),0,sizeof(Word_t));
	
	return NORMAL;
}

