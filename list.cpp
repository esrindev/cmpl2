/* Lisence GPL V3   
   Ahmed Sadikhov 2014.22.oct
   ahmed.sadikhov@gmail.com
*/
 
 
#include "esrin.h"
#include "standart.h"


extern int prg_start; /* pos of program on exec_band*/

void addto_band_val(struct tok_list_el *tll, struct band_list_el band[], int *size);
void addto_band_adrs(struct tok_list_el *tll, struct band_list_el band[], int *size, struct tok_list_el **tll_last);
//void build_exec_band(struct tok_list_el *tll, struct band_list_el band[], int *size);

extern struct dec_table *get_paramts_table(unsigned long hid);

struct band_list_el  exec_band[BAND_SIZE] ;
int band_size, prg_start;


//exec_bandi qurduqdan sonra funk-larin head_pos -larini set edib
//fcall -ra menimsetmeliyik
struct gen_func_el{
	int id; // id of function
	unsigned long hid; // hid of funcs name
	int head_pos; // pos of func code at exec band
};

struct gen_func_table{
	struct gen_func_el gen_func_els[MAX_FUNC];
	int count;
} gft;


void add_gen_func_el(struct gen_func_table *gft, unsigned long hid, int h_pos){

	gft->gen_func_els[gft->count].head_pos = h_pos;
	gft->gen_func_els[gft->count].hid = hid;
	gft->count++;
}

void init_gen_func_table(struct band_list_el *exec_band, int size, struct gen_func_table *gft){

	struct band_list_el *bll;
	int i;

	gft->count = 0;

	for (i=0; i<size; ++i){
		bll = &exec_band[i];
		if (bll->band_id == FUNK)
			add_gen_func_el(gft, bll->hid, i);
	}
}

void print_gen_func_table(struct gen_func_table *gft){

	int i;

	printf("Gen func table \n");

	for (i=0; i<gft->count; ++i){
		printf("h_pos %d \n",gft->gen_func_els[i].head_pos);
	
	}
}

int get_func_head_pos(unsigned long hid, struct gen_func_table *gft){

	int i;

	for (i=0; i<gft->count; ++i)
		if (gft->gen_func_els[i].hid == hid)
			return gft->gen_func_els[i].head_pos;

	//error
	printf("Xeta: funksiya tapilmadi %ul\n", hid);
	return -1;

}

int get_std_func_id(unsigned long hid, struct std_func_table *sft){

	int i;

	for (i=0; i<sft->count; ++i)
		if (sft->std_funcs[i].hid == hid)
			return sft->std_funcs[i].id;

	return 0; //non standart function
}

void set_fcall_ids(struct band_list_el band[], int size, struct gen_func_table *gft){

	struct band_list_el *bll;
	int i;

	for (i=0; i<size; ++i){
		bll = &exec_band[i];
		if (bll->band_id == FCALL1){
			bll->head_pos = get_func_head_pos(bll->hid, gft);
			bll->std_func_id = 0; // set all to 0 means non std. For std id 's will be set later by func
		}
	}
}


void set_std_func_ids(struct band_list_el band[], int size, struct std_func_table *sft){

	struct band_list_el *bll;
	int i;

	for (i=0; i<size; ++i){
		bll = &exec_band[i];
		if (bll->band_id == FCALL1)
			bll->std_func_id = get_std_func_id(bll->hid, sft);
	}
}



static void std_hash(unsigned char *str, unsigned long *h){

  unsigned long hash = 5381;

  int c;

  if (str == NULL){
    *h =  0;
    return;
  }

  while (c = *str){
    hash = ((hash<<5)+hash)+c;
    str++;
  }

  *h = hash;
}


void set_std_func_hids(struct std_func_table *sft){

  unsigned long h;
  int i;

  for (i=0; i < sft->count; ++i){
	  std_hash((unsigned char *)sft->std_funcs[i].name, &h);
	  sft->std_funcs[i].hid = h;
  }
}



 

void set_func_ids(struct band_list_el band[], int size){

	init_gen_func_table(exec_band, size, &gft);
	print_gen_func_table(&gft);
	set_fcall_ids(exec_band, size, &gft);
	//standart fucs
	//build_std_func_table(sft);
	set_std_func_hids(&sft);
	set_std_func_ids(exec_band, size, &sft);

}

/* if IDT, NUMB, FLOAt, CHR ten just tok, if CRG1/2 then childs[0]->tok, if STRMEM  then ... check code */
struct token *head_tok(struct tok_list_el *tll){

  struct tok_list_el  *head_tll;
  struct token *head_tok;

  if (tll->id == STRMEM || tll->id == SNMEM)
    head_tll = tll->childs[0];
  else
    head_tll = tll;

