
#include "esrin.h"
#include "sehvler_3.h"

#if 0
static void get_row_col_right(struct tok_list_el *tll, int *row, int *col, char **pt){

  if (tll->cld_cnt == 0){ /* reached far right leave, get row and col  */
    *row =  tll->tok->row;
    *col =  tll->tok->col;
    *pt  =  tll->tok->tok;
    return ;
  }
  
  /* not yet reached, continue by far right branch  */
  get_row_col_right(tll->childs[tll->cld_cnt-1], row, col, pt);
}

static void get_row_col_left(struct tok_list_el *tll, int *row, int *col, char **pt){

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
static void print_before(struct tok_list_el *tll, char *msg){

	int row, col;

    get_row_col_left(tll, &row, &col, &pt);    
	memset(err_msg, '\0', ERR_MSG_LEN);
    sprintf(err_msg, "(%d:%d)sehv: `%s` -dan evvel %s gozlenirdi.\n", row, col, pt,msg);    
	add_to_errors(row, col, err_msg);
	memset(ins_tll_ary[ins_tll_cnt].msg, '\0', 1024);
	sprintf(ins_tll_ary[ins_tll_cnt].msg, "%s", err_msg);
}

static void print_after(struct tok_list_el *tll, char *msg){

	int row, col;

	get_row_col_right(tll, &row, &col, &pt); 
	memset(err_msg, '\0', ERR_MSG_LEN);
    sprintf(err_msg, "(%d:%d)sehv: `%s` -dan sonra %s gozlenirdi.\n", row, col, pt,msg);    
	add_to_errors(row, col, err_msg);
	memset(ins_tll_ary[ins_tll_cnt].msg, '\0', 1024);
	sprintf(ins_tll_ary[ins_tll_cnt].msg, "%s", err_msg);
}

#endif

static int is_terminal(int id){

	int i;

	for (i=0; i< terminal_set_len; ++i)
		if (terminal_set[i] == id)
			return 1;

	return 0;

}

void print_terminals(struct tok_list *tree){

	struct tok_list_el *tll;

	tll = tree->first->next;
	
	printf("Terminals: ");

	while (tll->id != FILESON){
		if (is_terminal(tll->id)) 
			printf("%s %d:%d \n", tll->tok->tok, tll->tok->row, tll->tok->col);
		tll = tll->next;			
	}
}

//check if id2 is in id1's follow list
int following(int id1, int id2){

	struct follow *fl;
	int i ;

	fl = &follow_set_fin_2[id1];

	for (i=0; i<fl->len; ++i)
		if (fl->fol[i] == id2)
			return 1;

	return 0;

}

void check_follow(struct tok_list *tree){

	struct tok_list_el *tll;
	
	tll= tree->first->next;

	while(tll->id != FILESON){
	
		if (is_terminal(tll->id)){
			if (!following(tll->id, tll->next->id)){
				printf("(%d:%d)sehv:   %s -den sonra dogru ifade yazilmayib\n",  tll->tok->row, tll->tok->col, tll->tok->tok);
			}
		}

		tll = tll->next;
	}

}

void sehvleri_yoxla_yeni(struct tok_list *tree){


	printf("Checking grammar errors\n");

#if 0

	struct tok_list_el *tll = tree->first;

  if (tree->first->id == FILEBEG    && \
      (tree->first->next->id == TEXT || tree->first->next->id == FILESON )&& \
	  tree->first->next->next->id == FILESON ){
		printf("No syntax errors.\n");
		return ;  /* clean parsing  */
  }
  
 
#endif

   do_ast_job();

   	check_follow(tree);

	printf("Parsing agaci qurulmadi. Metn kodun yoxlayin ve proqrami yeniden kompilyasiya edin. \n");
	//printf("Eger metnin dogru tertib olunduguna eminsinizse onda zehmet olmasa kodu ahmed.sadikhov@gmail.com unvanina gonderin.\n");
	printf("\n--------------------------------------------\n");

  
 
}