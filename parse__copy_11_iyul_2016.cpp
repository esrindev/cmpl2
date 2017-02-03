
 

  #include "esrin.h"

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

char *pt;
int row, col; /* used to format error message */

void init_bottom_layer(struct tok_list *tree, struct token tks[]);
void add_cur_layer(struct tok_list *tree, struct layer *lr);
void init_parse_tree(struct parse_tree  **ptree, struct parse_tree ptree_holder);
void print_parse_tree(struct tok_list *tree);
struct layer *create_cur_layer();
struct tok_list_el *create_tok_list_el(struct token *tok);
int parser_core();

/* error.c  */
void  sehv(int say, char *s);
void sehv_tll_first(int say, struct tok_list_el *tll, char *s);

extern void report_to_vartab(struct variable *var, int usage);
extern void print_dec_list(struct dec_list_el dec_list[], int dec_cnt);

extern void execute(struct tok_list *tree);

FILE *fp;

/* parsing sehvleri  */
int psh = 0;

void insert_tll(struct tok_list_el *ptll, int id){

	struct tok_list_el *tll, *tmp;

	tll = create_tok_list_el(ptll->tok);

	//now insert
	tll->next = ptll->next;
	ptll->next = tll;

}

int matchid(int k, int tok){

  return ( k  == tok);

}

/* match against id , nb  */
int match_in(int k){

  return (matchid(k, IDT) || matchid(k, NUMB));
  
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

int match_inas(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, STR);
  
 }

int match_inac(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR);
  
 }


int match_inacf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, FLOAT) ;
  
 }


int match_inacs(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR);
  
 }

int match_inacsf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT) ;
  
 }


int match_inacsfE(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT)   || matchid(k, EXPR) ;
  
 }

int match_inacsfEF(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT)   || matchid(k, EXPR) || \
 matchid(k, FCALL1);
  
 }


/*
int match_opr(int k){

}
*/
int match_lp_opr(int k){

  return (matchid(k, POSSIG) ||  matchid(k, NEGSIG)) ;

}


int match_hp_opr(int k){

  return ( matchid(k, MULT) ||  matchid(k, DEL) ||  matchid(k, PRCT)) ;

}

int match_opr(int k){

  return match_lp_opr(k) || match_hp_opr(k);

}

int match_cond(int k){

  return (matchid(k, GRT) ||  matchid(k, LES) ||  matchid(k, BERABER) ||  matchid(k, FERQLI) ||  matchid(k, GRTEQU) ||   matchid(k, LESEQU));
 
  /*	      ||   matchid(k, VE)  ||   matchid(k, VEYA)) ; */

}

int match_tsky(int k){

  return (matchid(k, TAM1) || matchid(k, SIMVOL1) || matchid(k, KESR1) || matchid(k, YNT1));

}


/* our standart funks  */
int match_std_func(int k){

	return matchid(k, KVADRAT_KOK); /* later we add another std funcs also  */

}

void get_row_col_right(struct tok_list_el *tll, int *row, int *col, char **pt){

  if (tll->cld_cnt == 0){ /* reached far right leave, get row and col  */
    *row =  tll->tok->row;
    *col =  tll->tok->col;
    *pt  =  tll->tok->tok;
    return ;
  }
  
  /* not yet reached, continue by far right branch  */
  get_row_col_right(tll->childs[tll->cld_cnt-1], row, col, pt);
}

void get_row_col_left(struct tok_list_el *tll, int *row, int *col, char **pt){

  if (tll->cld_cnt == 0){ /* reached far left leave, get row and col  */
    *row = tll->tok->row;
    *col = tll->tok->col;
    *pt  =  tll->tok->tok;
    return ;
  }
  
  /* not yet reached, continue by far left branch  */
  if (tll->childs[0] != NULL)
	get_row_col_left(tll->childs[0], row, col, pt);
}

/* print expected string without surrounding ``  */
void print_before(struct tok_list_el *tll, char *msg){
    get_row_col_left(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -dan evvel %s gozlenirdi.\n", row, col, pt,msg);    
	err_cnt++;
}

void print_after(struct tok_list_el *tll, char *msg){
    get_row_col_right(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -dan sonra %s gozlenirdi.\n", row, col, pt,msg);    
	err_cnt++;
}


 
/* convert FUNKSIYA IDT (...) into FUNK_ELAN */
int cons_funk_elan_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c, k;

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

  k = 0;
  ptr->next = NULL;


  if (matchid(a,FDECL) && (matchid(b,YNT1) || matchid(b,YNT1_FUNC_ARGS)) &&  matchid(c, CLSBRK))
      goto L3;

  if (matchid(a,FDECL) && matchid(b, CLSBRK))
      goto L2;

       goto out;

  L3:
       ptr->childs[2] = tll->next->next;
       ptr->next  =  tll->next->next->next;
       k+=1;

  L2:


       ptr->id = FUNK_ELAN;
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


int check_funk_elan_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  get_row_col_right(tll, &row, &col, &pt);

  if (matchid(a,FDECL)){
    if (!(matchid(b,YNT1)||matchid(b, CLSBRK))){
      printf("Sehv:%d:%d: %s -dan sonra elan veya `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (matchid(b,YNT1)){

      get_row_col_right(tll->next, &row, &col, &pt);

      if (!matchid(c, CLSBRK)){
	printf("Sehv:%d:%d: %s -den sonra `)` gozlenirdi.\n", row, col, pt);
	err_cnt++;
      *tll_ref = tll->next->next;
      return 0;
      }
    }
  }

  return 0;
}




/* convert FUNK_ELAN FUNK_BODY into FUNK */
int cons_funk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,FUNK_ELAN) && matchid(b,BLOCK)){
       ptr->id = FUNK;
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

 
  if ((matchid(a,FUNK)||matchid(a,KODDATA)||matchid(a,KOD_FUNK)) && (matchid(b,KODDATA)||matchid(b,FUNK)||matchid(b,KOD_FUNK))){
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





/* convert { koddata } into block */
int cons_block_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,k;   

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

  k = 0;

  if (!matchid(a,IDT) && matchid(b,OPNBLK) && matchid(c,KODDATA) && matchid(d,CLSBLK))
   goto L3;

  if (!matchid(a,IDT) && matchid(b,OPNBLK) && matchid(c,CLSBLK))  
   goto L2;

 goto out;

  k = 0;
  ptr->next = NULL;

 L3:
    ptr->childs[2] = tll->next->next->next;
    k++;
    if (ptr->next == NULL)
		ptr->next = tll->next->next->next->next; 
 L2:

       ptr->id  = BLOCK;
	   ptr->childs[0] = tll->next;
	   ptr->childs[1] = tll->next->next;

       k+= 2;
       ptr->cld_cnt   = k;
	   ptr->lrb = 1; //branch

       if (ptr->next == NULL)
		   ptr->next = tll->next->next->next; 
	   tllp->next->next = ptr;  //!!!!!!!!!! do not consume fist tll, because it is for check
       *tll_ref = ptr;
    (*pos)++;
    return 1;
out:
  return 0;
}




/* convert koddata koddata into koddata */
int cons_koddata_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if ( !matchid(a, YOXSA) &&  matchid(b,KODDATA) && matchid(c,KODDATA)){
       ptr->id = KODDATA;
       ptr->childs[0] = tll->next;
       ptr->childs[1] = tll->next->next; 
       ptr->cld_cnt   = 2;
       ptr->lrb = 1; //branch

       ptr->next = tll->next->next->next; 
       tllp->next->next = ptr;

       *tll_ref = ptr;
        (*pos)++;

      return 1;
   }
  return 0;
}