  switch(head_tll->id){
  case IDT:
  case NUMB:
  case FLOAT:
  case CHR:
  case EXPR:/* why EXPR is here ???????? */
    head_tok = head_tll->tok;
    break;
  case CRG1:
  case CRG2:
    head_tok = head_tll->childs[0]->tok;
    break;
  default:
    head_tok = head_tll->tok;
  }

  return head_tok;
}


struct tok_list_el *create_tok_list_el(struct token *tok){

  struct tok_list_el *tmp = (struct tok_list_el *)malloc(sizeof(struct tok_list_el));

  tmp->tok  = tok;
  tmp->id   = tok->id;
  /*No Child*/ 
  tmp->childs[0] = NULL;
  tmp->cld_cnt   = 0;
  tmp->next = NULL;
  tmp->old_copy = 0;
  tmp->lrb = 0;
  
  return tmp;

}

int tok_list_ids[MAX_ERR_TKS];
int tok_list_len = 0;

void init_tok_list_ids(struct tok_list *tree){

	struct tok_list_el *tll = tree->first;

	while(tll != NULL){


	
	}




}

void tok_list_insert_el(struct tok_list *tl, struct tok_list_el **tll_ref, struct tok_list_el *tll_end, struct tok_list_el *ptr){

  (*tll_ref)->next = ptr;
  ptr->next = tll_end;
  tl->count++;      

}


void tok_list_add_el(struct tok_list *tl, struct tok_list_el *tmp){

  if (tl->count == 0){
    tl->first = tl->last = tmp;
      }
  else{
    tl->last->next = tmp;
    tl->last = tmp;
  }

  tl->count++;      

}


void init_bottom_layer_new(struct tok_list *tree){
  
int i;

  struct tok_list_el *tll;

  for (i=0; i<tks; ++i){
    tll = create_tok_list_el(&tokens[i]);
    tok_list_add_el(tree,tll);
  }

}

int get_tll_count(struct tok_list *tree){

	struct tok_list_el *tll = tree->first;
	int k=0;

	while (tll != NULL){
		k++;
		tll = tll->next;	
	}

	return k;
}

void init_tree(struct tok_list **tree_copy){

  *tree_copy = (struct tok_list *)malloc(sizeof(struct tok_list)) ;
  (*tree_copy)->first = (*tree_copy)->last = NULL;
  (*tree_copy)->count = 0;

}


void free_toklist(struct tok_list *tree){

	struct tok_list_el *ptll, *ntll;

	ntll = tree->first;

	while(ntll != NULL){
		ptll = ntll;
		ntll = ntll->next;
		free(ptll);
	}

	tree->first = tree->last = NULL;
	tree->count = 0;
}

void make_copy_of_toklist(struct tok_list *tree, struct tok_list *tree_copy){

	

	struct tok_list_el *tll, *ptll;

	free_toklist(tree_copy);

 	tll = tree->first;
	ptll = tree_copy->first;

	while(tll != NULL){
		ptll =  create_tok_list_el(tll->tok);
		ptll->id = tll->id;
		tok_list_add_el(tree_copy,ptll);
		tll = tll->next;
	}

	tree_copy->last = ptll;
}

// only SINIF, FUNKSIYA , OPNBLK and CLSBLK are copied
void init_bottom_layer_block(struct tok_list *tree){
  
int i;

  struct tok_list_el *tll;

  for (i=0; i<tks; ++i){
    if (tokens[i].id == SINIF   || tokens[i].id == FUNKSIYA || tokens[i].id == OPNBLK || tokens[i].id == CLSBLK ||\
		tokens[i].id == FILEBEG || tokens[i].id == FILESON ){
		tll = create_tok_list_el(&tokens[i]);
	    tok_list_add_el(tree,tll);
	}
  }
}


void addto_list(struct tok_list_el *tll, struct tok_list *plst){

  struct tok_list_el *tmp;

  //  tmp = create_new_tll(tll);
  tmp = (struct tok_list_el *)malloc(sizeof(struct tok_list_el));
  *tmp = *tll;

  if (plst->count == 0){
    /* first element  */
    plst->first = plst->last = tmp;
    plst->count = 1;
    tmp->next = tmp->prev = NULL;
  }
  else{
    plst->last->next = tmp;
    tmp->prev = plst->last;
    plst->last = tmp;
    tmp->next = NULL;
    (plst->count)++;
  }
}


