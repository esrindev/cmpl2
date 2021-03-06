
 

#include "esrin.h"
#include "sehvler.h"

#include <conio.h>

extern char * lines[];

struct parse_tree ptree_holder, *ptree = &ptree_holder ;

int prg_found = 0;

struct tok_list *tree;



#define MAX_HALS 100

struct sec{
  struct tok_list_el *hals[MAX_HALS];
  int size;
} sec_cont, *sec;

struct tok_list_el *create_tok_list_el(struct token *tok);
int parser_core();

extern void add_to_errors(int row, int col, char *msg);


extern void insert_tll_next(struct tok_list_el *ptll, int id); 
extern void check_parse_errors();

extern void sehvler_yoxla();
 

int matchid(int k, int tok){

  return ( k  == tok);

}

 

/* match against id , ary  */
int match_ia(int k){

  return matchid(k, IDT) || matchid(k, CRG1) || matchid(k, CRG2) ;
  
 }

/* match against id , nb or ary, ary will be implemented in  future  */
int match_ina(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2);
  
 }

/* match against id , nb or ary, ary will be implemented in  future  */
int match_inaf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2)  || matchid(k, FLOAT) ;
  
 }


int match_inacf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, FLOAT) ;
  
 }

int match_inacsfE(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT)   || matchid(k, EXPR) ;
  
 }
 


int match_lp_opr(int k){

  return (matchid(k, POSSIG) ||  matchid(k, NEGSIG)) ;

}


int match_hp_opr(int k){

  return ( matchid(k, MULT) ||  matchid(k, DEL) ||  matchid(k, PRCT)) ;

}


int match_cond(int k){

  return (matchid(k, GRT) ||  matchid(k, LES) ||  matchid(k, BERABER) ||  matchid(k, FERQLI) ||  matchid(k, GRTEQU) ||   matchid(k, LESEQU));
 
  /*	      ||   matchid(k, VE)  ||   matchid(k, VEYA)) ; */

}


int sim_com_blk(int tok){

	return (tok == SIMPLE_OPER || tok == COMP_OPER || tok == BLOCK) ;

}

int cons_funk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

 
  if (matchid(a,FUNKSIYA) && matchid(b, FCALL1) && matchid(c, BLOCK)){
       ptr->id = FUNK;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int cons_kod_funk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

 
  if ((matchid(a,FUNK)|| matchid(a,KOD_PART) || sim_com_blk(a) ||matchid(a,KOD_FUNK)) && \
	  (matchid(b,FUNK)|| matchid(b,KOD_PART) || sim_com_blk(b) ||matchid(b,KOD_FUNK))){
       ptr->id = KOD_FUNK;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}






/* convert { kod_part } into block */
int cons_block_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,k;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;


  k = 0;

  if (matchid(a,OPNBLK) && (matchid(b,KOD_PART) || sim_com_blk(b)) && matchid(c,CLSBLK))
   goto L3;

  if (matchid(a,OPNBLK) && matchid(b,CLSBLK))  
   goto L2;


 goto out;

  k = 0;
  ptr->next = NULL;

 L3:
    ptr->childs[2] = tll->next->next;
    k++;
    if (ptr->next == NULL)
		ptr->next = tll->next->next->next; 
 L2:

       ptr->id  = BLOCK;
	   ptr->childs[0] = tll;
	   ptr->childs[1] = tll->next;

       k+= 2;
       ptr->cld_cnt   = k;
	   ptr->lrb = 1; //branch

       if (ptr->next == NULL)
		   ptr->next = tll->next->next; 
	   tllp->next = ptr;  //!!!!!!!!!! do not consume fist tll, because it is for check
       *tll_ref = ptr;
    (*pos)++;
    return 1;
out:
  return 0;
}