/* convert eger_exp koddata yoxsa koddata into koddata */
int cons_eger_yoxsa_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,EGER_EXP2) && (matchid(b,KODDATA) || matchid(b,BLOCK)) &&  matchid(c,YOXSA) &&  (matchid(d,KODDATA) || matchid(d,BLOCK)) ){
       ptr->id = KODDATA;
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



int check_eger_yoxsa_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  get_row_col_right(tll, &row, &col, &pt);    

  if (matchid(a,EGER_EXP2)){
    if (!matchid(b,KODDATA)){
      printf("(%d:%d)sehv: `%s` -den sonra operator gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (matchid(c,YOXSA)){
      get_row_col_right(tll->next->next, &row, &col, &pt);   
      if (!matchid(d,KODDATA)){
	printf("(%d:%d)sehv: `%s` -den sonra operator gozlenirdi.\n", row, col, pt);
	err_cnt++;
      *tll_ref = tll->next->next->next;
      return 0	;
      }
    }
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

  if (matchid(a,NEQEDERKI2) && (matchid(b,KODDATA)||matchid(b,BLOCK))){
       ptr->id = KODDATA;
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


int check_neqederki_error(struct tok_list_el **tll_ref){
  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  get_row_col_right(tll, &row, &col, &pt);   

  if (matchid(a,NEQEDERKI2)){
      if (!matchid(b,KODDATA)){
	printf("(%d:%d)sehv: `%s` -den sonra operator gozlenirdi.\n", row, col, pt);
	err_cnt++;
	*tll_ref = tll->next;
	return 0;
      }
    } 

  return 0;
}


/* convert eger_exp koddata into koddata */
int cons_eger_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,EGER_EXP2) && (matchid(b,KODDATA) || matchid(b,BLOCK) ) && !matchid(c, YOXSA)){
       ptr->id = KODDATA;
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

  int a,b,c,d,e,f,k;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,SEC1) && match_ia(b) && matchid(c, CLSBRK)       &&  \
      matchid(d, OPNBLK) &&  matchid(e, HAL2) &&  matchid(f, CLSBLK)){

       ptr->id = KODDATA;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->childs[4] = tll->next->next->next->next;
       ptr->childs[5] = tll->next->next->next->next->next;
       ptr->cld_cnt   = 6;
       ptr->lrb = 1; //branch

       tllp->next = ptr;
       ptr->next =  tll->next->next->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

       return 1;
      }
 
  return 0;
}


int check_secim_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c,d,e,f;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0;

  get_row_col_right(tll, &row, &col, &pt);   

  if (matchid(a,SEC1)){
    if (!matchid(b,IDT)){
      printf("(%d:%d)sehv: `%s` -den sonra deyishen gozlenirdi.\n",row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c, CLSBRK)){
      get_row_col_right(tll->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s`-den sonra  `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next;
      return 0;      
    }
    if (! matchid(d, OPNBLK)){
      get_row_col_right(tll->next->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `{` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next;
      return 0; 
    }
    if (!matchid(e, HAL2)){
      get_row_col_right(tll->next->next->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `hal` acar sozu  gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next->next;
      return 0;       
    }
    if (!matchid(f, CLSBLK)){
      get_row_col_right(tll->next->next->next->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `}`  gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next->next->next->next;
      return 0;      
    }
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
 
  if ((matchid(a,SAY_TIP_1) || matchid(a,SAY_TIP_2) || matchid(a,SAY_TIP_3)) && \
	  (matchid(b,KODDATA)||matchid(b,BLOCK))){
       ptr->id = KODDATA;
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



/* convert NEQEDERKI1 SHERT ) into NEQEDERKI2  */
int cons_neqederki1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,NEQEDERKI1) && matchid(b,SHERT) && matchid(c,CLSBRK)){
       ptr->id = NEQEDERKI2;
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

int check_neqederki1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  get_row_col_right(tll, &row, &col, &pt);    

  if (matchid(a,NEQEDERKI1)){
    if (!matchid(b,SHERT)){
      printf("(%d:%d)sehv: `%s` -den sonra  shert gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,CLSBRK)){
      get_row_col_right(tll->next, &row, &col, &pt);   
      printf("(%d:%d)sehv: `%s` -den sonra `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next;
      return 0;
    }

  }

  return 0;
}

/* convert EGEREXP1 SHERT ) into EGER_exp2  */
int cons_eger_exp1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,EGER_EXP1) && matchid(b,SHERT) && matchid(c,CLSBRK)){
       ptr->id = EGER_EXP2;
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


int check_eger_exp1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  get_row_col_right(tll, &row, &col, &pt);    

  if (matchid(a,EGER_EXP1)){
    if (!matchid(b,SHERT)){
      printf("(%d:%d)sehv: `%s` -den sonra shert gozlenirdi.\n",row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,CLSBRK)){
      get_row_col_right(tll->next, &row, &col, &pt);    
      printf("(%d:%d)sehv: `%s` -den sonra `)` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next->next;
      return 0;      
    }
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



int check_davamet_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,DAVAMET)){
    get_row_col_right(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -den sonra `;` gozlenirdi.\n", row, col, pt);
	err_cnt++;
    *tll_ref = tll->next;
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


int check_dayan_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,DAYAN)){
    get_row_col_right(tll, &row, &col, &pt);    
    printf("(%d:%d)sehv: `%s` -den sonra `;` gozlenirdi.\n", row, col, pt);
	err_cnt++;
    *tll_ref = tll->next;
   }

  return 0;
}



int cons_standart_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,STANDART) && match_std_func(b) && matchid(c, NOQTEV)){
       ptr->id = STANDART_FUNK;
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



int cons_steke_qoy_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,STEKE_QOY) && match_inacf(b) && matchid(c, NOQTEV)){
       ptr->id = STEKE_QOY_STD;
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



int cons_stekden_gotur_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,STEKDEN_GOTUR) && match_ia(b) && matchid(c, NOQTEV)){
       ptr->id = STEKDEN_GOTUR_STD;
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


int cons_neqederki_exp_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,NEQEDERKI) && matchid(b,OPNBRK)){
       ptr->id = NEQEDERKI1;
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


int check_neqederki_exp_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

    if (matchid(a,NEQEDERKI)){
      get_row_col_right(tll, &row, &col, &pt);    
      printf("(%d:%d)sehv: `%s` -den sonra `(` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
   }

  return 0;
}



/* convert eger ( into EGER_exp1  */
int cons_eger_exp_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
    if (matchid(a,EGER) && matchid(b,OPNBRK)){
       ptr->id = EGER_EXP1;
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



int check_eger_exp_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

    if (matchid(a,EGER)){
      get_row_col_right(tll, &row, &col, &pt);    
      printf("(%d:%d)sehv: `%s` -den sonra `(` gozlenirdi.\n", row, col, pt);
	  err_cnt++;
      *tll_ref = tll->next;
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

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0; 

  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0; 

 
  if (matchid(a,SAY) && matchid(b, OPNBRK) &&(match_ina(c) || matchid(c, STRMEM) || matchid(c, EXPR) ) && matchid(d, CLSBRK)){
    ptr->id = SAY_TIP_1;
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


int cons_say_tip_2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e; 

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

/* very special case only for say_2 and say_3 operators
   as idt ia translated to ynt , say (idt idt ) will also be
   translated to say (ynt), so we have to check and extract indexes
   from ynt
*/

  /* firs check ynt case  */

  if (matchid(a,SAY) && matchid(b, OPNBRK) && matchid(c, YNT1) && matchid(d, CLSBRK)){
    ptr->id = SAY_TIP_2;
	ptr->childs[0] = tll;
	ptr->childs[1] = tll->next;
	ptr->childs[2] = tll->next->next->childs[0];
	ptr->childs[3] = tll->next->next->childs[1];
	ptr->childs[4] = tll->next->next->next;
	ptr->cld_cnt   = 5;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
	ptr->next = tll->next->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

	 return 1;
    }

  
  if (matchid(a,SAY) && matchid(b, OPNBRK) && (match_ia(c) || matchid(c, STRMEM))  && \
       (match_ina(d) || matchid(d, STRMEM) || matchid(d, EXPR) ) && matchid(e, CLSBRK)){
    ptr->id = SAY_TIP_2;
	ptr->childs[0] = tll;
	ptr->childs[1] = tll->next;
	ptr->childs[2] = tll->next->next;
	ptr->childs[3] = tll->next->next->next;
	ptr->childs[4] = tll->next->next->next->next;
	ptr->cld_cnt   = 5;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
	ptr->next = tll->next->next->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

	 return 1;
    }

  return 0;
}



int cons_say_tip_3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e,f; 

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
	  f = tll->next->next->next->next->next->id;
  else
    return 0; 

  /* very special case only for say_2 and say_3 operators
   as idt ia translated to ynt , say (idt idt idt) will also be
   translated to say (ynt idt ) or say (ary idt idt ) to say (ary ynt1), so we 
   have to check and extract indexes from ynt
*/

  /* firs check say (ynt idt) case  */

  if (matchid(a,SAY) && matchid(b, OPNBRK) && matchid(c, YNT1) && \
	  (match_ina(d) || matchid(d, STRMEM) || matchid(d, EXPR) ) && matchid(e, CLSBRK)){
    ptr->id = SAY_TIP_3;
	ptr->childs[0] = tll;
	ptr->childs[1] = tll->next;
	ptr->childs[2] = tll->next->next->childs[0];
	ptr->childs[3] = tll->next->next->childs[1];
	ptr->childs[4] = tll->next->next->next;
	ptr->childs[5] = tll->next->next->next->next;
	ptr->cld_cnt   = 6;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

	 return 1;
    }


    /* next check say(idt ynt) case  */

  if (matchid(a,SAY) && matchid(b, OPNBRK) && (match_ia(c) || matchid(c, STRMEM))  && \
	   matchid(d, YNT1) && matchid(e, CLSBRK)){
    ptr->id = SAY_TIP_3;
	ptr->childs[0] = tll;
	ptr->childs[1] = tll->next;
	ptr->childs[2] = tll->next->next;
	ptr->childs[3] = tll->next->next->next->childs[0];
	ptr->childs[4] = tll->next->next->next->childs[1];
	ptr->childs[5] = tll->next->next->next->next;
	ptr->cld_cnt   = 6;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next->next->next; 
    *tll_ref = ptr;
    (*pos)++;

	 return 1;
    }
 
  if (matchid(a,SAY) && matchid(b, OPNBRK) && (match_ia(c) || matchid(c, STRMEM))  && \
	   (match_ina(d) || matchid(d, STRMEM) || matchid(d, EXPR) ) && \
	   (match_ina(e) || matchid(e, STRMEM) || matchid(e, EXPR) ) && matchid(f, CLSBRK)){
    ptr->id = SAY_TIP_3;
	ptr->childs[0] = tll;
	ptr->childs[1] = tll->next;
	ptr->childs[2] = tll->next->next;
	ptr->childs[3] = tll->next->next->next;
	ptr->childs[4] = tll->next->next->next->next;
	ptr->childs[5] = tll->next->next->next->next->next;
	ptr->cld_cnt   = 6;
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next->next->next->next; 
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

  if ((match_ia(a)||matchid(a, STRMEM)) && matchid(b,ASGN) && (match_inacf(c)||matchid(c, EXPR) || matchid(c, FCALL1)|| matchid(c, SNMEM) || matchid(c, STRMEM)) && matchid(d,NOQTEV)){
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


int check_asgn_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(b,ASGN)){
    if (!(match_ia(a)||matchid(a, STRMEM))){
      print_before(tll->next, "deyishen");
      *tll_ref = tll->next->next;
      return 0;
    }
    if ((!match_inacf(c)||matchid(c, EXPR) || matchid(c, FCALL1)|| matchid(c, SNMEM) || matchid(c, STRMEM))){
      print_after(tll->next->next, "ifade");
      *tll_ref = tll->next->next->next;
      return 0;
    }
    if (!matchid(d,NOQTEV)){
      print_before(tll->next->next->next, "`;`");
      *tll_ref = tll->next->next->next->next;
      return 0;
    }
  }

  return 0;
}


int cons_capet3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,CAPET2)&&matchid(b,CLSBRK)&&matchid(c, NOQTEV)){
    ptr->id  = CAPET3;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->cld_cnt   = 3;

    
    ptr->lrb = 1; //branch

    tllp->next = ptr;
    ptr->next = tll->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


int check_capet3_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,CAPET2)){
    if (!matchid(b,CLSBRK)){
      print_before(tll, "`)`");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,NOQTEV)){
      print_before(tll->next, "`;`");
      *tll_ref = tll->next->next;
      return 0;
    }
  }

  return 0;
}