void free_dovr_stek(struct stek *stk, int pos_dayan, int pos_davam){

  struct band_list_el *bll;
  void *ptr;

  while (stk->top != NULL){

	ptr = pop_from_stek(stk);
    bll = (struct band_list_el *)ptr;

    switch(bll->band_id){
    case INIT_DOVR:
    case NEQEDERKI2:
	case CHECK_SAY_TIP_1:
	case INIT_SAY_TIP_2:
      return;
    case DAVAMET1:
      bll->head_pos = pos_davam;
      break;
    case DAYAN1:
      bll->head_pos = pos_dayan;
      break;
      // default:
      //error
    }

  }
}

void free_secim_stek(struct stek *stk, int pos ){

  struct band_list_el *bll;

  while (stk->top != NULL){

    bll = (struct band_list_el *)pop_from_stek(stk);

    switch(bll->band_id){
    case FREE_SEC_DYN_STK:
      return;
    case DAVAMET1:
      printf("Sehv: davamet operatoru dovr daxilinde olmalidir.\n");
      break;
    case DAYAN1:
      bll->head_pos = pos;
      break;
    }
  }
}

void addto_band_new(struct tok_list_el *tll, struct band_list_el band[], int *size, int id, int hpos){

  struct tok_list_el *tmp;
  struct token *tok;

  if (*size > BAND_SIZE - 1){
	  printf("Sehv: Icra lenti dolub.\n");
	err_cnt++;
    return;
  }

  tok = head_tok(tll);

  band[*size].id = tll->id;
  band[*size].band_id = id;
  if ( tok->tok != NULL)
	strcpy(band[*size].tok, tok->tok);
  //maybe we have to remove upper strcopy 
  
  band[*size].d1 = tok->d1;
  band[*size].d2 = tok->d2;
  band[*size].row = tok->row;
  band[*size].col = tok->col;
  band[*size].tip = tok->tip;
  band[*size].size = tok->size;
  band[*size].lgm = tok->lgm;
  band[*size].ofst = tok->ofst;
  band[*size].fid = tok->fid;
  band[*size].sntip = tok->sntip;
  band[*size].hid = tok->hid;
  band[*size].head_pos = hpos;
  band[*size].std_func_id = 0;//std func ids will be set later by func


  //set val for constants

  //first set all to 0
  band[*size].int_val = 0;
  band[*size].float_val = 0;
  band[*size].char_val = 0;

  //now set exact value for given const
  switch(id){
	  case NUMB:
		  band[*size].int_val = atoi(tok->tok);
		  break;
	  case FLOAT:
		  band[*size].float_val = atof(tok->tok);
		  break;
	  case CHR:
		  band[*size].char_val = *(tok->tok);
		  break;
  }

  (*size)++;

}



int sntip = GLB_TIP;
int fid = 1;


struct tok_list_el *addto_band_ofst(struct tok_list_el *tll, struct band_list_el band[], int *size, struct tok_list_el **tll_last){

  struct tok_list_el *head, *ofst;

    
  switch(tll->id){
  case IDT:
  case CRG1:
  case CRG2:
  case FCALL1:
    head = tll;
    ofst = NULL;
    break;
  case STRMEM:
  case SNMEM:    
    head = tll->childs[0];
    ofst = tll->childs[2];
    break;
  }

  switch(head->id){
  case IDT:
    addto_band_new(head, band, size, IDT_OFST, 0);
    break;
  case CRG1:
    addto_band_val(head->childs[2], band, size);
    addto_band_new(head->childs[0], band, size, CRG1_OFST, 0);
    break;
  case CRG2:
    addto_band_val(head->childs[2], band, size);
    addto_band_val(head->childs[5], band, size);
    addto_band_new(head->childs[0], band, size, CRG2_OFST, 0);
    break;
  }
  
  if (ofst == NULL)
    *tll_last =  head;
  else
    addto_band_ofst(ofst, band, size, tll_last);

  return NULL; /* error  */
}

void addto_band_val(struct tok_list_el *tll, struct band_list_el band[], int *size){

  struct tok_list_el *tll_last;

  switch(tll->id){
  case ARTSIN:
    addto_band_new(tll, band, size, ARTSIN, 0);
    break;
  case AZALSIN:
    addto_band_new(tll, band, size, AZALSIN, 0);
    break;
  case NUMB:
    addto_band_new(tll, band, size, NUMB, 0);
    break;
  case FLOAT:
    addto_band_new(tll, band, size, FLOAT, 0);
    break;
  case CHR:
    addto_band_new(tll, band, size, CHR, 0);
    break;
  case IDT:
    addto_band_new(tll, band, size, IDT_VAL, 0);
    break;
  case CRG1:
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[0], band, size, CRG1_VAL, 0);
    break;
  case CRG2:
    addto_band_val(tll->childs[2], band, size);
    addto_band_val(tll->childs[5], band, size);
    addto_band_new(tll->childs[0], band, size, CRG2_VAL, 0);
    break;
  case EXPR:
    addto_band_val(tll->childs[0], band, size);
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[1], band, size, EXPR, 0);
    break;
  case SHERT:
    addto_band_val(tll->childs[0], band, size);
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[1], band, size, SHERT, 0);
    break;  
  case STRMEM:
    addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_ofst(tll->childs[2], band, size, &tll_last);
    addto_band_new(tll_last, band, size, STRMEM_VAL, 0);
    break;
  case SNMEM:
  case FCALL1:
    build_exec_band_for_main(tll, band, size);
	/* get val from func_ret_res and push to valstk   */
	addto_band_new(tll, band, size, GET_FUNC_RET_RES , 0);
    break;
  }

}