int cons_kod_part(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

 
  if ( (sim_com_blk(a) || matchid(a, KOD_PART) || matchid(a, EGER_OPER)) && \
	   (sim_com_blk(b) || matchid(b, KOD_PART) || matchid(a, EGER_OPER)) ){
       ptr->id = KOD_PART;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next; 
       ptr->cld_cnt = 2;
       ptr->lrb = 1; //branch

       ptr->next = tll->next->next; 
       tllp->next = ptr;

       *tll_ref = ptr;
        (*pos)++;

      return 1;
   }
  return 0;
}



int cons_eger_yoxsa_opr_full(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;
 
  if (matchid(a,EGER_EXP2) && sim_com_blk(b) && matchid(c,YOXSA) && sim_com_blk(d)){
       ptr->id = COMP_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}

int cons_eger_yoxsa(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

 
  if (matchid(a,EGER_EXP2) && sim_com_blk(b) && matchid(c,YOXSA)){
       ptr->id = EGER_YOXSA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



int cons_eger_yoxsa_opr_1(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

 
  if (matchid(a,EGER_EXP2) && sim_com_blk(b) && matchid(c,YOXSA_OPER) ){
       ptr->id = COMP_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



int cons_eger_yoxsa_opr_2(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;


  if (matchid(a,EGER_YOXSA) && sim_com_blk(b)){
       ptr->id = COMP_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}






 
int cons_yoxsa_opr(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;
 
  if (matchid(a,YOXSA) && sim_com_blk(b)){
       ptr->id = YOXSA_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
	   ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}

int cons_eger_opr(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,EGER_EXP2) && sim_com_blk(b)){
       ptr->id = COMP_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
	   ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


/* convert neqederki koddata into koddata */
int cons_neqederki_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,NEQEDERKI2) && sim_com_blk(b)){
       ptr->id = COMP_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


/* convert  koddata into koddata */
int cons_secim_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,k;   

  k = 0;


  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;

   if (matchid(a,SEC1) && matchid(b, OPNBLK) &&  matchid(c, HAL2) &&  matchid(d, CLSBLK)){

       ptr->id = COMP_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next =  tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

       return 1;
      }
 
  return 0;
}


int cons_say_123_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;
 
  if ((matchid(a,SAY_TIP_1) || matchid(a,SAY_TIP_2) || matchid(a,SAY_TIP_3)) && sim_com_blk(b)){
       ptr->id = COMP_OPER;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



int cons_neqederki1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,NEQEDERKI) && matchid(b,BRK_SHERT)){
       ptr->id = NEQEDERKI2;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;

       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}



int cons_sec1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SEC) && matchid(b,BRK_VAL)){
       ptr->id = SEC1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;

       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}

/* convert EGEREXP1 SHERT ) into EGER_exp2  */
int cons_eger_exp1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,EGER) && matchid(b,BRK_SHERT)){
       ptr->id =  EGER_EXP2;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;

       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next;
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int cons_davamet_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;
  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;
 
  if (matchid(a,DAVAMET) && matchid(b,NOQTEV)){

       ptr->id = DAVAMET1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}




int cons_dayan_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;
  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;
 
  if (matchid(a,DAYAN) && matchid(b,NOQTEV)){

       ptr->id = DAYAN1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}









int cons_say_tip_1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d; 

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;  

  if (matchid(a,SAY) && (matchid(b, BRK_VAL)||matchid(b, BRK_SEQ))){
    ptr->id = SAY_TIP_1;
	ptr->childs[0] = tll;
	ptr->childs[1] = tll->next;
	ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

	 return 1;
    }

  return 0;
}




int cons_asgn_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;  

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0; 

  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0; 

  if (match_ia(a) && matchid(b,ASGN) && (match_inacsfE(c)|| matchid(c, BRK_VAL) ||  matchid(c, FCALL1)) && matchid(d,NOQTEV)){
    ptr->id  = ASGN_OP;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->childs[3] = tll->next->next->next;
    ptr->cld_cnt   = 4;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next =  tll->next->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}