int cons_capet2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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
 
  if (matchid(a,CAPET1) && matchid(b,VDEC)){
    ptr->id  = CAPET2;
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



int cons_daxilet3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,DAXILET2)&&matchid(b,CLSBRK)&&matchid(c,NOQTEV)){
    ptr->id  = DAXILET3;
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


int check_daxilet3_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,DAXILET2)){
    if (!matchid(b,CLSBRK)){
      print_before(tll, "`)`");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,NOQTEV)){
      print_before(tll->next, "`;`");
      *tll_ref = tll->next->next;
      return 0;       
    }    
  }

  return 0;
}


int cons_daxilet2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if (matchid(a,DAXILET1)&&matchid(b,VDEC)){
    ptr->id  = DAXILET2;
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


int cons_capet1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,CAPET1)&& (match_inacsfE(b) || matchid(b, STRMEM) )){
    ptr->id  = CAPET2;
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

int cons_strmem_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; 

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

  /* d is used for testing, not consumed  */
  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;


  if ((match_ia(a) && matchid(b,DOT) && match_ia(c) && !(matchid(d, DOT) || matchid(d, ARYOPN)  )) || \
      (match_ia(a) && matchid(b,DOT) && matchid(c,STRMEM))){
       ptr->id = STRMEM;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch


       ptr->next =  tll->next->next->next; /* God save me from pointers   */
       *tll_ref = ptr;
       tllp->next = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int check_strmem_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  /* d is used for testing, not consumed  */
  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;

  if (matchid(b,DOT)){
    if (!match_ia(a)){
      print_before(tll, "deyishen");
      *tll_ref = tll->next;
      return 0;
    }
    if (!( match_ia(c)||matchid(c,STRMEM) || matchid(c, SNMEM) || matchid(c, FCALL1))){
      print_before(tll->next, "deyishen");
      *tll_ref = tll->next->next;
      return 0;      
    }
}

  return 0;
}


int cons_snmem_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; 

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

  /* d is used for testing, not consumed  */
  if (tll->next->next->next!=NULL)
    d = tll->next->next->next->id;
  else
    return 0;


  if (match_ia(a) && matchid(b,DOT) && (matchid(c, FCALL1)||matchid(c,SNMEM))){
       ptr->id = SNMEM;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->cld_cnt   = 3;
       ptr->lrb = 1; //branch


       ptr->next =  tll->next->next->next; /* God save me from pointers   */
       *tll_ref = ptr;
       tllp->next = ptr;
       (*pos)++;

      return 1;
   }
  return 0;
}


int cons_strukt2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,STRUKT1)&& matchid(b,YNT2) && matchid(c,CLSBLK) && matchid(d, NOQTEV))
   goto L4;

  if (matchid(a,STRUKT1)&& matchid(b, CLSBLK) && matchid(c,NOQTEV))
   goto L3;

  if (matchid(a, STRUKT2) && matchid(b, STRUKT2))
    goto L2;

  goto out;

  k = 0;
  ptr->next = NULL;

 L4:
    ptr->childs[3] = tll->next->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next->next; 

 L3:
    ptr->childs[2] = tll->next->next;
    k++;
    if (ptr->next == NULL)
        ptr->next = tll->next->next->next; 
 L2:

       ptr->id  = STRUKT2;
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


int cons_daxilet1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,DAXILET1)&& (match_ia(b) || matchid(b, STRMEM))){
    ptr->id  = DAXILET2;
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



int check_daxilet1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,DAXILET1)){
    if (!(match_ia(b) || matchid(b, STRMEM))){
      print_after(tll, "deyishen");
      *tll_ref = tll->next;
      return 0;
    }
  }

  return 0;
}