void addto_band_adrs(struct tok_list_el *tll, struct band_list_el band[], int *size, struct tok_list_el **tll_last){

  int i,id;

  switch(tll->id){
  case IDT:
    addto_band_new(tll, band, size, IDT_ADRS, 0);
    *tll_last = tll;
    break;
  case CRG1:
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[0], band, size, CRG1_ADRS, 0);
    *tll_last = tll->childs[0];    
    break;
  case CRG2:
    addto_band_val(tll->childs[2], band, size);
    addto_band_val(tll->childs[5], band, size);
    addto_band_new(tll->childs[0], band, size, CRG2_ADRS, 0);
    *tll_last = tll->childs[0];    
    break;
 case STRMEM:
   addto_band_adrs(tll->childs[0], band, size, tll_last);
   addto_band_ofst(tll->childs[2], band, size, tll_last);
   break;
  case CHR:
    addto_band_new(tll, band, size, CHR_ADRS, 0);
    *tll_last = tll;
    break;
  case STR:
    addto_band_new(tll, band, size, STR_ADRS, 0);
    *tll_last = tll;
    break;
  }

}

void addto_band_capet(struct tok_list_el *tll, struct band_list_el band[], int *size){
  int i;
  struct tok_list_el *tll_last;

  switch(tll->id){
  case NUMB:
    addto_band_new(tll,band, size, NUMB, 0);
    addto_band_new(tll,band, size, CAPET_NUMB,0);    
    return;
  case FLOAT:
    addto_band_new(tll,band, size, FLOAT, 0);
    addto_band_new(tll,band, size, CAPET_FLOAT, 0);
    return;
  case CHR:
    addto_band_new(tll,band, size, CHR, 0);
    addto_band_new(tll, band, size, CAPET_CHR, 0);
    return; 
  case STR:
    addto_band_adrs(tll,band, size, &tll_last);
    addto_band_new(tll,band, size, CAPET_STR, 0);
    return;
  case IDT:
    addto_band_new(tll,band, size, IDT_VAL, 0);
    addto_band_new(tll,band, size, CAPET_IDT, 0);
    return;
  case CRG1:
    addto_band_val(tll->childs[2], band, size);
    addto_band_new(tll->childs[0],band, size, CAPET_CRG1, 0);
    return;
  case CRG2:
    addto_band_val(tll->childs[2], band, size);
    addto_band_val(tll->childs[5], band, size);
    addto_band_new(tll->childs[0],band, size, CAPET_CRG2, 0);
    return;
  case STRMEM:
    addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_ofst(tll->childs[2], band, size, &tll_last);
    addto_band_new(tll_last, band, size, CAPET_STRMEM, 0);
    return;
  case EXPR:
    addto_band_val(tll, band, size);
    addto_band_new(tll, band, size, CAPET_EXPR, 0);
    return;
  }
  
  for (i=0; i < tll->cld_cnt ; ++i)
    addto_band_capet(tll->childs[i], band, size);

}


void addto_band_daxilet(struct tok_list_el *tll, struct band_list_el band[], int *size){
  int i;
  struct tok_list_el *tll_last;

  switch(tll->id){
  case IDT:
  case CRG1:
  case CRG2:
  case STRMEM:
    addto_band_adrs(tll, band, size, &tll_last);
    addto_band_new(tll_last,band, size, DAXILET_IDT, 0);
    return;
  }
  
  for (i=0; i < tll->cld_cnt ; ++i)
    addto_band_daxilet(tll->childs[i], band, size);
}



void add_fargs_to_band(struct tok_list_el *tll, struct band_list_el band[], int *size){

  int i ;

  switch(tll->id){
  case NUMB:
  case FLOAT:
  case CHR:
  case STR:
  case IDT:
  case CRG1:
  case CRG2:
  case EXPR:
  addto_band_val(tll, band, size);
  return ;
  }

    for (i = 0; i<tll->cld_cnt; ++i)
      add_fargs_to_band(tll->childs[i], band, size);

    return;
}