int cons_hal1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,k;   

  k = 0;


  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next != NULL)
	  c = tll->next->next->id;
  else
	  return 0;

  if (matchid(a,HAL) && (matchid(b,NUMB) || matchid(b, CHR)) && matchid(c, COLON)){
    ptr->id  = HAL1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;
    
    return 1;
  }
  
  return 0;
}

int cons_vdec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,k;   

  k = 0;

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,TAM) && ( match_ia(b) || matchid(b, SEQ))){
		ptr->id  = VDEC;
		ptr->childs[0] = tll;
		ptr->childs[1] = tll->next;
		ptr->cld_cnt   = 2;
		ptr->lrb = 1; //branch

		tllp->next = ptr;
		ptr->next = tll->next->next; 
		*tll_ref = ptr;
		(*pos)++;
	    
		return 1;
  }
  
  return 0;
}


int cons_vdec_kod_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,k;   

  k = 0;

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a, VDEC) && matchid(b, NOQTEV)){
		ptr->id  = SIMPLE_OPER;
		ptr->childs[0] = tll;
		ptr->childs[1] = tll->next;
		ptr->cld_cnt   = 2;
		ptr->lrb = 1; //branch

		tllp->next = ptr;
		ptr->next = tll->next->next; 
		*tll_ref = ptr;
		(*pos)++;
	    
		return 1;
  }
  
  return 0;
}


int cons_var_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,k;   

  k = 0;

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,TAM) || matchid(a,KESR) || matchid(a,SIMVOL)) && \
	  (matchid(b,SEQ) || match_ia(b))){
		ptr->id  = VAR;
		ptr->childs[0] = tll;
		ptr->childs[1] = tll->next;
		ptr->cld_cnt   = 2;
		ptr->lrb = 1; //branch

		tllp->next = ptr;
		ptr->next = tll->next->next; 
		*tll_ref = ptr;
		(*pos)++;
	    
		return 1;
  }
  
  return 0;
}

int cons_sus1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SUSMA) && matchid(b, COLON) ){
    ptr->id  = SUS1;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;
       
    return 1;
  }
  
  return 0;
}

int cons_hal1s_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,HAL1) || matchid(a, HAL1S))&& (matchid(b, HAL1)||matchid(b, HAL1S))){
    ptr->id  = HAL1S;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;
       
    return 1;
  }
  
  return 0;
}



int cons_hal2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */


  int a,b,c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (((matchid(a,HAL1) || matchid(a, HAL1S) || matchid(a, SUS1)) && \
       (matchid(b,KOD_PART) || sim_com_blk(b))) || \
       (matchid(a, HAL2) && matchid(b, HAL2))   || \
	   (matchid(a, HAL2) && (matchid(b,KOD_PART) || sim_com_blk(b)))){
    ptr->id  = HAL2;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int cons_brk_emp_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,     int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,OPNBRK) && matchid(b, CLSBRK)){
    ptr->id  = BRK_EMP;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next; 
    tllp->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}




/* consume  (TEXT | SINIF_TYPE | KOD_FUNK | FUNK | KODDATA) &&
            (TEXT | SINIF_TYPE | KOD_FUNK | FUNK | KODDATA)
   into TEXT  
*/
int cons_text_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,k;   

  k = 0;


  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

 if ((matchid(a,TEXT) || matchid(a,KOD_FUNK) || matchid(a,FUNK) || matchid(a,KOD_PART) || sim_com_blk(a)) && \
	 (matchid(b,TEXT) || matchid(b,KOD_FUNK) || matchid(b,FUNK) || matchid(b,KOD_PART) || sim_com_blk(b)))
     goto L2;

 if (matchid(a,KOD_FUNK) || matchid(a,FUNK) || matchid(a,KOD_PART) || sim_com_blk(a))
   goto L1;

 goto out;

 ptr->next = NULL;

 L2:
    ptr->childs[1] = tll->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next; 
 L1:

       ptr->id  = TEXT;
       ptr->childs[0] = tll;
       k+= 1;
       ptr->cld_cnt   = k;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
	 ptr->next = tll->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:
  return 0;
}