int cons_capet_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  struct tok_list_el *atll, *btll;
  int a,b;   

  if (tll!=NULL){
    a = tll->id;
	atll = tll;
  }
  else
    return 0;

  if (tll->next!=NULL){
    b = tll->next->id;
	btll = tll->next;
  }
  else
    return 0;

  if (matchid(a,CAPET)){

	  //insert pseudo tll using tok of previous tll
	  if (!matchid(b,OPNBRK)){
		insert_tll(atll, OPNBRK);
		print_after(tll, "`(`");
	  }

    ptr->id  = CAPET1;
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



int check_capet_error(struct tok_list_el **tll_ref){


  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,CAPET)){
    print_after(tll, "`(`");
    *tll_ref = tll->next;
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

  if (tll->next->next!=NULL)
    c = tll->next->next->id;
  else
    return 0;

  if (matchid(a,HAL)&& (matchid(b,NUMB) || matchid(b, CHR)) && matchid(c, COLON)){
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


int check_hal1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,HAL)){
    if (!(matchid(b,NUMB) || matchid(b, CHR))){
      print_after(tll, "EDED ve ya SIMVOL");
      *tll_ref = tll->next;
      return 0;
    }
    else
      if (!matchid(c, COLON)){
	print_after(tll->next, "`:`");
	*tll_ref = tll->next->next;
	return 0;
	}
  }

    /* if (matchid(a, HAL1)  && matchid(b, HAL1)  NOTE: did not checked! */

    if (matchid(a, SUSMA)){
      print_after(tll, "`:`");
      *tll_ref = tll->next;
      return 0;
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

  if (matchid(a,SUSMA)&& matchid(b, COLON)){
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


int check_sus1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SUSMA)){
     if (!matchid(b, COLON)){
	print_after(tll, "`:`");
	*tll_ref = tll->next;
	return 0;
	}
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
       matchid(b,KODDATA) && (matchid(c, CLSBLK) || matchid(c, HAL2))) || \
      (matchid(a, HAL2) && matchid(b, HAL2))){
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


int check_hal2_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,HAL1) || matchid(a, HAL1S) || matchid(a, SUS1))) { 
    if (!matchid(b, KODDATA)){
      print_after(tll, "emeliyyat ");
      *tll_ref = tll->next;
      return 0;
    }
  }

  return 0;
}

int cons_hal3_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


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

  if ((matchid(a, HAL2) && matchid(b, HAL2)) || \
      (matchid(a, HAL2) && matchid(b, HAL3)) || \
      (matchid(a, HAL3) && matchid(b, HAL3)) || \
      (matchid(a, HAL3) && matchid(b, HAL2))){
    ptr->id  = HAL3;
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


int cons_sec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,   int *pos){


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

  if (matchid(a,SEC)&&matchid(b,OPNBRK)){
    ptr->id  = SEC1;
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


int check_sec_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,SEC)){
    print_after(tll, "`(`");
    *tll_ref = tll->next;
 }

  return 0;
}

/* convert  FCALL IDT into FCALL VDEC  */
int cons_vdec1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a, FCALL) && (match_inacsfEF(b) || matchid(b, STRMEM))){
    ptr->id  = VDEC;
    ptr->childs[0] = tll->next;
    ptr->cld_cnt   = 1;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}

/* convert , idt into VDEC  */
int cons_vdec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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


  if (!matchid(a, IDT) &&
      ((match_inacsfE(b)||matchid(b,STRMEM)||matchid(b,VDEC))) &&\
        matchid(c, VERGUL) &&\
      ((match_inacsfE(d)||matchid(d,STRMEM) || matchid(d, VDEC)))){
    ptr->id  = VDEC;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->childs[2] = tll->next->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


/* conevrt funksiya idt ( into FDECL  */
int cons_fdecl_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,     int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if (matchid(a,FUNKSIYA) && matchid(b,IDT) && matchid(c, OPNBRK) ){
    ptr->id  = FDECL;
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


int check_fdecl_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,FUNKSIYA)){
    if (!matchid(b,IDT)){
      print_after(tll, "funksiyanin adi");
      *tll_ref = tll->next;
      return 0;	
    }
    else{
      if (!matchid(c, OPNBRK)){
      print_before(tll->next, "`(`");
      *tll_ref = tll->next->next;
      return 0;		
      }
    }
  }

  return 0;
}


/* conevrt idt ( into FCALL  */
int cons_fcall_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,     int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if (!matchid(a,FUNKSIYA) && matchid(b,IDT) && matchid(c, OPNBRK)){
    ptr->id  = FCALL;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}




int cons_strukt1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if (matchid(a,STRUKT)&&matchid(b,IDT)&&matchid(c,OPNBLK)){
    ptr->id  = STRUKT1;
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


/* convert SINIF IDT to SINIF_HEAD_START  */
int cons_SHS_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,SINIF)&&matchid(b,IDT)){
    ptr->id  = SINIF_HEAD_START;
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

int check_SHS_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a,SINIF)){
    if (!matchid(b,IDT)){
      print_after(tll, "sinfin adi");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(c,OPNBLK)){
      print_after(tll->next, "`{`");
      *tll_ref = tll->next->next;
      return 0;
    }
  }

  return 0;
}


/* convert SINIF_head_START OPNBLK to SINIF_HEAD  */
int cons_SH_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,SINIF_HEAD_START)&&matchid(b,OPNBLK)){
    ptr->id  = SINIF_HEAD;
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




/* convert  SINIF_HEAD YNT || FUNK  into SINIF_HEAD SINIF_BODY 
   !!! not consuming SINIF_HEAD  */
int cons_SB_1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if ((matchid(a,SINIF_HEAD)||matchid(a,COLON))&& (matchid(b,FUNK)||matchid(b,KODDATA)||matchid(b,KOD_FUNK))){
		  /* ikinci matchdaki KODDAATA eslinde VAR olmalidi , sadece biz koddata -ni consume eliyende
		     VAR -i KODDATA -ya cevirmishdik. Amma sinfin yoxlanmasi zamani koddatani VAR kimi 
			 yoxlayacayiq, sinif daxilinde emeliyyat elan etmek olmaz. Cox pis haldir, heleki, 
			 bir normal hell tapilmalidir*/

       ptr->id  = SINIF_BODY;
       ptr->childs[0] = tll->next;
       ptr->cld_cnt   = 1;
       ptr->lrb = 1; //branch

       ptr->next = tll->next->next; 
       tllp->next->next = ptr;
       *tll_ref = ptr;
    (*pos)++;
    return 1;
}
 
  return 0;
}


int check_SB_1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if ((matchid(a,SINIF_HEAD)||matchid(a,COLON))){
    if(!(matchid(b,FUNK)||matchid(b,KODDATA)||matchid(b,KOD_FUNK))){
      print_after(tll, "elan ve ya funksiya");
      *tll_ref = tll->next;
      return 0;
    }
  }    

  return 0;
}


/* convert SINIF_BODY YNT2 || FUNK to SINIF_BODY  */
int cons_SBS_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,SINIF_BODY)&&(matchid(b,SINIF_BODY)||matchid(b,KODDATA)||matchid(b, FUNK)||matchid(b, KOD_FUNK))){
		  /* matchdaki KODDAATA eslinde VAR olmalidi , sadece biz koddata -ni consume eliyende
		     VAR -i KODDATA -ya cevirmishdik. Amma sinfin yoxlanmasi zamani koddatani VAR kimi 
			 yoxlayacayiq, sinif daxilinde emeliyyat elan etmek olmaz. Cox pis haldir, heleki, 
			 bir normal hell tapilmalidir*/

       ptr->id  = SINIF_BODY;
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




/* convert  SINIF_HEAD SINIF_BODY into SINIF_HEAD SINIF_DECL
   inserting ficticious MEXSUSI tll as fist child SPECIAL CASE
   in order to make all members private by default. 
   !!! not consuming SINIF_HEAD  */
int cons_SHD_1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

  struct tok_list_el *fictiv_gizli;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;

  if (matchid(a,SINIF_HEAD) && matchid(b,SINIF_BODY)){
    ptr->id  = SINIF_DECL;
    fictiv_gizli = create_tok_list_el(tll->tok); /*that may confuse parser, possible BUG*/ ;
    fictiv_gizli->id  = MEXSUSI;
    ptr->childs[0] = fictiv_gizli;
    ptr->childs[1] = tll->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch

    ptr->next = tll->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}


/* convert QORUNAN || MEXSUSI || UMUMI  COLON SINIF_BODY  to SINIF_DECL  */
int cons_SD_1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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

  if ((matchid(a,QORUNAN)||matchid(a,MEXSUSI)||matchid(a,UMUMI))&&\
      matchid(b, COLON) && matchid(c,SINIF_BODY)){
    ptr->id  = SINIF_DECL;
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



int check_SD_1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if ((matchid(a,QORUNAN)||matchid(a,MEXSUSI)||matchid(a,UMUMI))){
    if (!(matchid(b, COLON) || matchid(b, SINIF_BODY))){
      print_after(tll, "`:`");
      *tll_ref = tll->next;
      return 0;
    } 
    if (matchid(b, COLON)){
      if (!(matchid(c, SINIF_BODY)||matchid(c, FUNK)||matchid(c, YNT2))){
	print_after(tll->next, "elan ve ya kod");
	*tll_ref = tll->next->next;
	return 0;	
      }
    }
  }

  return 0;
}


/* convert SINIF_DECL  SINIF_DECL  to SINIF_DECL  */
int cons_SDS_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,SINIF_DECL)&&matchid(b,SINIF_DECL)){
    ptr->id  = SINIF_DECL;
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



/* convert SINIF_HEAD SINIF_DECL CLSBLK NOQTEV to SINIF_TYPE  */
int cons_ST_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref, int *pos){

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

  if (matchid(a,SINIF_HEAD) && matchid(b,SINIF_DECL) && matchid(c, CLSBLK) && matchid(d,NOQTEV))
   goto L4;

  if (matchid(a,SINIF_HEAD) && matchid(b, CLSBLK) && matchid(c,NOQTEV))
   goto L3;

 goto out;

  k = 0;
  ptr->next = NULL;

 L4:
  ptr->childs[3] = tll->next->next->next;
  k++;
   if (ptr->next == NULL)
	   ptr->next = tll->next->next->next->next; 

 L3:
    ptr->id  = SINIF_TYPE;
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
	ptr->childs[2] = tll->next->next;

    k+= 3;
    ptr->cld_cnt   = k;
	ptr->lrb = 1; //branch

	tllp->next = ptr;
	if (ptr->next == NULL)
		ptr->next = tll->next->next->next; 

    *tll_ref = ptr;
    (*pos)++;

    return 1;
 
 out:

  return 0;
}


 
 
