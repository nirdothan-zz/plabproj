/* max number of rows of program segment*/
#define MAX_PC 2000 

/* the size of a word in bits */
#define WORD_BITS 20

/* 
storage for 20 bit words - with 4 excess bits
we will use the 20 LSBs
*/
typedef char Word_t[3];

/* struct for symbol table*/
typedef struct {
	char label[31];
	int decimal;
	int octal;
	int type;
} Symbol_t;

/* 
bit fileds helper struct to assit with bit manipulation

*/
typedef struct {
		unsigned char comb : 2;
		unsigned char target_reg : 3;	/* 3-5 */
		unsigned char target_addr : 2;	/* 6-7 */
		unsigned char src_reg : 3;		/* 8-10 */
		unsigned char src_addr : 2;		/* 11-12 */
		unsigned char opcode : 4;		/* 13-16 */
		unsigned char type : 1;			/* 17 */
		unsigned char dbl : 1;			/* 18 */
		unsigned char : 2;
	
} Fields_t;

typedef struct opcodes {
	char opcode[4];
	int decimal;
	int octal;
	int group; /*BINARY, UNARY, NOPARAMS*/
} Opcodes_t;

enum { BINARY, UNARY, NOPARAMS }; /* for operation type */

#define INP_SUFFIX ".as"
#define EXT_SUFFIX ".ext"
#define OBJ_SUFFIX ".ob"
#define ENT_SUFFIX ".ent"
#define MAX_LABEL_SIZE 30
#define MAX_ROW_SIZE 80

/*statuses used as return value */
enum { NORMAL=0, END, WARNING, ERROR, FATAL };

#define MSG_MAX_SIZE 100
#define COMMENT_CHAR ';'
#define MAX_SYMBOLS 2000
#define INIT_IC 100
#define INIT_DC 0
#define DATA_INSTRUCTION ".data"
#define STRING_INSTRUCTION ".string"
#define EXTERN_INSTRUCTION ".extern"
#define ENTRY_INSTRUCTION ".entry"
#define KNF -1 /*Key Not Found value for referece tables*/

enum {DATA_LABEL, CODE_LABEL, EXT_LABEL  };

/*indicates instuction types identified*/
enum { NO_INST_FLAG = 0, DATA_INSTRUCTION_FLAG, STRING_INSTRUCTION_FLAG, EXTERN_INSTRUCTION_FLAG, ENTRY_INSTRUCTION_FLAG };