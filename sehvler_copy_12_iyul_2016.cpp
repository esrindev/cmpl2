#include "esrin.h"
#include "rules_data_base.h"

// get index of rule_set from rule_token_set
//for given id
//extern build_cur_layer_hp_oprs(tree);
//extern build_cur_layer_lp_oprs(tree);

extern int parser_core();
extern void print_tok_str(int tok);


void insert_tll_next(struct tok_list_el *ptll, int id);

int math_oper(int id){

	switch(id){
		case MULT:
		case DEL:
		case NEGSIG:
		case POSSIG:
		case PRCT:
			return 1;
		default:
			return 0;	
	}

}

int math_oprnd(int id){

	switch(id){
		case NUMB:
		case IDT:
		case CRG1:
		case CRG2:
		case FLOAT:
		case EXPR:
		case STRMEM:
			return 1;
		default:return 0;	
	}

}

struct rule_token * get_rule_token(int id){

	int i, ind = -1;

	for (i=0; i<token_len; ++i)
		if (rule_token_set[i].id == id ){
			ind = i;
			break; 
		}

    // found rule set for given token
	if (ind >= 0)
		return &rule_token_set[ind];

	
	//no rule_set for given token
	return NULL;
}


void report_error_for_rule(struct tok_list_el *p_left, struct tok_list_el *p_right, int missing_left, int missing_right){

	if (p_right != NULL){
		printf("sehv(%d,%d):", p_right->tok->row, p_right->tok->col);
		printf(" %s -den sonra ", p_right->tok->tok);
		print_tok_str(missing_right);
		printf(" gozlenirdi. \n");
		return;
	}

	if (p_left != NULL){
		printf("sehv(%d,%d):", p_left->tok->row, p_left->tok->col);
		printf(" %s -den sonra ", p_left->tok->tok);
		print_tok_str(missing_left);
		printf(" gozlenirdi. \n");
		return;
	}

}

int get_id_pos(struct rule *rl, int id){

	int i;

	for (i=0; i<=rl->len; ++i)
		if (rl->left[i] == id)
			return  i;
	
	printf("Sehv: kompilyator xetasi: zehmet olmasa program metnin ahmed.sadikhov@gmail.com unvanina gonderin. get_id_pos -1 qaytardi\n");
	return -1; //error
}


void scan_backward(struct rule *rl, struct tok_list_el *tll, int pos,  int *match_cnt, int *missing_left,  struct tok_list_el **p_left){

	int i;

	for (i=pos-1; i >= 0; --i){
		tll = tll->prev;
		if (rl->left[i] != tll->id){
			*missing_left = rl->left[i];
			*p_left = tll;
			return;
		}
		(*match_cnt)++;
	}
}

void scan_forward(struct rule *rl, struct tok_list_el *tll, int pos, int *match_cnt,  int *missing_right, struct tok_list_el **p_right){

	int i;

	for (i=pos+1; i <=rl->len; ++i){
		tll = tll->next;
		if (rl->left[i] != tll->id){
			*missing_right = rl->left[i];
			*p_right = tll->prev;
			return;
		}
		(*match_cnt)++;
	}
}


int probe_for_rule(int rule_ind, struct tok_list_el *tll, int *match_cnt, int *missing_left, int *missing_right, struct tok_list_el **p_left, struct tok_list_el **p_right){

	struct rule *rl;	
	int i, cnt, pos;

	rl = &rule_set[rule_ind];

	//get pos of ID in given rule
	pos = get_id_pos(rl, tll->id);

	scan_backward(rl, tll, pos, match_cnt, missing_left, p_left);
	scan_forward(rl, tll, pos, match_cnt, missing_right, p_right);

	return pos;
}


void correct_math_errors(){

	struct tok_list_el *tll = tree->first->next;
	int id_next;

	//insert necessary tokens and restart parser

	while (tll->id != FILESON){
	
		if (math_oper(tll->id)){
		
			if (!math_oprnd(tll->prev->id))
				insert_tll_next(tll->prev, IDT);

			if (!math_oprnd(tll->next->id))
				insert_tll_next(tll, IDT);
		
		}

		tll = tll->next;
	
	}

}