int check_ST_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

   if (matchid(a,SINIF_HEAD) && matchid(b,SINIF_DECL)){
     if(!matchid(c, CLSBLK)){
       print_after(tll, "`}`");
       *tll_ref = tll->next;
       return 0;
     }

    if(!matchid(d, NOQTEV)){
      print_after(tll->next, "`;`");
       *tll_ref = tll->next->next;
       return 0;
     }
   }

  return 0;
}



int cons_daxilet_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a,DAXIL)&&matchid(b,OPNBRK)){
    ptr->id  = DAXILET1;
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



int check_daxilet_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,DAXIL)){
    print_after(tll, "`(`");
    *tll_ref = tll->next;
 }

  return 0;
}




/* consume   ynt2 to var   */
int cons_var_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
 
  int a;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (matchid(a,YNT2)){
    ptr->id  = VAR; /* always copy leftmost elements id on the list  */
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


int cons_ynt2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a, YNT1) &&  matchid(b, YNTS) && matchid(c, NOQTEV))
   goto L3;

  if ((matchid(a, YNT1) && matchid(b, NOQTEV)) || \
      (matchid(a, YNT2) && matchid(b, YNT2)))
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

       ptr->id  = YNT2;
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


/* YNT1 VERGUL YNT1 -> YNT1_FUNC_ARGS  
   YNT1_FUNC_ARGS VERGUL YNT1 -> YNT1_FUNC_ARGS
   YNT1_FUNC_ARGS VERGUL YNT1_FUNC_ARGS -> YNT1_FUNC_ARGS
*/ 
int cons_ynt1_func_args_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 
  if ((matchid(a, YNT1) && matchid(b,VERGUL) && matchid(c, YNT1))		 ||\
	  (matchid(a, YNT1_FUNC_ARGS) && matchid(b,VERGUL) && matchid(c, YNT1))  ||\
	  (matchid(a, YNT1_FUNC_ARGS) && matchid(b,VERGUL) && matchid(c, YNT1_FUNC_ARGS))){
       ptr->id  = YNT1_FUNC_ARGS;
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
 

/* IDT IA -> YNT1  */
int cons_ynt1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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


  if (matchid(a,IDT) && match_ia(b)){
    ptr->id  = YNT1;
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




/* YNT1 VERGUL IA -> YNT1 YNTS  */
int cons_ynts_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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


  /* a test ucun istifade olunur, parse olunmur  */
  if (matchid(a,YNT1) && matchid(b, VERGUL) && match_ia(c)){
    ptr->id  = YNTS;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->cld_cnt   = 2;
    ptr->lrb = 1; //branch


    ptr->next = tll->next->next->next; 
    tllp->next->next = ptr;
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



/* YNTS VERGUL IA -> YNTS  */
int cons_ynts1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */
  /*
  if ((cnt - *pos)<2)
  return 0; */  /* at least three elements nedeed to parse   */

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


  /* a test ucun istifade olunur, parse olunmur  */
  if (matchid(a,YNTS) && matchid(b, VERGUL) && match_ia(c)){
    ptr->id  = YNTS;
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





/* consume  (TEXT | SINIF_TYPE | STRUKT2 | KOD_FUNK | FUNK | KODDATA) &&
            (TEXT | SINIF_TYPE | STRUKT2 | KOD_FUNK | FUNK | KODDATA)
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

 if ((matchid(a,TEXT) || matchid(a,SINIF_TYPE) || matchid(a,STRUKT2) || matchid(a,KOD_FUNK) || matchid(a,FUNK) || matchid(a,KODDATA)) && \
	 (matchid(b,TEXT) || matchid(b,SINIF_TYPE) || matchid(b,STRUKT2) || matchid(b,KOD_FUNK) || matchid(b,FUNK) || matchid(b,KODDATA)))
     goto L2;

if (matchid(a,SINIF_TYPE) || matchid(a,STRUKT2) || matchid(a,KOD_FUNK) || matchid(a,FUNK) || matchid(a,KODDATA))
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


/* consume (FCALL1 || SNMEM)  NOQTEV to KODDATA   */
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


  if ((matchid(a,FCALL1)||matchid(a, SNMEM)) && matchid(b,NOQTEV)){
    ptr->id  = KODDATA;
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

int check_fcall1_op_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b;   

  if (tll!=NULL)
    a = tll->id;
  else
    return 0;

  if (tll->next!=NULL)
    b = tll->next->id;
  else
    return 0;


  if ((matchid(a,FCALL1)||matchid(a, SNMEM))){
    if (!matchid(b,NOQTEV)){
      print_after(tll, "`;`");
      *tll_ref = tll->next;
      return 0;
    }
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

 if (matchid(a,SHERT)&&(matchid(b,VE)||matchid(b,VEYA))&&matchid(c,SHERT)){
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


int check_vveya_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if ((matchid(b,VE)||matchid(b,VEYA))){
    if (!matchid(a,SHERT)){
      print_before(tll->next, "shert");
      *tll_ref = tll->next->next;
      return 0;
    }
    if (!matchid(c,SHERT)){
      print_after(tll->next, "shert");
      *tll_ref = tll->next->next->next;
      return 0;
    }
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

  if ((match_inacf(a)|| matchid(a, EXPR) || matchid(a, FCALL1) || matchid(a, STRMEM)) && match_cond(b) && (match_inacf(c)|| matchid(c, EXPR)  || matchid(c, FCALL1) || matchid(c, STRMEM) )){
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


int check_cond_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (match_cond(b)){
    if (!(match_inacf(a)|| matchid(a, EXPR) || matchid(a, FCALL1) || matchid(a, STRMEM))){
	print_before(tll->next, "ifade");
	*tll_ref = tll->next->next;
	return 0;
      }
      if (!(match_inacf(c)|| matchid(c, EXPR)  || matchid(c, FCALL1) || matchid(c, STRMEM))){
	print_after(tll->next, "ifade");
	*tll_ref = tll->next->next->next;
	return 0;
      }
  }            

  return 0;
}

/* consume (x) into x, i.e. get rid of brackets when there is only one 
   element inside   */
int cons_brk_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  /* SPECIAL CASE !!!!!!!!!!!!!!!!

     ONLY here we do not consume sequence into tree but simply remove
     brackets from sequence like (x) -> x .

	 NOTE! a is not consumed only for check
   */
#if 0
  if (matchid(a,OPNBRK)&&(match_ina(b)|| matchid(b, EXPR) || matchid(b, FCALL1) || matchid(b, SNMEM) || matchid(b, STRMEM) ||  matchid(b,SHERT))&&matchid(c,CLSBRK)){
    /* normally we have to delete a and c from memory as they are no more needed  */

       tllp->next = tll->next;
       *tll_ref = tll->next;
       tll->next->next = tll->next->next->next;
       (*pos)++;
    return 1;
 }

#endif

  /* why FLOAT is not in list  */
if ( !matchid(a,SAY) && matchid(b,OPNBRK)&&(match_inacf(c)|| matchid(c, EXPR) || matchid(c, FCALL1) || matchid(c, SNMEM) || matchid(c, STRMEM) ||  matchid(c,SHERT))&&matchid(d,CLSBRK)){
    /* normally we have to delete a and c from memory as they are no more needed  */

       *tll_ref = tll->next->next;
	   tll->next->next->next = tll->next->next->next->next; //remove CLSBRK
	   tllp->next->next = tll->next->next;
	   //tll->next = tll->next->next;
       (*pos)++;
    return 1;
 }




  return 0;
}

/* consume on declaration   */
int cons_crg2_dec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e,f,g;   
  int d1, d2;

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,IDT) && \
      matchid(b,ARYOPN) && matchid(c,NUMB) && matchid(d,ARYCLS) &&	\
      matchid(e,ARYOPN) && matchid(f,NUMB) && matchid(g,ARYCLS) ){

    ptr->id  = CRG2; /* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->childs[3] = tll->next->next->next;
    ptr->childs[4] = tll->next->next->next->next;
    ptr->childs[5] = tll->next->next->next->next->next;
    ptr->childs[6] = tll->next->next->next->next->next->next;
    ptr->cld_cnt   = 7;

    ptr->lrb = 1; //branch

    d1 = atoi(tll->next->next->tok->tok);
    d2 = atoi(tll->next->next->next->next->next->tok->tok);

    tll->tok->d1 = d1;
    tll->tok->d2 = d2;

    /* report to vartab  */
    /*add_dec_table(&dec_tab, tll->tok, CRG2);*/

    tllp->next = ptr;
    ptr->next =  tll->next->next->next->next->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



/* consume on declaration  */
int cons_crg1_dec_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref, int *pos){

  struct tok_list_el *tllp = *tll_ref;
  int d1;

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
 

 if (tll->next->next->next->next!=NULL)
   if (tll->next->next->next->next->id == ARYOPN)
     return 0; /* this is or CRG2 or ERROR  */

  if (matchid(a,IDT) && matchid(b,ARYOPN) && matchid(c, NUMB) && matchid(d,ARYCLS)){
    ptr->id  = CRG1;
       ptr->childs[0] = tll;
       ptr->childs[1] = tll->next;
       ptr->childs[2] = tll->next->next;
       ptr->childs[3] = tll->next->next->next;
       ptr->cld_cnt   = 4;
       ptr->lrb = 1; //branch

       d1 = atoi(tll->next->next->tok->tok);
       tll->tok->d1 = d1;

       /*add_dec_table(&dec_tab, tll, CRG1);*/

       tllp->next = ptr;
       ptr->next = tll->next->next->next->next; 
       *tll_ref = ptr;
       (*pos)++;

    return 1;
 }

  return 0;
}



/* consume on kod  */
int cons_crg2_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e,f,g;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

  if (matchid(a,IDT) && \
      matchid(b,ARYOPN) && (match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM)) && matchid(d,ARYCLS) &&	\
      matchid(e,ARYOPN) && (match_ina(f)|| matchid(f, EXPR)  || matchid(f, STRMEM)) && matchid(g,ARYCLS) ){ 

    ptr->id  = CRG2; /* always copy leftmost elements id on the list  */
    ptr->childs[0] = tll;
    ptr->childs[1] = tll->next;
    ptr->childs[2] = tll->next->next;
    ptr->childs[3] = tll->next->next->next;
    ptr->childs[4] = tll->next->next->next->next;
    ptr->childs[5] = tll->next->next->next->next->next;
    ptr->childs[6] = tll->next->next->next->next->next->next;
    ptr->cld_cnt   = 7;

    ptr->lrb = 1; //branch

    /* report to vartab  */
    /*add_usg_table(&usg_tab, tll->tok, CRG2);*/

    tllp->next = ptr;
    ptr->next =  tll->next->next->next->next->next->next->next; /* God save me from pointers   */
    *tll_ref = ptr;
    (*pos)++;

    return 1;
 }

  return 0;
}



int check_crg2_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

  int a,b,c,d,e,f,g;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next!=NULL)
    f = tll->next->next->next->next->next->id;
  else
    return 0; 

  if (tll->next->next->next->next->next->next!=NULL)
    g = tll->next->next->next->next->next->next->id;
  else
    return 0;

      if (matchid(b,ARYOPN)){
	if (!matchid(a,IDT)){
	  print_before(tll->next, "deyishen");
	  *tll_ref = tll->next->next;
	  return 0;
	}
	if (!(match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM))){
	  print_after(tll->next, "ifade");
	  *tll_ref = tll->next->next->next;
	  return 0;
	}
	if (!matchid(d,ARYCLS)){
	  print_after(tll->next->next, "]");
	  *tll_ref = tll->next->next->next->next;
	  return 0;
	}
	if (matchid(e,ARYOPN)){
	  if (!(match_ina(f)|| matchid(f, EXPR)|| matchid(f, STRMEM))){
	  print_after(tll->next->next->next->next, "ifade");
	  *tll_ref = tll->next->next->next->next->next;
	  return 0;
	  }
	  if (!matchid(g,ARYCLS)){
	  print_after(tll->next->next->next->next->next, "`]`");
	  *tll_ref = tll->next->next->next->next->next->next;
	  return 0;
	  }
	}
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

  if (((match_ia(a) || matchid(a, STRMEM)) && matchid(b, DECREMENT) && matchid(c, NOQTEV) ) ||\
	  (matchid(a, DECREMENT) && (match_ia(b) || matchid(b, STRMEM)) && matchid(c, NOQTEV) )){
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

  if (((match_ia(a) || matchid(a, STRMEM)) && matchid(b, INCREMENT) && matchid(c, NOQTEV) ) ||\
	  (matchid(a, INCREMENT) && (match_ia(b) || matchid(b, STRMEM)) && matchid(c, NOQTEV) )){
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



int cons_crg1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 if (tll->next->next->next->next!=NULL)
   if (tll->next->next->next->next->id == ARYOPN)
     return 0; /* this is or CRG2 or ERROR  */

 if (matchid(a,IDT) && matchid(b,ARYOPN) && (match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM)) && matchid(d,ARYCLS)){
    ptr->id  = CRG1;
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



int check_crg1_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;
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

 if (tll->next->next->next->next!=NULL)
   if (tll->next->next->next->next->id == ARYOPN)
     return 0; /* this is or CRG2 or ERROR  */

 if (matchid(a,IDT) && matchid(b,ARYOPN)){
   if (!(match_ina(c)|| matchid(c, EXPR)  || matchid(c, STRMEM))){
     print_after(tll->next, "ifade");
     *tll_ref=tll->next->next;
     return 0;
   }
   else{
     if (!matchid(d,ARYCLS)){
       print_after(tll->next->next, "ifade");
       *tll_ref=tll->next->next->next;
       return 0;       
     }
   }
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

  if (matchid(a, QAYTAR) &&  (match_inacsfE(b) || matchid(b, STRMEM)) && matchid(c, NOQTEV))
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



int check_qaytar_error(struct tok_list_el **tll_ref){

  struct tok_list_el *tll = *tll_ref;

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

  if (matchid(a, QAYTAR)){
    if (!(match_inacsfE(b) || matchid(b, STRMEM) || matchid(b, NOQTEV))){
      print_after(tll, "deyishen ve ya `;`");
      *tll_ref = tll->next;
      return 0;
    }
    if (!matchid(b, NOQTEV)){
      print_after(tll, "`;`");
      *tll_ref = tll->next->next;
      return 0;
    }
  }

  return 0;
}




int cons_fcall1_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

 if (matchid(a, FCALL) &&  matchid(b, VDEC) && matchid(c, CLSBRK))
   goto L3;

 if (matchid(a, FCALL) && matchid(b, CLSBRK))
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

       ptr->id  = FCALL1;
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



int cons_plus_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

  struct tok_list_el *tllp = *tll_ref;

  struct tok_list_el *tll = tllp->next; /* because it is refeence */

  int a,b,c,d,e;   

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

  if (tll->next->next->next->next!=NULL)
    e = tll->next->next->next->next->id;
  else
    return 0;

  if (!match_hp_opr(a) && ((match_inaf(b)  || matchid(b, EXPR) || matchid(b, FCALL1) || matchid(b, STRMEM)) && match_lp_opr(c) && (match_inaf(d)||  matchid(d, EXPR) || matchid(d, FCALL1) || matchid(d, STRMEM))) && !match_hp_opr(e) ){
    ptr->id  = EXPR;
    ptr->childs[0] = tll->next;
    ptr->childs[1] = tll->next->next;
    ptr->childs[2] = tll->next->next->next;
    ptr->cld_cnt   = 3;
    ptr->lrb = 1; //branch
    
    /* WARNING!!!!!!
       Only here we copy tok of first child to parent, in order to
       be able to get tok on interp.c/get_head_tok().
       Otherwise we NEVER copy anything from child to parent,
       as this may break parsing logic.
    */
    ptr->tok = ptr->childs[0]->tok;
    
    /* special case: we don't consume all nodes as childs but border
       nodes checked for hp oprs, hope that works   */
    
    ptr->next = tll->next->next->next->next; 
    tllp->next->next = ptr;
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


  if ((match_inaf(a)||  matchid(a, FCALL1) || matchid(a, EXPR) || matchid(a, STRMEM)   ) && match_hp_opr(b) && (match_inaf(c)||  matchid(c, FCALL1) || matchid(c, EXPR)  || matchid(c, STRMEM)  )){
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
	  matchid(a, VAR)			||  matchid(a, STANDART_FUNK)		||	\
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



 


int build_cur_layer_funk_elan (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int    pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_funk_elan_tok(ptr, &tll,   &pos))
      continue; 

    tll = tll->next;
   }
 
  return pos;
}


int check_funk_elan_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_funk_elan_error(&tll);

    tll = tll->next;
   }
 
  return 1;
}






int build_cur_layer_funk (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_funk_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}


int build_cur_layer_kod_funk (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_kod_funk_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }
 
  return pos;
}



int build_cur_layer_sinif (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

       if (cons_SHS_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_SH_tok(ptr, &tll,   &pos))
      continue; 
    
   if (cons_SB_1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_SBS_tok(ptr, &tll,   &pos))
    continue; 

   
   tll = tll->next;
   }
 
  return pos;
}



int check_sinif_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_SHS_error(&tll);
    check_SB_1_error(&tll);
    check_SD_1_error(&tll);
    check_ST_error(&tll);

   tll = tll->next;
   }
 
  return 1;
}