void add_fparms_to_band(struct tok_list_el *tll, struct band_list_el band[], int *size){

  struct tok_list_el *tmp;
  int i, old_size, id = tll->id;
  struct tok_list_el *tll_last;

  /* currently  we support only passing variables of type 1,2 or 3 and not arrays to functions */
  if (tll->cld_cnt == 2 || tll->id == IDT ){
    addto_band_new(tll->childs[1], band, size, FPARM_IDT_VAL, 0);
    return ;
  }
  

  /* add args in revers order for stack reasons  */
    for (i = tll->cld_cnt - 1; i>=0; --i)
      add_fparms_to_band(tll->childs[i], band, size);

    return;
}



void add_fparms_to_band_new(struct tok_list_el *tll, struct band_list_el band[], int *size){

	struct dec_table *pt;
	int i;

	pt = get_paramts_table(tll->tok->hid);

	for (i=0; i<pt->count; ++i)
		addto_band_new(pt->decs[i].tll, band, size, FPARM_IDT_VAL, 0);

    return;
}

void addto_band_HAL1S(struct tok_list_el *tll, struct band_list_el band[], int *size, int *hal1_cnt){

  int i;

  if (tll->id == HAL1){
    addto_band_val(tll->childs[1], band, size);
	tll->childs[0]->tok->tip = tll->tok->tip;
    addto_band_new(tll->childs[0], band, size, HAL1S, 0); 
    *hal1_cnt += 2;
    return;
  }

  for (i=0; i<tll->cld_cnt; ++i){
	tll->childs[i]->tok->tip = tll->tok->tip;
    addto_band_HAL1S(tll->childs[i], band, size, hal1_cnt);
  }

  return;
}

 


void build_exec_band_funks(struct tok_list_el *tll, struct band_list_el band[], int *size){

  int i, hpos, old_size, old_size1, old_size2, id ;
  struct tok_list_el *tmp, *tll_last;
  struct token *tok;

  if (*size > BAND_SIZE){
    printf("Icra lenti dolub.\n");
    return;
  }

  id = tll->id;

  switch(id){

  case TEXT:
  case KOD_FUNK:
  case KOD_PART:


    for (i = 0; i<tll->cld_cnt; ++i)
      build_exec_band_funks(tll->childs[i], band, size);
    break;

  case FUNK:
	  printf("got funk\n");
	tok = tll->childs[1]->childs[0]->tok;
    fid = tok->fid;
    addto_band_new(tll->childs[1]->childs[0], band, size, FUNK, 0); 
    add_fparms_to_band_new(tll->childs[1]->childs[0], band, size);
    build_exec_band_for_main(tll->childs[2], band, size);
    addto_band_new(tll->childs[0], band, size, QAYTAR1, 0); 
    fid++;
    break;

  }
}