void correct_ary_errors(){

	struct tok_list_el *tll = tree->first->next;
	int id_next;

	//insert necessary tokens and restart parser

	while (tll->id != FILESON){
	
		if (math_oper(tll->id)){
		
			if (!math_oprnd(tll->prev->id))
				insert_tll_next(tll->prev, IDT);

			if (!math_oprnd(tll->next->id))
				insert_tll_next(tll, IDT);
		
		}

		tll = tll->next;
	
	}

}

void check_errors(){

	struct rule_token *rtn;
	int i,max, match_cnt, found_rule, pos;
	int missing_left, missing_right, found_missing_left, found_missing_right;
	struct tok_list_el *p_left, *p_right, *found_p_left, *found_p_right;

	int max_restart_cnt = 0;

	printf("\n----------------------------------------------\n\n");
	printf("----------------  ERRORS BY RULE ------------------\n\n");
   

   struct tok_list_el *tll = tree->first->next;

   while (tll->id != FILESON){

   	   rtn = get_rule_token(tll->id);

	   // no rule_set for given token
	   if (rtn == NULL){
		//fixup
		   printf("sehv:setir %d gozlenilmeyen hedd", tll->tok->row);
		   tll = tll->next;
		   continue;
	   }

	   max = 0;
	   found_rule = 0;
	   found_p_left = found_p_right = NULL;
	   p_left = NULL;
	   p_right = NULL;

	   // probe for best matching syntax rule for given token
	   for (i=0; i<rtn->len; ++i){
		   match_cnt = 0;
		   //printf("rule %d for token ", rtn->rules[i]);
		  // print_tok(tll->id);
		  // printf("\n");
		  // print_rule_id(rtn->rules[i]);
		   pos = probe_for_rule(rtn->rules[i], tll, &match_cnt, &missing_left, &missing_right, &p_left, &p_right);
		   if (match_cnt > max){
			   max = match_cnt;
			   found_rule = i;
			   found_missing_left = missing_left;
			   found_missing_right = missing_right;
			   found_p_left = p_left;
			   found_p_right = p_right;
		   }
	   }

	   printf("\n Best matching rule for token ");
	   print_tok(tll->id);
	   printf(" is \n\n");
	   print_rule_id(rtn->rules[found_rule]);
	   printf("end of best match\n\n");
	   
	   // report error for best matched syntax rule
	   report_error_for_rule(p_left, p_right, missing_left, missing_right);
		
		//tll has to updated inside report_error
		//otherwise we may encounter endless loop 

	   //tll = tll->next;

	   if (p_right != NULL){
			tll = p_right->next;

		/*

		   printf("\nparse_tree before insert---------------------------\n");
		   print_tok_list(tree);

		   if (found_p_left != NULL)
			insert_tll_next(found_p_left, missing_left);
		   if (found_p_right != NULL)
			insert_tll_next(found_p_right, missing_right);

		   printf("\nparse_tree after insert---------------------------\n");
		   print_tok_list(tree);

		   //restating parser
		   parser_core();
		   printf("\nparse tree after restart ==============================\n");
		   print_tree_dx(tree); 
		   if ( ++max_restart_cnt > 3)
			   return;
			   */

	   }
	   else{
		   for (i=pos+1; i <= rule_set[rtn->rules[found_rule]].len; ++i)
			tll = tll->next;
	   }

   }


   //find_subtractions();

}

int get_next_tok(int id){

	int i;

	for (i=0; i<err_msg_len; ++i )
		if ( rl_msg_set[i].id == id )
			return  rl_msg_set[i].next_id;

	return -1;
}

//similar of this func defined in parse.cpp
void insert_tll_next(struct tok_list_el *ptll, int id){

	struct tok_list_el *tll, *tmp;

	tll = create_tok_list_el(ptll->tok);

	tll->id = id;

	//now insert
	tll->next = ptll->next;
	ptll->next->prev = tll;
	tll->prev = ptll;
	ptll->next = tll;

}


void check_errors_new(){

	struct tok_list_el *tll = tree->first->next;
	int id_next;

	//insert necessary tokens and restart parser

	correct_math_errors();

/*    build_cur_layer_hp_oprs(tree);
	build_cur_layer_lp_oprs(tree);
*/
    correct_ary_errors();

}

