#include "esrin.h"
#include "rule_set.h"
#include "token_rules_1_set.h"
#include "follow_set.h"

extern void make_copy_of_toklist(struct tok_list *tree, struct tok_list *tree_copy);
extern int get_tll_count(struct tok_list *tree);
extern void init_tree(struct tok_list **tree_copy);
extern int parser_core(struct tok_list *tree);

void print_prev_links(){

	struct tok_list_el *tll = tree->last;

	printf("prevlinks\n");

	while(tll->id != FILEBEG){
		print_tok(tll->id);	
		printf(" ");
		tll = tll->prev;
	}

	printf("\n");
}


void print_next_links(){

	struct tok_list_el *tll = tree->first->next;

	printf("nextlinks\n");

	while(tll->id != FILESON){
		print_tok(tll->id);	
		printf(" ");
		tll = tll->next;
	}

	printf("\n");
}

/* init trees tok_list_els (laytest layer) prev links 
   and trees last nodes reference  */
void init_prev_links_new(){

	struct tok_list_el *prev, *tll = tree->first->next;

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




struct token_rules_1 *get_token_rules(int id){

	int i, ind = -1;

	for (i=0; i<token_rules_1_set_len; ++i)
		if (token_rules_1_set[i].id == id ){
			ind = i;
			break; 
		}

    // found rule set for given token
	if (ind >= 0)
		return &token_rules_1_set[ind];

	
	//no rule_set for given token
	return NULL;
}


void print_token_rules(struct token_rules_1 *tr){

	int j;

	printf("\nrules for token ");
	print_tok(tr->id);
	printf(" :");

	for (j=0; j<tr->len; ++j)
		printf("%d ,", tr->rules[j] );

	printf("\n");
}

void print_prev_curr_next(int prev2, int prev1, int curr, int next1, int next2){

	if (prev2 >= 0)
		print_tok(prev2);
	else
		printf("NA");

	printf(" ");

	if (prev1 >= 0)
		print_tok(prev1);
	else
		printf("NA ");

	printf(" ");

	print_tok(curr);

	printf(" ");

	if (next1 >= 0)
		print_tok(next1);
	else
		printf("NA ");

	printf(" ");

	if (next2 >= 0)
		print_tok(next2);
	else
		printf("NA ");

	//NA - i.e. not applicable
	printf(" \n");

}
void get_prev_cur_next(struct tok_list_el *tll, int *prev2, int *prev1, int *curr, int *next1, int *next2){

	*prev1 = *prev2 = *next1 = *next2 = -1;
	*curr = tll->id; 

	if (tll->prev != NULL) 
		*prev1 = tll->prev->id;
	else
		goto _next1;

	if (tll->prev->prev != NULL) 
		*prev2 = tll->prev->prev->id;

_next1:
	if (tll->next != NULL) 
		*next1 = tll->next->id;
	else
		goto out;

	if (tll->next->next != NULL) 
		*next2 = tll->next->next->id;

out:
	return;
}


void print_rule(struct rule_new *rl ){

	int i;

	printf("\n");

	for (i=0; i<rl->len; ++i){
		print_tok(rl->left[i]);
		printf(" ");
	}
	
	printf("\n");
}

int get_cur_pos(int curr, struct rule_new *rl){

	int i;

	for (i=0; i<rl->len; ++i)
		if (rl->left[i] == curr)
			return i;

	return -1;
}

void get_rl_prev_curr_next(int *prev2, int *prev1, int *next1, int *next2, int cur_pos, struct rule_new *rl){

	  *prev1 = *prev2 = *next1 = *next2 = -1;

	  if (cur_pos - 2 >= 0)
		  *prev2 = rl->left[cur_pos - 2];

	  if (cur_pos - 1 >= 0)
		  *prev1 = rl->left[cur_pos - 1];

	  if (cur_pos + 1 <= rl->len - 1)
		  *next1 = rl->left[cur_pos + 1];

	  if (cur_pos + 2 <= rl->len - 1)
		  *next2 = rl->left[cur_pos + 2];
}

int get_satisfaction(int prev2, int prev1, int curr, int next1,int next2, int rule_id){

	struct rule_new *rl = &rule_new_set[rule_id];
	int rl_prev2, rl_prev1, rl_next1, rl_next2, cur_pos;

	print_rule(rl);

	cur_pos = get_cur_pos(curr, rl);

	if (cur_pos < 0){
		printf("daxili xeta: get_satisfaction\n");
		return 0;
	} 

	get_rl_prev_curr_next(&rl_prev2, &rl_prev1, &rl_next1, &rl_next2, cur_pos, rl);

	printf("prev next of the token \n");
	print_prev_curr_next(prev2, prev1, curr, next1, next2);
	printf("prev next of the rule \n");
	print_prev_curr_next(rl_prev2, rl_prev1, curr, rl_next1, rl_next2);

	//calculate satisfaction
	int sat = 0; // not satisfied

	if (prev1 == -1)
		goto next1;

	if (prev1 == rl_prev1)
		sat++;

	if (prev2 == -1)
		goto next1;

	if (prev2 == rl_prev2)
		sat++;

next1:

	if (next1 == -1)
		goto out;

	if (next1 == rl_next1)
		sat++;

	if (next2 == -1)
		goto out;

	if (next2 == rl_next2)
		sat++;

out:
	printf("sat is %d\n", sat);
	return sat;
}

void print_satisfy(struct tok_list_el *tll){

	int i;

	printf("satisfaction for ");
	print_tok(tll->id);
	printf("\t");

	for (i=0; i<tll->sat_len; ++i)
		printf("%d ", tll->sat[i]);

	printf("\n");
}


void calc_satisfy(struct tok_list_el *tll){

	struct token_rules_1 *tr;
	int prev2, prev1, curr, next1, next2, i;
	int stf_max, rl_stf;

	get_prev_cur_next(tll, &prev2, &prev1, &curr, &next1, &next2);

	tr = get_token_rules(curr);

	if (tr!=NULL) 
		print_token_rules(tr); 
	else{
		printf("no rules for");
		print_tok(curr);
		printf("\n");
		return; 
	}


	stf_max = -1;
	rl_stf = -1;

	for (i=0; i < tr->len; ++i){
		tll->sat[i] = get_satisfaction(prev2, prev1, curr, next1, next2, tr->rules[i]);
		if (tll->sat[i] > stf_max){
			stf_max = tll->sat[i];
			rl_stf = i;
		}
	}


	printf("prev next of token list\n");
	print_prev_curr_next(prev2, prev1, curr, next1, next2);

	printf("max satisfaction for token ");
	print_tok(tll->id);
	printf(" %d, and rule is\n", stf_max);
	if (rl_stf >= 0)
		print_rule(&rule_new_set[tr->rules[rl_stf]]);
	else
		printf("not satisfied \n");
	printf("\n");


	tll->sat_len = tr->len;
}

void init_satisfaction(){

	struct tok_list_el *tll = tree->first->next;

	while(tll->id != FILESON){
		calc_satisfy(tll);
		print_satisfy(tll);
		tll = tll->next;
	}
 }

struct tok_list_el *get_tll_adrs(struct tok_list *tree, int pos){

	struct tok_list_el *tll = tree->first->next;
	int i;

	for (i=0; i<pos; ++i)
		tll = tll->next;

	return tll;
}

void get_row_col_right_new(struct tok_list_el *tll, int *row, int *col, char **pt){

  if (tll->cld_cnt == 0){ /* reached far right leave, get row and col  */
    *row =  tll->tok->row;
    *col =  tll->tok->col;
    *pt  =  tll->tok->tok;
    return ;
  }
  
  /* not yet reached, continue by far right branch  */
  get_row_col_right_new(tll->childs[tll->cld_cnt-1], row, col, pt);
}

//insert new tll next to  pos on tok_list
void insert_tll_next_new(struct tok_list *tree, int pos, int id){

	struct tok_list_el *tll, *ptll;

	ptll = get_tll_adrs(tree, pos);

	get_row_col_right_new(ptll, &ptll->tok->row, &ptll->tok->col, &ptll->tok->tok);

	tll = create_tok_list_el(ptll->tok);

	tll->id = id;

	if (ESR_DEBUG){

	printf("\ninserting ");
	print_tok(id);
	printf(" after ");
	print_tok(ptll->id);
	printf("\n");
	}

	//now insert
	tll->next = ptll->next;
	ptll->next = tll;

	//print_tok_list(tree);
}

int  get_best_missing_tok(struct tok_list *ctree, int pos, int id, int *mis_tok){

	struct tok_list_el *tll = ctree->first->next;
	struct tok_list *tmp_tree;

	int i, j, k, min,  cur_tok;

	tll = get_tll_adrs(ctree, pos);
	min = get_tll_count(ctree);

	if (follow_list_set[id].len == 0)
		return -1; //nothing follows this element

	*mis_tok = follow_list_set[id].fol[0];

	//return min;

	for (j =0; j < follow_list_set[id].len; ++j){

		init_tree(&tmp_tree);
		make_copy_of_toklist(ctree, tmp_tree);

		cur_tok = follow_list_set[id].fol[j];
		insert_tll_next_new(tmp_tree, pos, cur_tok);
		parser_core(tmp_tree);
		k = get_tll_count(tmp_tree);
		if (k < min){
			min = k;
			*mis_tok = cur_tok;
		}
	}

	return min;
}

void sehvler_yoxla(){

//	print_next_links();

	//init_prev_links_new();
	//init_satisfaction();

	struct tok_list *copy_tree;
	struct tok_list_el *tll = tree->first->next;
	int k;

	init_tree(&copy_tree);

	int mis_tok, pos = 0;

	printf("Missing tokens \n");

	int loop_max = 1000;

	while (tll->id != FILESON){

		make_copy_of_toklist(tree, copy_tree);
		k = get_best_missing_tok(copy_tree, pos, tll->id, &mis_tok);
		if (mis_tok == -1)
			goto cont_loop;

		print_tok(tll->id);
		printf(" : ");
		print_tok(mis_tok);
		printf(" : size is %d\n", k);
		//insert_tll_next_new(copy_tree, pos, mis_tok);
		//parser_core(copy_tree);
		//print_tree_dx(copy_tree);

cont_loop:
		tll = tll->next;
		pos++;	
		if (loop_max-- == 0)
			break;
	}
 
	printf("\n");
	
}