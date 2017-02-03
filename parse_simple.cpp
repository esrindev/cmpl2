
  
  #include "esrin.h"

  #include <conio.h>

static int matchid(int k, int tok){

  return ( k  == tok);

}


static int cons_asgn_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

  if (matchid(a, IDT) && matchid(b,ASGN) && matchid(c, NUMB) && matchid(d,NOQTEV)){
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



static int build_cur_layer_simple_oprs (struct tok_list *tree){

  struct tok_list_el *tll, *ptr;

  int  pos;

  tll = tree->first;
 
  pos = 0;      /* how many elements scanned so far  */
 


  while (tll!=NULL){

    ptr = create_tok_list_el(tll->tok); /* null reference */


  if (cons_asgn_tok(ptr, &tll,   &pos))
      continue; 

   tll = tll->next;
  }

  return pos;
}


static void bottom_up_parse_simple_oprs(struct tok_list *tree){
 
    while(build_cur_layer_simple_oprs(tree));
    
}




 
static int cons_text_tok(struct tok_list_el *ptr, struct tok_list_el **tll_ref,  int *pos){

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

int parse_simple(){

	  int res = 0;

  tree = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  tree->first = tree->last = NULL;
  tree->count = 0;

  init_bottom_layer_new(tree);


    bottom_up_parse_simple_oprs(tree);

	 print_tree_dx(tree);

	 return 0;
}