int build_cur_layer_sinif_dec (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

 
      if (cons_SHD_1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_SD_1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_SDS_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_ST_tok(ptr, &tll,   &pos))
     continue; 
   
   tll = tll->next;
   }
 
  return pos;
}




int build_cur_layer_koddata (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_say_123_tok(ptr, &tll,   &pos))
      continue; 	

    if (cons_eger_yoxsa_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_eger_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_neqederki_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_hal2_tok(ptr, &tll,   &pos))
      continue; 

   //   if (cons_hal3_tok(ptr, &tll,   &pos))
   //   continue; 

  if (cons_secim_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_koddata_tok(ptr, &tll,   &pos))
      continue;

	if (cons_block_tok(ptr, &tll,   &pos))
      continue;

  tll = tll->next;

   }

  return pos;
}


int check_koddata_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_eger_yoxsa_error(&tll);
    check_neqederki_error(&tll);
    check_hal2_error(&tll);
    check_secim_error(&tll);

  tll = tll->next;

   }

  return 1;
}


int build_cur_layer_dax1_cap1 (struct tok_list *tree){

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

    if (cons_daxilet1_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_daxilet2_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_capet1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_capet2_tok(ptr, &tll,   &pos))
      continue;

   if (cons_hal1s_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
   }

  return pos;
}



