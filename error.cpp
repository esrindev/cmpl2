
  #include "esrin.h"

#if 0

/*#include <conio.h>*/

char *file_name = NULL;


struct lexem {
  int id;
  struct token *tok;
};

struct lexem lexems[MAX_LEXEM];
int lct;

int not_in_left_asgn(int tk);

/* hardcoded  */
int left_tks[] = {  NUMB, FLOAT, IDT,  CRG1,  CRG2,  EXPR,  STRMEM,  DOT,  ARYOPN,  ARYCLS,  MULT,  DEL,  PRCT,  POSSIG,  NEGSIG,  OPNBRK,  CLSBRK};
int left_tc = 17; /*  token count  */

int right_tks[] = {  NUMB, FLOAT, IDT,  CRG1,  CRG2,  EXPR,  STRMEM,  DOT,  ARYOPN,  ARYCLS,  MULT,  DEL,  PRCT,  POSSIG,  NEGSIG,  OPNBRK,  CLSBRK, VERGUL, SNMEM};
int right_tc = 19; /*  token count  */

int Numb[] = {MULT, DEL,  PRCT,  POSSIG,  NEGSIG, ARYCLS, CLSBRK, VERGUL , NOQTEV,  -1};
int Float[] = {MULT, DEL,  PRCT,  POSSIG,  NEGSIG, ARYCLS, CLSBRK, VERGUL,NOQTEV,  -1};
int Idt[] = {MULT, DEL,  PRCT,  POSSIG,  NEGSIG, ARYCLS, CLSBRK, VERGUL, ARYOPN, OPNBRK, ASGN,NOQTEV,  -1};
int Crg1[] = {MULT, DEL,  PRCT,  POSSIG,  NEGSIG, ARYCLS, CLSBRK, VERGUL, ARYOPN, ASGN, NOQTEV,  -1};
int Crg2[] = {MULT, DEL,  PRCT,  POSSIG,  NEGSIG, ARYCLS, CLSBRK, VERGUL, ASGN,  NOQTEV,  -1};
int Expr[] = {MULT, DEL,  PRCT,  POSSIG,  NEGSIG, ARYCLS, CLSBRK, VERGUL,  NOQTEV,  -1};
int Strmem[] = {MULT, DEL,  PRCT,  POSSIG,  NEGSIG, ARYCLS, CLSBRK, VERGUL, ARYOPN, OPNBRK, ASGN,  NOQTEV,  -1};
int Snmem[] = {NOQTEV, -1};
int Fcall1[] = {NOQTEV, -1};
int Dot[] = { IDT, CRG1, CRG2 , -1};
int Aryopn[] = {NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, OPNBRK, -1};
int Arycls[] = {ARYOPN,  MULT, DEL,  PRCT,  POSSIG,  NEGSIG, CLSBRK, VERGUL, DOT, ASGN,   NOQTEV, -1};
int Mult[] = {OPNBRK, NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, -1};
int Del[] = {OPNBRK, NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, -1};
int Prct[] = {OPNBRK, NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, -1};
int Possig[] = {OPNBRK, NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, -1};
int Negsig[] = {OPNBRK, NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, -1};
int Opnbrk[] = {OPNBRK, NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, -1};
int Clsbrk[] = {CLSBRK, ARYCLS, MULT, DEL,  PRCT,  POSSIG,  NEGSIG, VERGUL,  NOQTEV,  -1};
int Vergul[] = {NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, -1};
int Asgn[] = {NUMB, FLOAT, IDT, CRG1, CRG2, EXPR, STRMEM, OPNBRK, -1};

int *follow[MAX_STATES];

void init_follow(){

  follow[NUMB] = Numb;
  follow[FLOAT] = Float;
  follow[IDT] = Idt;
  follow[CRG1] = Crg1;
  follow[CRG2] = Crg2;
  follow[EXPR] = Expr;
  follow[STRMEM] = Strmem;
  follow[SNMEM] = Snmem;
  follow[FCALL1] = Fcall1;
  follow[DOT] = Dot;
  follow[ARYOPN] = Aryopn;
  follow[ARYCLS] = Arycls;
  follow[MULT] = Mult;
  follow[DEL] = Del;
  follow[PRCT] = Prct;
  follow[POSSIG] = Possig;
  follow[NEGSIG] = Negsig;
  follow[OPNBRK] = Opnbrk;
  follow[CLSBRK] = Clsbrk;
  follow[VERGUL] = Vergul;
  follow[ASGN] = Asgn;
}

void check_follow_single(int c, int d){

  int i, *p;

  p = follow[c];

  while (*p != -1){
    if (*p == d)
      return;
    p++;
  }

  printf("\n\tnot correct token ");
  print_tok(d);
  printf(" following ");
  print_tok(c);
  printf("\n");
}