/* traverse tree and extract all kod except funcs and classes kod  */
void build_exec_band_for_main(struct tok_list_el *tll, struct band_list_el band[], int *size){

  int i, hpos, old_size, old_size1, old_size2, id;
  struct tok_list_el *tmp, *tll_last;
  struct token *tok;

  id = tll->id;

  if (*size > BAND_SIZE){
    printf("Icra lenti dolub.\n");
    return;
  }

  switch(id){

   case FUNK:
	   return;

  case TEXT:
  case KOD_FUNK:
  case KOD_PART:
  case BLOCK:
  case KODDATA:
  case HAL2:
  case COMP_OPER:
  case SIMPLE_OPER:

    if (tll->childs[0]->id == DOVR_EXP){
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_val(tll->childs[0]->childs[4], band, size);
      addto_band_new(tll_last, band, size, INIT_DOVR, 0); /* INIT_DOVR  */
      push_to_stek(&dovrstk, &band[*size-1]);
      
      old_size = *size; /* we'll jump here from STEP_DOVR  */
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_val(tll->childs[0]->childs[7], band, size);
      addto_band_val(tll->childs[0]->childs[3], band, size);
      old_size1 = *size;
    addto_band_new(tll_last, band, size, CHECK_DOVR, 0); /* CHECK_DOVR  */

         build_exec_band_for_main(tll->childs[1], band, size);

      old_size2 = *size;
      addto_band_val(tll->childs[0]->childs[3], band, size);
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
    addto_band_new(tll_last, band, size, STEP_DOVR, old_size); /* STEP_DOVR  */
      /* jump out of loop to next instr. 3 is ofst from INIT_DOVR  */     
      band[old_size1].head_pos = *size;  
      free_dovr_stek(&dovrstk, *size, old_size2);
      break;
    }


  if (tll->childs[0]->id == SAY_TIP_1){
      old_size = *size; /* we'll jump here from STEP_DOVR  */
      addto_band_val(tll->childs[0]->childs[2], band, size);
      old_size1 = *size;
      addto_band_new(tll, band, size, CHECK_SAY_TIP_1, 0); /* CHECK_DOVR  */
	  push_to_stek(&dovrstk, &band[*size-1]);

      build_exec_band_for_main(tll->childs[1], band, size);

      old_size2 = *size;
      addto_band_new(tll, band, size, STEP_SAY_TIP_1, old_size1); /* STEP_DOVR  */
      /* jump out of loop to next instr. 3 is ofst from INIT_DOVR  */     
      band[old_size1].head_pos = *size;  
      free_dovr_stek(&dovrstk, *size, old_size2);
      break;
    }


    if (tll->childs[0]->id == SAY_TIP_2){
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_new(tll_last, band, size, INIT_SAY_TIP_2, 0); /* INIT_DOVR  */
      push_to_stek(&dovrstk, &band[*size-1]);
      
      old_size = *size; /* we'll jump here from STEP_DOVR  */
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_val(tll->childs[0]->childs[3], band, size);
      old_size1 = *size;
      addto_band_new(tll_last, band, size, CHECK_SAY_TIP_2, 0); /* CHECK_DOVR  */

      build_exec_band_for_main(tll->childs[1], band, size);

      old_size2 = *size;
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_new(tll_last, band, size, STEP_SAY_TIP_2, old_size); /* STEP_DOVR  */
      /* jump out of loop to next instr. 3 is ofst from INIT_DOVR  */     
      band[old_size1].head_pos = *size;  
      free_dovr_stek(&dovrstk, *size, old_size2);
      break;
    }

	  if (tll->childs[0]->id == SAY_TIP_3){
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_val(tll->childs[0]->childs[3], band, size);
      addto_band_new(tll_last, band, size, INIT_SAY_TIP_3, 0); /* INIT_DOVR  */
      push_to_stek(&dovrstk, &band[*size-1]);
      
      old_size = *size; /* we'll jump here from STEP_DOVR  */
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_val(tll->childs[0]->childs[3], band, size); /* ilk  */
      addto_band_val(tll->childs[0]->childs[4], band, size); /* son  */
      old_size1 = *size;
      addto_band_new(tll_last, band, size, CHECK_SAY_TIP_3, 0); /* CHECK_DOVR  */

      build_exec_band_for_main(tll->childs[1], band, size);

      old_size2 = *size;
      addto_band_val(tll->childs[0]->childs[3], band, size);  /* ilk  */
	  addto_band_val(tll->childs[0]->childs[4], band, size);  /* son  */
      addto_band_adrs(tll->childs[0]->childs[2], band, size, &tll_last);
      addto_band_new(tll_last, band, size, STEP_SAY_TIP_3, old_size); /* STEP_DOVR  */
      /* jump out of loop to next instr. 3 is ofst from INIT_DOVR  */     
      band[old_size1].head_pos = *size;  
      free_dovr_stek(&dovrstk, *size, old_size2);
      break;
    }


    if (tll->childs[0]->id == NEQEDERKI2){
      old_size = *size;
      addto_band_val(tll->childs[0]->childs[1], band, size);      
     // push_to_stek(&dovrstk, tll->childs[0]);
      old_size1 = *size;
      addto_band_new(tll->childs[0], band, size, NEQEDERKI2, 0); 
	  push_to_stek(&dovrstk, &band[*size-1]);
      build_exec_band_for_main(tll->childs[1], band, size);      
      addto_band_new(tll->childs[0], band, size, NEQ_TOP, old_size);      
      band[old_size1].head_pos = *size;  
      free_dovr_stek(&dovrstk, *size, old_size);
      break;
    }

	    //addto_band_new(tll, band, size, tll->id, 0);
	 //push_to_stek(&dovrstk, &band[*size-1]);


    if (tll->childs[0]->id == EGER_EXP2){
      if (tll->cld_cnt == 2){ // if ...
	addto_band_val(tll->childs[0]->childs[1], band, size);
	old_size = *size;
	addto_band_new(tll->childs[0], band, size, EGER_EXP2, 0);
	build_exec_band_for_main(tll->childs[1], band, size);
	band[old_size].head_pos = *size;  	
      }
      else{ // if ... else ...
	addto_band_val(tll->childs[0]->childs[1], band, size);
	old_size = *size;
	addto_band_new(tll->childs[0], band, size, EGER_EXP2, 0);	
	build_exec_band_for_main(tll->childs[1], band, size);
	band[old_size].head_pos = *size + 1;  	 // +1 for else
	old_size = *size;
	addto_band_new(tll->childs[0], band, size, YOXSA, 0);		
	build_exec_band_for_main(tll->childs[3], band, size);	
	band[old_size].head_pos = *size;  		
      }
      break;      
    }


    if (tll->childs[0]->id == SEC1){
      old_size = *size;
      addto_band_adrs(tll->childs[1], band, size, &tll_last);
	  addto_band_new(tll->childs[0], band, size, FREE_SEC_DYN_STK, 0); 
	  push_to_stek(&dovrstk, &band[*size-1]);
      //copy tip from tll_last to hal2
	  tll->childs[4]->tok->tip = tll_last->tok->tip;
      build_exec_band_for_main(tll->childs[4], band, size);      
      addto_band_new(tll->childs[0], band, size, SEC1, 0); 
      free_secim_stek(&dovrstk, *size);
      /* just rem switch variable from address stek  */	
      break;      
    }


     if (tll->id == HAL2){

     //copy tip from hal2
	tll->childs[0]->tok->tip = tll->tok->tip;

      if (tll->childs[0]->id == HAL1){
	addto_band_val(tll->childs[0]->childs[1], band, size);
	addto_band_new(tll->childs[0], band, size, HAL1, 0);
	old_size = *size;
	build_exec_band_for_main(tll->childs[1], band, size);      
	band[old_size-1].head_pos = *size;  		      
	break;     
      }

      if (tll->childs[0]->id == SUS1){
	addto_band_new(tll->childs[0], band, size, SUS1, 0); 
	build_exec_band_for_main(tll->childs[1], band, size);      
	break;    
      }

      if (tll->childs[0]->id == HAL1S){
	int j, hal1_cnt = 0;
	old_size = *size;
	addto_band_HAL1S(tll->childs[0], band, size, &hal1_cnt);
	old_size1 = *size;
	build_exec_band_for_main(tll->childs[1], band, size); 
	for (j=1; j<hal1_cnt; j += 2)
	  band[old_size + j].head_pos = old_size1;  		      
	band[old_size1 - 1].head_pos = *size;  
	band[old_size1 - 1].band_id = HAL1;
	// set tip here also
	band[old_size1 - 1].tip = tll->tok->tip;
	break;      
      }

      build_exec_band_for_main(tll->childs[0], band, size);      
      build_exec_band_for_main(tll->childs[1], band, size);      
      break;
    }


    for (i = 0; i<tll->cld_cnt; ++i)
      build_exec_band_for_main(tll->childs[i], band, size);
    break;

  case DAVAMET1:
  case DAYAN1:    
    addto_band_new(tll, band, size, tll->id, 0);
    push_to_stek(&dovrstk, &band[*size-1]);
    break;
  case CAPET3:    
    addto_band_capet(tll->childs[0]->childs[1], band, size);
    break;
  case DAXILET3:
    addto_band_daxilet(tll->childs[0]->childs[1], band, size);
    break;
  case ASGN_OP:
    addto_band_val(tll->childs[2], band, size);
    addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_new(tll_last, band, size, ASGN, 0); 
    break;
  case DECREMENT_OPR:
	  if (tll->childs[0]->id == DECREMENT)
		addto_band_adrs(tll->childs[1], band, size, &tll_last);
	  else
		addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_new(tll_last, band, size, DECREMENT_OPR, 0); 
    break;
  case INCREMENT_OPR:
	  if (tll->childs[0]->id == INCREMENT)
		addto_band_adrs(tll->childs[1], band, size, &tll_last);
	  else
		addto_band_adrs(tll->childs[0], band, size, &tll_last);
    addto_band_new(tll_last, band, size, INCREMENT_OPR, 0); 
    break;
  case STANDART_FUNK:
	  addto_band_new(tll, band, size, tll->childs[1]->id, 0); 
	 // addto_band_new(tll, band, size, tll->childs[1]->id, 0); 
	  break;

  case STEKE_QOY_STD:
	addto_band_val(tll->childs[1], band, size);
	break;

  case STEKDEN_GOTUR_STD:
	addto_band_adrs(tll->childs[1], band, size, &tll_last);
	addto_band_new(tll_last, band, size, ASGN, 0); 
	break;

    /* in above case (FUNK) we add QAYTAR1 to band at the end. This is just
       to return properly if user will not write qaytar for void funcs.
       next case (QAYTAR1) however is used for returning value or by user
       request. */

	/* diqqet main ucun band yiganda QAYTAR1 -i SON -la evez edirik  
	    mainin qaytardigi neticeni heleki inkar edirik, gelecekde lazim olsa
		 istifade elemek olar. ahms.
	   */
  /*case QAYTAR1:

    addto_band_new(tll->childs[0], band, size, SON, 0); 
	break;
	*/

  case QAYTAR1:
    if (tll->cld_cnt == 3){
      addto_band_val(tll->childs[1], band, size);
	  /* pop val from valstk and save it in func_ret_res   */
	  addto_band_new(tll, band, size, PUT_FUNC_RET_RES, 0);
      addto_band_new(tll, band, size, QAYTAR1, 0); 
    }
	else
	  addto_band_new(tll, band, size, QAYTAR1, 0); 
      
    break;

  case FCALL1:
    /* push return address to glbstk  */
    /* TODO:  remove tmp which is not clear why is it here */
    old_size = *size;
    addto_band_new(tll, band, size, PUT_RET_ADDR, 0); 
    add_fargs_to_band(tll->childs[1], band, size);
    addto_band_new(tll->childs[0], band, size, FCALL1, hpos);
    /* set dt_area and fields for funcs  */
   // band[*size - 1].dt_size = tt->tips[tok->sntip].ft.funcs[tok->fid].dt.size;
    band[old_size].head_pos = *size;
	addto_band_new(tll, band, size, POP_FNSTK, 0);
    break;

 }
}