int cons_fcall1_op_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,FCALL1) && matchid(b,NOQTEV)){
    ptr->id  = SIMPLE_OPER;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


/* consume  SHERT ve SHERT into SHERT   */
int cons_vveya_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

 if ((matchid(a,SHERT) || matchid(a,BRK_SHERT)) && (matchid(b,VE)||matchid(b,VEYA)) && (matchid(c,SHERT) || matchid(c,BRK_SHERT))){
	    ptr->id  = SHERT;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}


 
int cons_seq_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

 if ((match_inacsfE(a) || matchid(a,BRK_VAL) || matchid(a,SEQ)) && \
	  matchid(b,VERGUL)&& \
	 (match_inacsfE(c) || matchid(c,BRK_VAL) || matchid(c,SEQ))){
	    ptr->id  = SEQ;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}


/* cons  x > y into SHERT */
int cons_cond_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if ((match_inacf(a)|| matchid(a, EXPR) || matchid(a, BRK_VAL)) && match_cond(b) && (match_inacf(c)|| matchid(c, EXPR)  || matchid(c, BRK_VAL) )){
       ptr->id  = SHERT; 
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}


 
 
int cons_brk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (matchid(a,OPNBRK) &&  matchid(c,CLSBRK)){ 	   
    
	  switch(b){
	  case IDT:
	  case CRG1:
	  case CRG2:
	  case NUMB:
	  case FLOAT:
	  case CHR:
	  case EXPR:
	  case STR:
	  case BRK_VAL:
		  ptr->id  = BRK_VAL;		  
		  ptr->childs[0] = tll;
		  ptr->childs[1] = tll->next;
		  ptr->childs[2] = tll->next->next;
		  ptr->cld_cnt   = 3;
		  ptr->lrb = 1; //branch
		  tllp->next = ptr;
		  ptr->next = tll->next->next->next; 
		  *tll_ref = ptr;
		  (*pos)++;
		return 1;
	  case SHERT:
	  case BRK_SHERT:
		  ptr->id  = BRK_SHERT;		  
		  ptr->childs[0] = tll;
		  ptr->childs[1] = tll->next;
		  ptr->childs[2] = tll->next->next;
		  ptr->cld_cnt   = 3;
		  ptr->lrb = 1; //branch
		  tllp->next = ptr;
		  ptr->next = tll->next->next->next; 
		  *tll_ref = ptr;
		  (*pos)++;
		return 1;
	  case SEQ:
	  case BRK_SEQ:
		  ptr->id  = BRK_SEQ;		  
		  ptr->childs[0] = tll;
		  ptr->childs[1] = tll->next;
		  ptr->childs[2] = tll->next->next;
		  ptr->cld_cnt   = 3;
		  ptr->lrb = 1; //branch
		  tllp->next = ptr;
		  ptr->next = tll->next->next->next; 
		  *tll_ref = ptr;
		  (*pos)++;
		return 1;
	  case BRK_EMP:
		  ptr->id  = BRK_EMP;		  
		  ptr->childs[0] = tll;
		  ptr->childs[1] = tll->next;
		  ptr->childs[2] = tll->next->next;
		  ptr->cld_cnt   = 3;
		  ptr->lrb = 1; //branch
		  tllp->next = ptr;
		  ptr->next = tll->next->next->next; 
		  *tll_ref = ptr;
		  (*pos)++;
		return 1;
	  default:
		  printf("Error: unexpected token inside bracket;\n");
	  }
  }

  return 0;
}



 
int cons_arybrk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;


  if (matchid(a,ARYOPN) &&  matchid(c,ARYCLS)){ 	   
    
	  switch(b){
	  case IDT:
	  case CRG1:
	  case CRG2:
	  case NUMB:
	  case FLOAT:
	  case CHR:
	  case EXPR:
		  ptr->id  =  ARYBRK_VAL;
		  ptr->childs[0] = tll;
		  ptr->childs[1] = tll->next;
		  ptr->childs[2] = tll->next->next;
		  ptr->cld_cnt   = 3;
		  ptr->lrb = 1; //branch
		  tllp->next = ptr;
		  ptr->next = tll->next->next->next; 
		  *tll_ref = ptr;
		  (*pos)++;
		return 1;
	  default:
		  printf("Error: unexpected token inside array bracket;\n");
	  }
  }

  return 0;
}