void check_follow(int beg, int end){

  int i;

  for (i=beg; i<=end; ++i)
    check_follow_single(lexems[i].id, lexems[i+1].id);
}

void init_lexems(struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;

  while(tll!=NULL){
    lexems[lct].id = tll->id;
    lexems[lct].tok = tll->tok;
    lct++;
    tll = tll->next;
  }
}

void print_lexems(){

  int i;

  /*  printf("\n\tLexems\n\t\t");*/

  for (i=0; i<lct; ++i){
    print_tok(lexems[i].id);
    printf("  ");
  }

  printf("\n");
}



  

int filebeg(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == FILEBEG)
   return 1;

  return 0;
}


int fileson(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == FILESON)
   return 1;

  return 0;
}



int S(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == START)
   return 1;

  return 0;
}




int numb(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == NUMB)
   return 1;

  return 0;
}



int noqtev(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == NOQTEV)
   return 1;

  return 0;
}


static int idt(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == IDT)
   return 1;

  return 0;
}


static int crg1(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == CRG1)
   return 1;

  return 0;
}


static int crg2(int pos){

  int a;   

  if ((lct - pos)<1)
    return 0;

  a = lexems[pos].id;

 if (a == CRG2)
   return 1;

  return 0;
}


int variable(int pos){

 if (idt(pos) || crg1(pos) || crg2(pos))
   return 1;

 return 0;
}

int expression(int pos){

  if (idt(pos) || crg1(pos) || crg2(pos) || numb(pos))
    return 1;

  return 0;
}


int get_asgn_pos(int beg){

  int i;

  for (i= beg -1; i<lct; ++i)
    if (lexems[i].id == ASGN)
      return i;

  return -1;
}

int upleft_asgn(int apos){

  int i;

  for (i = apos - 1; i >= 0; --i)
    if (not_in_left_asgn(lexems[i].id))
      return i+1;

  return i;
}

int not_in_left_asgn(int tk){

  int i;

  for (i=0; i<left_tc; ++i)
    if (left_tks[i] == tk )
      return 0;

  return 1; 
}


int not_in_right_asgn(int tk){

  int i;

  for (i=0; i<right_tc; ++i)
    if (right_tks[i] == tk )
      return 0;

  return 1; 
}

int upright_asgn(int apos){

  int i;

  for (i = apos + 1; i < lct; ++i)
    if (not_in_right_asgn(lexems[i].id))
      return i - 1;

  return i;
}

void print_asgn(int beg, int end){

  int i;

  printf("Tokens of ASGN opr\n\t\t");

  for (i=beg; i<=end; ++i){
    print_tok(lexems[i].id);
    printf("\t");
  }

  printf("\n");
}

int left_asgn_ok(int upl, int apos){

  if ((upl == apos - 1) && \
      (match_ia(lexems[upl].id) || matchid(lexems[upl].id,STRMEM)))
    return 1;

  return 0;
}



int right_asgn_ok(int apos, int upr){

  int c = lexems[upr].id;

  if ((upr == apos + 1) && \
      (match_inacf(c)    || matchid(c, EXPR) || matchid(c, FCALL1) ||\
       matchid(c, SNMEM) || matchid(c, STRMEM))  && \
       matchid(lexems[upr+1].id, NOQTEV))
    return 1;

  return 0;
}

void check_left_side(int upl, int apos){

  if (!left_asgn_ok(upl, apos)){
   print_asgn(upl, apos);
   check_follow(upl, apos);
  }
}


void check_right_side(int apos, int upr){

  if (!right_asgn_ok(apos, upr)){
    print_asgn(apos, upr);
    check_follow(apos, upr);
  }
}


int check_asgn(){

  int upl, upr, apos, beg;

  beg = 0;

  apos = get_asgn_pos(beg);

  if (apos < 0)
    return; /* there is no more ASGN   */

  upl  = upleft_asgn(apos);
  upr  = upright_asgn(apos);

  check_left_side(upl, apos);
  check_right_side(apos, upr);
}



int start(int pos){

  int i;

  if (filebeg(pos) && S(pos+1) && fileson(pos+2)){
    printf("No errors, text is clean.\n");
    printf("Parsing done properly.\n");
    return 1;
  }

  return 0;
}

void check_errors(struct tok_list *tree){

  int pos;

  pos = 0; 

  /*
  init_lexems(tree);
  init_follow();
  print_lexems();
  
  if (!start(pos)){
    printf("\nNot Parsed! Please check syntax errors.\n");
    check_asgn();
    exit(0);
  }
  */

  if (!(tree->first->id == FILEBEG && tree->first->next->id == START &&	\
	tree->first->next->next->id == FILESON)){
    printf("Sintaksis sehvlerim movcuddur, sehvleri duzeldin ve proqrami tekrar kompilyasiya edin.\n");
    exit(0);
  }
    

  /* generate_kod();  */
}


#endif