void print_exec_band(struct band_list_el band[], int size){

  int i;

  for (i=0; i<size; ++i){
    printf("hpos -> %d\t",i);
    print_tok(band[i].id);
    printf("\t");
    print_tok(band[i].band_id);
	printf("\thpos = %d\tlgm %d\ttok %s\ttip %d\tofst %d\tstd_func_id %d", band[i].head_pos, band[i].lgm, band[i].tok,  band[i].tip, band[i].ofst, band[i].std_func_id);
    if (band[i].band_id == FUNK || band[i].band_id == FCALL1 || \
	band[i].band_id == SNMEM)
      printf("%s sntip %d  fid  %d   dt_size %d\n ", band[i].tok, band[i].sntip, band[i].fid, band[i].dt_size);
    else{
      if (band[i].band_id == GLBMT)
	printf(" dt_size  %d", band[i].dt_size);
      printf("\n");
    }
  }
}


void init_fcalls_jmp_addrs(struct band_list_el band[], int size)
{

  int i,j;

  for (i=0; i<size; ++i)
  {
    if (band[i].band_id == FUNK)
	{
      for (j=0; j<size; ++j)
	  {
		if (band[j].band_id == FCALL1 || band[j].band_id == SNMEM)
		{
		 if (band[i].sntip == band[j].sntip && band[i].fid == band[j].fid)
			 band[j].head_pos = i;
		}
      }
    }
  }
}