/* consume on kod  */
int cons_crg2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;  

  if (matchid(a,CRG1) && matchid(b,ARYBRK_VAL)){ 

    ptr->id  = CRG2; /* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next =  tll->next->next; 
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int cons_crg1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;


 if (matchid(a,IDT) && matchid(b,ARYBRK_VAL)){
	   ptr->id  = CRG1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}

  
 


int cons_decrement_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b, c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if ((match_ia(a) && matchid(b, DECREMENT) && matchid(c, NOQTEV) ) ||\
	  (matchid(a, DECREMENT) && match_ia(b) && matchid(c, NOQTEV) )){
	   ptr->id  = DECREMENT_OPR;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
	   ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
	   ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}



int cons_increment_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b, c;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if ((match_ia(a) && matchid(b, INCREMENT) && matchid(c, NOQTEV) ) ||\
	  (matchid(a, INCREMENT) && match_ia(b) && matchid(c, NOQTEV) )){
	   ptr->id  = INCREMENT_OPR;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
	   ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
	   ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;
    return 1;
 }

  return 0;
}


int cons_qaytar_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,k;   

  k = 0;


  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (matchid(a, QAYTAR) && (match_inacsfE(b) || matchid(b, BRK_VAL)) && matchid(c, NOQTEV))
   goto L3;

 if (matchid(a, QAYTAR) && matchid(b, NOQTEV))
   goto L2;

 goto out;

      k = 0;
  ptr->next = NULL;

 L3:
    ptr->childs[2] = tll->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next; 
 L2:

       ptr->id  = QAYTAR1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;

       k+= 2;
       ptr->cld_cnt   = k;
	   ptr->lrb = 1; //branch

       tllp->next = ptr;
       if (ptr->next == NULL)
	 ptr->next = tll->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 
 out:

  return 0;
}
 

int cons_fcall1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a, IDT) && (matchid(b, BRK_EMP) || matchid(b, BRK_VAL) || matchid(b, BRK_SEQ))){
		ptr->id  =  FCALL1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 }

  return 0;
}




int cons_plus_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;


  if ((match_inaf(a) || matchid(a, EXPR) || matchid(a, BRK_VAL)) && \
	   match_lp_opr(b) && \
	  (match_inaf(c) || matchid(c, EXPR) || matchid(c, BRK_VAL) )){
		ptr->id  = EXPR;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 }

  return 0;
}



int cons_mult_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c;   

 if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;


  if ((match_inaf(a) || matchid(a, EXPR) || matchid(a, BRK_VAL)) && \
	   match_hp_opr(b) && \
	  (match_inaf(c) || matchid(c, EXPR) || matchid(c, BRK_VAL) )){
		ptr->id  = EXPR;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next = tll->next->next->next; 
       *tll_ref = ptr;
    (*pos)++;
    return 1;
 }

  return 0;
}




//consume simple opers to SIMPLE_OPER
int cons_convert_kod_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a, DAXILET3)		||  matchid(a, CAPET3)				||  \
      matchid(a, ASGN_OP)		||  matchid(a, DAVAMET1)			||  \
	  matchid(a, DAYAN1)		||  matchid(a, QAYTAR1)				||  \
	  matchid(a, STANDART_FUNK)	||	\
	  matchid(a, STEKE_QOY_STD) ||  matchid(a, STEKDEN_GOTUR_STD)	||	\
	  matchid(a, DECREMENT_OPR)	||	matchid(a, INCREMENT_OPR))
  {
    ptr->id  = SIMPLE_OPER; 
    ptr->childs[0] = tll;
    ptr->cld_cnt   = 1;

    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next; 
    *tll_ref = ptr;
    (*pos)++;
    return 1;
 }

  return 0;
}

/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////////////////////////////////////////////*/


void convert_cur_layer_kod (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 // tll = tree->count; /* how many elemnts 'r one current layer  */
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_convert_kod_tok(ptr, &tll,   &pos))
      continue; 

    tll = tll->next;

  }

}

 