int check_dax1_cap1_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_daxilet1_error(&tll);

    /*
    if (cons_daxilet1_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_daxilet2_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_capet1_tok(ptr, &tll,   &pos))
     continue; 

   if (cons_capet2_tok(ptr, &tll,   &pos))
      continue; 
    */

   tll = tll->next;
   }

  return 1;
}


int build_cur_layer_dax_cap (struct tok_list *tree){

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


    if (cons_daxilet_tok(ptr, &tll, &pos))
      continue; 


     if (cons_capet_tok(ptr, &tll,  &pos))
      continue; 


    /* egeri de burda eliyiriy  */
     if (cons_eger_exp_tok(ptr, &tll,  &pos))
      continue;

     if (cons_neqederki_exp_tok(ptr, &tll,  &pos))
      continue;

    if (cons_hal1_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_sus1_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_sec_tok(ptr, &tll,  &pos))
      continue;  

    if (cons_davamet_tok(ptr, &tll, &pos))
      continue; 

    if (cons_dayan_tok(ptr, &tll,  &pos))
      continue; 

	if (cons_standart_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_steke_qoy_tok(ptr, &tll,  &pos))
      continue; 

	if (cons_stekden_gotur_tok(ptr, &tll,  &pos))
      continue; 

    if (cons_fdecl_tok(ptr, &tll,  &pos))
      continue;

     if (cons_fcall_tok(ptr, &tll,  &pos))
      continue;

   tll = tll->next;

  }

  return pos;
}

void check_dax_cap_errors(struct tok_list *tree){


  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

     check_daxilet_error(&tll);
//     check_capet_error(&tll);
     check_eger_exp_error(&tll);
     check_neqederki_exp_error(&tll);
     check_hal1_error(&tll);
     check_sus1_error(&tll);
     check_sec_error(&tll);
     check_davamet_error(&tll);
     check_dayan_error(&tll);
     //     check_emptyop_tok(&tll); NOT implemented
     check_fdecl_error(&tll);
     /*

     if (cons_fcall_tok(ptr, &tll,  &pos))
     */

   tll = tll->next;
 }
   
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

    if (cons_daxilet3_tok(ptr, &tll,   &pos))
      continue; 

    if (cons_capet3_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_asgn_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_decrement_tok(ptr, &tll,   &pos)) 
	   continue;

  if (cons_increment_tok(ptr, &tll,   &pos)) 
	   continue;

  if (cons_say_tip_1_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_say_tip_2_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_say_tip_3_tok(ptr, &tll,   &pos))
      continue; 

  /* shertin consumunda sintaksis telebi olan moterezelerde consume\
     olundugundan eger 2 fazada cons olunur daxil ve capet operatorlari kimi  */
 if (cons_eger_exp1_tok(ptr, &tll,   &pos))
      continue; 

 if (cons_neqederki1_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_fcall1_op_tok(ptr, &tll,   &pos))
      continue; 

  if (cons_qaytar_tok(ptr, &tll,   &pos))
      continue; 


   tll = tll->next;
  }

  return pos;
}



int check_simple_oprs_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  while (tll!=NULL){

    check_daxilet3_error(&tll);
    check_capet3_error(&tll);
    check_asgn_error(&tll);
    check_eger_exp1_error(&tll);
    check_neqederki1_error(&tll);
    check_fcall1_op_error(&tll);
    check_qaytar_error(&tll);

    tll = tll->next;
  }

  return 1;
}



int build_cur_layer_crg_dec (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
  pos = 0;      /* how many elements scanned so far  */

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_crg2_dec_tok(ptr, &tll,  &pos))
      continue; 
     if (cons_crg1_dec_tok(ptr, &tll,  &pos))
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


int build_cur_layer_ynt1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); 

   if (cons_ynt1_tok(ptr, &tll,   &pos))
      continue; 

   if (cons_ynt1_func_args_tok(ptr, &tll,   &pos))
      continue; 
  

   tll = tll->next;

  }

  return pos;
}




int build_cur_layer_ynts (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); 

   if (cons_ynts_tok(ptr, &tll,   &pos))
      continue; 
   if (cons_ynts1_tok(ptr, &tll,   &pos))
      continue; 
  if (cons_ynt2_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;

  }

  return pos;
}


/* actually here we just convert ynt2 to VAR  */
int build_cur_layer_var (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      

  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); 

   if (cons_var_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;

  }

  return pos;
}


int build_cur_layer_vdec1 (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_vdec1_tok(ptr, &tll,   &pos))
      continue;


   tll = tll->next;
  }

  return pos;
}


int build_cur_layer_vdec (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

    if (cons_vdec_tok(ptr, &tll,   &pos))
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
      continue; /* match found, cons_emel updated everything 
                   and copied new element to upper layer  */
   if (cons_crg2_tok(ptr, &tll,   &pos))
      continue;
   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_strmem_tok(ptr, &tll,   &pos))
      continue;

   if (cons_mult_tok(ptr, &tll,   &pos))
      continue;

   tll = tll->next;
  }

  return pos;
}



