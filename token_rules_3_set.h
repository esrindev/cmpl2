#ifndef TOKEN_RULES_3_SET
#define TOKEN_RULES_3_SET


//length 3
struct token_rules_3{
	int id1;
	int id2;
	int id3;
	int rules[100]; // number of rules this token occurs
	int len;
};

struct token_rules_3 token_rules_3_set[]={

{ IDT , ASGN , IDT , { 327 ,}, 1 },
{ IDT , ASGN , CHR , { 331 ,}, 1 },
{ IDT , ASGN , NUMB , { 330 ,}, 1 },
{ IDT , ASGN , CRG1 , { 328 ,}, 1 },
{ IDT , ASGN , CRG2 , { 329 ,}, 1 },
{ IDT , ASGN , EXPR , { 333 ,}, 1 },
{ IDT , ASGN , FLOAT , { 332 ,}, 1 },
{ IDT , ASGN , FCALL1 , { 335 ,}, 1 },
{ IDT , ASGN , BRK_VAL , { 334 ,}, 1 },
{ ASGN , IDT , NOQTEV , { 327 ,336 ,345 ,}, 3 },
{ ASGN , CHR , NOQTEV , { 331 ,340 ,349 ,}, 3 },
{ ASGN , NUMB , NOQTEV , { 330 ,339 ,348 ,}, 3 },
{ ASGN , CRG1 , NOQTEV , { 328 ,337 ,346 ,}, 3 },
{ ASGN , CRG2 , NOQTEV , { 329 ,338 ,347 ,}, 3 },
{ ASGN , EXPR , NOQTEV , { 333 ,342 ,351 ,}, 3 },
{ ASGN , FLOAT , NOQTEV , { 332 ,341 ,350 ,}, 3 },
{ ASGN , FCALL1 , NOQTEV , { 335 ,344 ,353 ,}, 3 },
{ ASGN , BRK_VAL , NOQTEV , { 334 ,343 ,352 ,}, 3 },
{ OPNBLK , HAL2 , CLSBLK , { 999 ,}, 1 },
{ CRG1 , ASGN , IDT , { 336 ,}, 1 },
{ CRG1 , ASGN , CHR , { 340 ,}, 1 },
{ CRG1 , ASGN , NUMB , { 339 ,}, 1 },
{ CRG1 , ASGN , CRG1 , { 337 ,}, 1 },
{ CRG1 , ASGN , CRG2 , { 338 ,}, 1 },
{ CRG1 , ASGN , EXPR , { 342 ,}, 1 },
{ CRG1 , ASGN , FLOAT , { 341 ,}, 1 },
{ CRG1 , ASGN , FCALL1 , { 344 ,}, 1 },
{ CRG1 , ASGN , BRK_VAL , { 343 ,}, 1 },
{ CRG2 , ASGN , IDT , { 345 ,}, 1 },
{ CRG2 , ASGN , CHR , { 349 ,}, 1 },
{ CRG2 , ASGN , NUMB , { 348 ,}, 1 },
{ CRG2 , ASGN , CRG1 , { 346 ,}, 1 },
{ CRG2 , ASGN , CRG2 , { 347 ,}, 1 },
{ CRG2 , ASGN , EXPR , { 351 ,}, 1 },
{ CRG2 , ASGN , FLOAT , { 350 ,}, 1 },
{ CRG2 , ASGN , FCALL1 , { 353 ,}, 1 },
{ CRG2 , ASGN , BRK_VAL , { 352 ,}, 1 },
{ BLOCK , YOXSA , BLOCK , { 989 ,}, 1 },
{ BLOCK , YOXSA , SIMPLE_OPER , { 987 ,}, 1 },
{ BLOCK , YOXSA , COMP_OPER , { 988 ,}, 1 },
{ EGER_EXP2 , BLOCK , YOXSA , { 987 ,988 ,989 ,}, 3 },
{ EGER_EXP2 , SIMPLE_OPER , YOXSA , { 981 ,982 ,983 ,}, 3 },
{ EGER_EXP2 , COMP_OPER , YOXSA , { 984 ,985 ,986 ,}, 3 },
{ SEC1 , OPNBLK , HAL2 , { 999 ,}, 1 },
{ SIMPLE_OPER , YOXSA , BLOCK , { 983 ,}, 1 },
{ SIMPLE_OPER , YOXSA , SIMPLE_OPER , { 981 ,}, 1 },
{ SIMPLE_OPER , YOXSA , COMP_OPER , { 982 ,}, 1 },
{ COMP_OPER , YOXSA , BLOCK , { 986 ,}, 1 },
{ COMP_OPER , YOXSA , SIMPLE_OPER , { 984 ,}, 1 },
{ COMP_OPER , YOXSA , COMP_OPER , { 985 ,}, 1 },


};


int token_rules_3_set_len = 50;

#endif