void  convert_to_koddata_dca(struct tok_list *tree){

	convert_cur_layer_kod(tree);
}


int build_cur_layer_kod_funk (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

	if (cons_funk_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_kod_funk_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}



int build_cur_layer_eger (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_eger_yoxsa_opr_full (ptr, &tll,   &pos))
      continue; 

	if (cons_eger_yoxsa (ptr, &tll,   &pos))
      continue; 

    if (cons_eger_yoxsa_opr_1 (ptr, &tll,   &pos))
      continue; 

    if (cons_yoxsa_opr(ptr, &tll,   &pos))
      continue; 

    if (cons_eger_yoxsa_opr_2 (ptr, &tll,   &pos))
      continue; 

    if (cons_eger_opr(ptr, &tll,   &pos))
      continue; 

  tll = tll->next;

   }

  return pos;
}


int build_cur_layer_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_say_123_tok(ptr, &tll,   &pos))
      continue; 	

	if (cons_neqederki_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_hal2_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_secim_tok(ptr, &tll,   &pos))
      continue; 

  tll = tll->next;

   }

  return pos;
}


int build_cur_layer_blk (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */
	

    if (cons_block_tok(ptr, &tll,   &pos))
      continue;


  tll = tll->next;

   }

  return pos;
}

int build_cur_layer_kod (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */
	

    if (cons_kod_part(ptr, &tll,   &pos))
      continue;


  tll = tll->next;

   }

  return pos;
}


int build_cur_layer_dax1_cap1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;     

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

   if (cons_vdec_tok(ptr, &tll,   &pos))
      continue;

   if (cons_hal1s_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }

  return pos;
}


int build_cur_layer_dax_cap (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
  pos = 0;      /* how many elements scanned so far  */

   while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

     if (cons_hal1_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_sus1_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_davamet_tok(ptr, &tll, &pos))
      continue; 

    if (cons_dayan_tok(ptr, &tll,  &pos))
      continue; 

     if (cons_brk_emp_tok(ptr, &tll,  &pos))
      continue;

   tll = tll->next;

  }

  return pos;
}


int build_cur_layer_simple_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  /*
  cons_birtips(bpt1, bpt2, &bptc);
  cons_tiplers(bpt2, bpt1, &bptc);
  cons_var(bpt1, bpt2, &bptc);
*/  


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

  if (cons_asgn_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_decrement_tok(ptr, &tll,   &pos)) 
	   continue;

  if (cons_increment_tok(ptr, &tll,   &pos)) 
	   continue;

  if (cons_say_tip_1_tok(ptr, &tll,   &pos))
      continue; 
 

  /* shertin consumunda sintaksis telebi olan moterezelerde consume\
     olundugundan eger 2 fazada cons olunur daxil ve capet operatorlari kimi  */
 if (cons_eger_exp1_tok(ptr, &tll,   &pos))
      continue; 

 if (cons_neqederki1_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_sec1_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_fcall1_op_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_vdec_kod_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_qaytar_tok(ptr, &tll,   &pos))
      continue; 


   tll = tll->next;
  }

  return pos;
}

 
int build_cur_layer_text (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_text_tok(ptr, &tll,  &pos))
      continue; 
    
   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_fcall1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_fcall1_tok(ptr, &tll,   &pos))
      continue;


   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_lp_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_plus_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}



int build_cur_layer_hp_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_crg1_tok(ptr, &tll,   &pos))
      continue;  
    if (cons_crg2_tok(ptr, &tll,   &pos))
     continue;
    if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
	if (cons_arybrk_tok(ptr, &tll,   &pos))
      continue;
    if (cons_mult_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}




