
#include "esrin.h"
#include "sehvler_3.h"

extern void print_after_test(struct tok_list_el *tll, char *msg);

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
	
		//if (is_terminal(tll->id)){
			if (!following(tll->id, tll->next->id)){
				//printf("(%d:%d)sehv:   %s -den sonra dogru ifade yazilmayib\n",  tll->tok->row, tll->tok->col, tll->tok->tok);				
				print_after_test(tll, " dogru ifade yazilmayib ");
			}
		//}

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

	printf("\n--------------------------------------------\n");

#if 0

	printf("Parsing agaci qurulmadi. Metn kodun yoxlayin ve proqrami yeniden kompilyasiya edin. \n");
	//printf("Eger metnin dogru tertib olunduguna eminsinizse onda zehmet olmasa kodu ahmed.sadikhov@gmail.com unvanina gonderin.\n");
	printf("\n--------------------------------------------\n");

#endif

  
 
}