struct pre_stek_el{
  long double el;
  struct pre_stek_el *next;
};

struct pre_stek{
  struct pre_stek_el *top;
  int count;
};


void push_to_pre_stek(struct pre_stek *st, long double k){

  struct pre_stek_el *stl;

  stl = (struct pre_stek_el *)malloc(sizeof(struct pre_stek_el));
  stl->el = k;
  stl->next = st->top;
  st->top = stl;
  (st->count)++;
}

struct pre_stek_el *pop_from_pre_stek(struct pre_stek *st){

  struct pre_stek_el *stl;

  if (st->count != 0){
    stl = st->top;
    st->top = st->top->next;
    stl->next= NULL;
    (st->count)--;
    return stl;
  }

  return NULL; /* steck is empty  */
}





int print_layer_dx_tr(struct tok_list_el *tll, int depth, int n){

  int i,m;

  m = 0;

  if (depth == n){
    print_tok(tll->id);
	//printf(" %d ", tll->id);
    printf(" ");
    //    printf(" %s %p ",tll->tok->tok, tll->tok);

    if (tll->lrb == 1)
      return 1;   
    else
      return 0;
  }

  if (tll->lrb == 0)
    return 0;

  for (i=0; i<tll->cld_cnt; ++i)
    m+=print_layer_dx_tr(tll->childs[i], depth, n+1);   

  return m;

}


int print_layer_dx(struct tok_list *tl, int depth){

  struct tok_list_el *tll;
  int  k;

  k = 0;

  tll = tl->first;
  
  while (tll!=NULL){
    k+=print_layer_dx_tr(tll, depth, 1);
    tll = tll->next;
  }
  
  return k;
}


void print_tree_dx(struct tok_list *tl){

  int depth, cnt;

  depth = 0;
  cnt = 1;

  while(cnt){
    depth++;
    cnt = print_layer_dx(tl, depth);
    printf("\n");
 }
}

 
void print_tok_list(struct tok_list *tlist){

	struct tok_list_el *tll = tlist->first;

	while (tll != NULL){
	
		print_tok(tll->id);
		printf(" ");
		tll = tll->next;
	}

}