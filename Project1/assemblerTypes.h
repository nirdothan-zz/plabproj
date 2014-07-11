typedef char Word_t[3];

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