int build_cur_layer_snmem (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */

  
  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */

   if (cons_snmem_tok(ptr, &tll,   &pos))
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



int check_cnds_errors (struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;
 
  
  while (tll!=NULL){

    check_cond_error(&tll);
    check_vveya_error(&tll);

    /*
   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_cond_tok(ptr, &tll,   &pos))
      continue;
   if (cons_vveya_tok(ptr, &tll,   &pos))
      continue;
    */

   tll = tll->next;
  }

  return 1;
}


 

void bottom_up_parse_snmem(struct tok_list *tree){
 
    while(build_cur_layer_snmem(tree));
}


void bottom_up_parse_funk_elan(struct tok_list *tree){

 
    while(build_cur_layer_funk_elan(tree));
}




void bottom_up_parse_text(struct tok_list *tree){

 
    while(build_cur_layer_text(tree));
}


 


void bottom_up_parse_kod_funk(struct tok_list *tree){


    while(build_cur_layer_funk(tree));
	while(build_cur_layer_kod_funk(tree));
}

 



void bottom_up_parse_kod_or_var(struct tok_list *tree){

 
    while(build_cur_layer_koddata(tree));
    
}

void bottom_up_parse_dax1_cap1(struct tok_list *tree){
 
    while(build_cur_layer_dax1_cap1(tree));    
}

void bottom_up_parse_dax_cap(struct tok_list *tree){

	while(build_cur_layer_dax_cap(tree));    
}


/* array declaration 1 and 2 size  */
void bottom_up_parse_crg_dec(struct tok_list *tree){

      while(build_cur_layer_crg_dec(tree));    
}

void bottom_up_parse_var(struct tok_list *tree){
 
    while(build_cur_layer_ynt1(tree));    
    while(build_cur_layer_ynts(tree));    
    while(build_cur_layer_var(tree));    

    
}


void bottom_up_parse_exprs(struct tok_list *tree){
 

    int k;

 loop:

    k = build_cur_layer_hp_oprs(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_lp_oprs(tree);
    if (k>0)
      goto loop;

    k = build_cur_layer_fcall1(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec1(tree);
    if (k>0) 
      goto loop;



}


void check_exprs_erros(struct tok_list *tree){

  struct tok_list_el *tll;

  tll = tree->first;

  while (tll!=NULL){
 
    //  check_crg1_error(&tll);
    check_crg2_error(&tll);
    check_strmem_error(&tll);
    /*

   if (cons_brk_tok(ptr, &tll,   &pos))
      continue;
   if (cons_strmem_tok(ptr, &tll,   &pos))
      continue;
   if (cons_mult_tok(ptr, &tll,   &pos))
      continue;
-----
    if (cons_plus_tok(ptr, &tll,   &pos))
-----
    if (cons_fcall1_tok(ptr, &tll,   &pos))
-----
    if (cons_vdec_tok(ptr, &tll,   &pos))
-----
    if (cons_vdec_tok(ptr, &tll,   &pos))

    */

    /*
    k = build_cur_layer_hp_oprs(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_lp_oprs(tree);
    if (k>0)
      goto loop;

    k = build_cur_layer_fcall1(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec(tree);
    if (k>0) 
      goto loop;

    k = build_cur_layer_vdec1(tree);
    if (k>0) 
      goto loop;


    */
    tll = tll->next;
  }
}


void bottom_up_parse_cnds(struct tok_list *tree){
 
    while(build_cur_layer_cnds(tree));
    
}


void bottom_up_parse_sinif(struct tok_list *tree){
 
    while(build_cur_layer_sinif(tree));
    while(build_cur_layer_sinif_dec(tree));
    
}


void bottom_up_parse_simple_oprs(struct tok_list *tree){
 
    while(build_cur_layer_simple_oprs(tree));
    
}


/* checking code comes here  */

void add_sec_hal(struct sec *sec, struct tok_list_el *tll){

  sec->hals[sec->size] = tll;
  sec->size++;

}

void build_secim(struct tok_list_el *tll, struct sec *sec){

  int i;

  if (tll->id == HAL1){
    if (tll->childs[0]->id == HAL){
      add_sec_hal(sec, tll->childs[1]);
      return;
    }
    if (tll->childs[0]->id == SUSMA){
      add_sec_hal(sec, tll->childs[0]);
    return ;
    }
}

  for (i=0; i<tll->cld_cnt; ++i)
    build_secim(tll->childs[i], sec);

  return;
}

void check_secim(struct sec *sec){

  struct tok_list_el *tll;
  int i, j,  sus, hits, cnt = sec->size;

  unsigned long hals[MAX_HALS];

  sus = -1;
  hits = 0;

  for (i=0; i<cnt; ++i){
    tll = sec->hals[i];
    if (tll->id == SUSMA){
      if (sus > 0)
	printf("Sehv: setir %d: sec operatoru susma hali evvel setir %d -de elan olunmushdur.\n", tll->tok->row, sec->hals[sus]->tok->row);
	  err_cnt++;
      sus = i;
      continue;
    }

    if (tll->id != NUMB && tll->id != CHR){
      printf("Sehv: setir %d: sec operatoru hal parametri olaraq yalniz tam tipli eded ve ya simvol verile biler.\n ", tll->tok->row);
	  err_cnt++;
      continue;
    }

    hals[hits] = tll->tok->hid;

    for (j = 0; j<hits; ++j)
      if (hals[j] == hals[hits]){
	printf("Sehv: setir %d: sec operatoru hal parametrinin \'%s\' qiymeti artiq daxil olunub (setir %d).\n", sec->hals[hits]->tok->row, sec->hals[hits]->tok->tok, sec->hals[j]->tok->row);
	err_cnt++;
	break;
      }
    
    hits++;
    
  }
}




void print_by_kwd();
void init_context_left();
void print_by_context();
void print_only_sinif_and_funcs();
void print_ast_list(struct AST_list *ast_list);
/* we accept text if it is parsed i.e. TEXT is on top, or
   it is empty i.e.  user typed no kod */
int check_parse_tree(struct tok_list *tree){

  struct tok_list_el *tll = tree->first;

  if (tree->first->id == FILEBEG    && \
      (tree->first->next->id == TEXT || tree->first->next->id == FILESON )&& \
      tree->first->next->next->id == FILESON )
      return 1;  /* clean parsing  */

   
printf("Parsing agaci qurulmadi. Metn kodun yoxlayin ve proqrami yeniden kompilyasiya edin. \n");
printf("Eger metnin dogru tertib olunduguna eminsinizse onda zehmet olmasa kodu ahmed.sadikhov@gmail.com unvanina gonderin.\n");
printf("\n--------------------------------------------\n");

print_tree_dx(tree);


//do_ast_job();

//required by check errors
init_prev_links();
check_errors_new();

printf("parse_tree after insert---------------------------\n");
//restart parser
parser_core();
print_tree_dx(tree); 

 
exit(1);


  return 0;
}

  int res = 0;



void init_parser(){

  tree = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  tree->first = tree->last = NULL;
  tree->count = 0;

  init_bottom_layer_new(tree);

}

int parser_core(){
  
  bottom_up_parse_dax_cap(tree);  
   check_dax_cap_errors(tree);

  bottom_up_parse_crg_dec(tree); 
  // no check for the moment

  bottom_up_parse_var(tree);
  // check_vars_errors(tree);

  bottom_up_parse_exprs(tree);
   check_exprs_erros(tree);

  bottom_up_parse_cnds(tree);
   check_cnds_errors (tree);

  bottom_up_parse_snmem(tree);
  bottom_up_parse_dax1_cap1(tree); 
  check_dax1_cap1_errors(tree);

  bottom_up_parse_simple_oprs(tree);
  check_simple_oprs_errors(tree); 

  convert_to_koddata_dca(tree);

  bottom_up_parse_funk_elan(tree); 
  check_funk_elan_errors(tree);

  bottom_up_parse_kod_or_var(tree);
  check_koddata_errors (tree);  

  bottom_up_parse_kod_funk(tree);

  bottom_up_parse_sinif(tree);
  check_sinif_errors (tree);   

  bottom_up_parse_text(tree);

	 return 0;
}

int parse(){

	init_parser();
	parser_core();
	check_parse_tree(tree);

	return 0;

}