int build_cur_layer_cnds (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_cond_tok(ptr, &tll,   &pos))
      continue;
   if (cons_vveya_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_seq (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

   if (cons_seq_tok(ptr, &tll,   &pos))
      continue;

   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}



 
void bottom_up_parse_dax_cap(struct tok_list *tree){

	while(build_cur_layer_dax_cap(tree));    
}



void bottom_up_parse_text(struct tok_list *tree){

 
    while(build_cur_layer_text(tree));
}


 


void bottom_up_parse_kod_funk(struct tok_list *tree){


   // while(build_cur_layer_funk(tree));
	while(build_cur_layer_kod_funk(tree));
}

 



void bottom_up_parse_kod_or_var(struct tok_list *tree){

	int k;

 loop:

	k = 0;

    k =  build_cur_layer_eger(tree);
	//printf("_eger_____________________________________\n");
	//print_tree_dx(tree);
    if (k>0) 
      goto loop;

	k = 0;

    k =  build_cur_layer_oprs(tree);
	//printf("_oprs_____________________________________\n");
	//print_tree_dx(tree);
    if (k>0) 
      goto loop;

	k = 0;

   k = build_cur_layer_blk(tree);
	//printf("_blk__________________________________\n");
	//print_tree_dx(tree);
   if (k>0)
	 goto loop;

	k = 0;

   k = build_cur_layer_kod(tree);
	//printf("_kod__________________________________\n");
	//print_tree_dx(tree);
   if (k>0)
	 goto loop;

}

void bottom_up_parse_dax1_cap1(struct tok_list *tree){
 
    while(build_cur_layer_dax1_cap1(tree));    
}




void bottom_up_parse_exprs(struct tok_list *tree){

	int k;

 loop:

	k = 0;

    k = build_cur_layer_hp_oprs(tree);
    if (k>0) 
      goto loop;

	k = 0;

    k = build_cur_layer_lp_oprs(tree);
    if (k>0)
      goto loop;

}

void bottom_up_parse_fcall(struct tok_list *tree){

   build_cur_layer_fcall1(tree);

}



void bottom_up_parse_cnds(struct tok_list *tree){
 
    while(build_cur_layer_cnds(tree));
    
}


void bottom_up_parse_seq(struct tok_list *tree){
 
    while(build_cur_layer_seq(tree));
    
}


void bottom_up_parse_simple_oprs(struct tok_list *tree){
 
    while(build_cur_layer_simple_oprs(tree));
    
}



void print_by_kwd();
void init_context_left();
void print_by_context();
void print_only_sinif_and_funcs();
void print_ast_list(struct AST_list *ast_list);
/* we accept text if it is parsed i.e. TEXT is on top, or
   it is empty i.e.  user typed no kod */

  int res = 0;



void init_parser(struct tok_list **tree){

  *tree = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  (*tree)->first = (*tree)->last = NULL;
  (*tree)->count = 0;

  init_bottom_layer_new(*tree);
}


int parser_core(struct tok_list *tree){

  bottom_up_parse_exprs(tree);

  bottom_up_parse_dax_cap(tree);
 
  bottom_up_parse_cnds(tree);

  bottom_up_parse_seq(tree);

  bottom_up_parse_fcall(tree);

  bottom_up_parse_dax1_cap1(tree); 
 
  bottom_up_parse_simple_oprs(tree);
 
  convert_to_koddata_dca(tree);

  bottom_up_parse_kod_or_var(tree);

  bottom_up_parse_kod_funk(tree);

 // bottom_up_parse_text(tree);

	 return 0;
}

int parse(){

	init_parser(&tree);
	parser_core(tree);

	print_tree_dx(tree);

 	// Sleep(20000);
    //check_parse_errors();

	//sehvler_yoxla();

	exit(0);

	return 0;

}

/*
=================================================================================================

                          BUILDING VERY NEW PARSER FOR BETTER ERROR HANDLING

=================================================================================================
*/

/* init trees tok_list_els (laytest layer) prev links 
   and trees last nodes reference  */
static void init_prev_links(struct tok_list *tree){

	struct tok_list_el *prev, *tll = tree->first;

	tll->prev = NULL; //tree->first; /* Should be FILEBEG  */

	while(tll->id != FILESON){
		prev = tll;
		tll = tll->next;
		tll->prev = prev;	
	}

	tree->last = tll->next; /* one more FILESON reserve  */
	tree->last->prev = tll;
	tree->last->next = NULL;
}


int is_key_tok(int id){

	switch (id){
	case SAY:
	case NEQEDERKI:
	case EGER:
	case SEC:
	case HAL:
	case SUSMA:
	case FUNKSIYA:
	case QAYTAR:
	case TAM:
	case KESR:
	case SIMVOL:
	case ASGN:
	case DAYAN:
	case DAVAMET:
	case NOQTEV:
	case OPNBLK:
	case CLSBLK:
	case YOXSA:
		return 1;
	default:
		return 0;
	}
}


int get_head_id(int id){

	switch(id){
	case SAY:
		return SAY_HEAD;
	case NEQEDERKI:
		return NEQEDERKI_HEAD;
	case EGER:
		return EGER_HEAD;
	case SEC:
		return SEC_HEAD;
	case HAL:
		return HAL_HEAD;
	case SUSMA:
		return SUSMA_HEAD;
	case FUNKSIYA:
		return FUNKSIYA_HEAD;
	case QAYTAR:
		return QAYTAR_HEAD;
	case TAM:
		return TAM_HEAD;
	case KESR:
		return KESR_HEAD;
	case SIMVOL:
		return SIMVOL_HEAD;
	}

}

void add_to_childs(struct tok_list_el *ptll, struct tok_list_el *tll){

	ptll->childs[ptll->cld_cnt] = tll;
	ptll->cld_cnt++;

}

struct tok_list_el * cons_keys_head(struct tok_list_el *src_tll){

	struct tok_list_el *tll,*ptll, *tmp;

	tll = src_tll;
	ptll = create_tok_list_el(tll->tok);
	ptll->id = get_head_id(tll->id);
	ptll->lrb = 1; 

	//save tll init pos
	tmp = tll;
	tll = tll->next;

	int i=0;

 	while(!is_key_tok(tll->id) && tll->id != FILESON){
		add_to_childs(ptll, tll);
		tll = tll->next;

		if (i==100)
			break;

		i++;
	}

	tmp->prev->next = ptll;
	ptll->next = tll;
	ptll->prev = tmp->prev;
	tll->prev = ptll;    

	return ptll;
}

struct tok_list_el * cons_key_childs(struct tok_list_el *tll){

	switch(tll->id){
	case SAY:
	case NEQEDERKI:
	case EGER:
	case SEC:
	case HAL:
	case SUSMA:
	case FUNKSIYA:
	case QAYTAR:
	case TAM:
	case KESR:
	case SIMVOL:
		return cons_keys_head(tll);
	case DAYAN:
	case DAVAMET:
		//cons_keys_dayan(tll);
		break;
	case ASGN:
		//cons_keys_asgn(tll);
		break;
	case NOQTEV:
		//cons_keys_noqtev(tll);
		break;
	}

	return tll->next;
}

void build_parse_tree(struct tok_list *tree){

	struct tok_list_el *tll;

	tll = tree->first->next;

	int j = 0;

	while(tll->id != FILESON){
		if (is_key_tok(tll->id))
			tll = cons_key_childs(tll);
		else
			tll = tll->next;
	
	if (j==500)
		break;
	j++;

	}
}

int parse_new(struct tok_list *tree){

	init_parser(&tree);
	init_prev_links(tree);

	build_parse_tree(tree);

	print_tree_dx(tree);

	return 0;
}


