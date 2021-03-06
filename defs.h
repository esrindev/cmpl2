#if 1 //for tsting with min limits
/* ahmed.sadikhov@gmail.com
   here we define all our LIMITS
*/

#ifndef DEFS_H
#define DEFS_H


//set debug flag
#define ESR_DEBUG 0

#define MAX_TOK_LEN 1024

/* max numb of local vars per function */
#define MAX_DEC 100

/* max number of var references inside function  */
#define MAX_USG 100

/* max number of tips, standars and user defined  */
#define MAX_TIP 1

/* max number of returns per function  */
#define MAX_QYT 15

#define MAX_FUNC 100

#define MAX_FCALL 100

#define MAX_FUNC_ARGS 100

/* max number of objects in obgect stack  */
#define MAX_OBJ 100

/* max number of members of new type  */
#define MAX_MEMB 20

/* max length of name  */
#define MAX_NAME_LEN 265

#define USR_TIP_START 5

/* index of global type in ttable  */
#define GLB_TIP USR_TIP_START

/* index of MAIN FUNC in global funks table */
#define MAIN_FUNC 0

#define LOCAL   1
#define MEMBER  2 
#define GLOBAL  3

/* max numb of els in stek*/
#define MAX_STEK_EL 1000000

/* max number of tokens in rule*/
#define MAX_RL_TOK 20
/* max number of states  */
#define MAX_STATES 500

/* max number of lexems on lexmes[] array, see error.c  */
#define MAX_LEXEM  100000

/* MAx number of error tokens   */
#define MAX_ERR_TKS 100000

/* MAX input bufer len  */
#define MAX_INB_LEN 1024

/* BAND_SIZE  max nodes in band  */
#define BAND_SIZE 1000000

/* define BAND_END  */
#define BAND_END exec_band + band_size

#define FIRST_INSTR_ADRS 1 /* 0 reserved  */

/* max number of errors  */
#define MAX_ERR_CNT 100

/* MAX LEN OF ERROR MESSAGE */
#define ERR_MSG_LEN 1024

//max length of src code
#define MAX_TEXT_LEN 1000000

//max number of scope childs
#define MAX_SCOPE_CHILD 100

//max number of standart funcs
#define MAX_STD_FUNC 20

#endif
#endif

//here we set our minimum limits for testing compiler on low mem machines

#if 0 //for tsting with min limits
/* ahmed.sadikhov@gmail.com
   here we define all our LIMITS
*/

#ifndef DEFS_H
#define DEFS_H


//set debug flag
#define ESR_DEBUG 0

#define MAX_TOK_LEN 1024

/* max numb of local vars per function */
#define MAX_DEC 3

/* max number of var references inside function  */
#define MAX_USG 3

/* max number of tips, standars and user defined  */
#define MAX_TIP 6

/* max number of returns per function  */
#define MAX_QYT 3

#define MAX_FUNC 3

/* max number of objects in obgect stack  */
#define MAX_OBJ 3

/* max number of members of new type  */
#define MAX_MEMB 3

/* max length of name  */
#define MAX_NAME_LEN 265

#define USR_TIP_START 5

/* index of global type in ttable  */
#define GLB_TIP USR_TIP_START

/* index of MAIN FUNC in global funks table */
#define MAIN_FUNC 0

#define LOCAL   1
#define MEMBER  2 
#define GLOBAL  3

/* max numb of els in stek*/
#define MAX_STEK_EL 10

/* max number of tokens in rule*/
#define MAX_RL_TOK 5
/* max number of states  */
#define MAX_STATES 500

/* max number of lexems on lexmes[] array, see error.c  */
#define MAX_LEXEM  100

/* MAx number of error tokens   */
#define MAX_ERR_TKS 100

/* MAX input bufer len  */
#define MAX_INB_LEN 1024

/* BAND_SIZE  max nodes in band  */
#define BAND_SIZE 100

/* define BAND_END  */
#define BAND_END exec_band + band_size

#define FIRST_INSTR_ADRS 1 /* 0 reserved  */

/* max number of errors  */
#define MAX_ERR_CNT 100

/* MAX LEN OF ERROR MESSAGE */
#define ERR_MSG_LEN 1024

#endif


#endif