/* Lisence GPL V3
   Ahmed Sadikhov
   ahmed.sadikhov@gmail.com
   program to build symbol tables

   second edition : 06 feb 2017
   */

/*
	TYPES

	0 - VOID - tipsiz
	1 - TAM  - tam
	2 - KESR - kesr
	3 - SIMVOL - simvol
	4 - SETIR - setir
*/

#include "esrin.h"
#include "sehvler.h"

struct fcall_table_el {
	struct token *htok;
	struct usg_table at; // arg table
} ;

struct fcall_table{
	struct fcall_table_el fcalls[MAX_FCALL];
	int count;
} ;


struct scope{
	int id;
	int par_id;
	int cld_cnt;
	struct scope *childs[MAX_SCOPE_CHILD];
	struct scope *par_scp;
	struct tok_list_el *tll;
	struct dec_table dt;
	struct dec_table pt;
	struct usg_table ut;
	struct fcall_table fct;
} glb_scp_cnt, *glb_scp;



struct func_table_el_new {
	struct token *htok; // head tok of funk name
	struct scope *scp;
	struct dec_table *pt; // parameters, we must calc types ourselves
	int ret_val_tip; 
	int id; //unique ID for function
};


struct func_table_new{
	struct func_table_el_new  func_table[MAX_FUNC];
	int count;
} func_table_new_cont;




//set initial values to global scope params
void init_glb_scp_ptr(){
	glb_scp = &glb_scp_cnt;
	glb_scp->id = 0;
	glb_scp->par_id = 0;
	glb_scp->par_scp = glb_scp;
	glb_scp->cld_cnt = 0;
	glb_scp->dt.count = 0;
	glb_scp->pt.count = 0;
	glb_scp->ut.count = 0;
	glb_scp->fct.count = 0;
	glb_scp->tll = tree->first->next;
}

struct scope *add_scp_cld(struct scope *scp_hd, struct tok_list_el *tll, int *id ){

	struct scope *scp_cld;

	//update unical id per scope
	(*id)++;

	//child related data
	scp_cld = (struct scope *)malloc(sizeof(struct scope));
	scp_cld->cld_cnt = 0;
	scp_cld->tll = tll;
	scp_cld->id = *id;
	scp_cld->par_id = scp_hd->id;
	scp_cld->par_scp = scp_hd;
	scp_cld->dt.count = 0;
	scp_cld->pt.count = 0;
	scp_cld->ut.count = 0;
	scp_cld->fct.count = 0;

	//parent related data
	scp_hd->childs[scp_hd->cld_cnt] = scp_cld;
	scp_hd->cld_cnt++;

	return scp_cld;
}

void build_scp_tree(struct scope *scp_hd, struct tok_list_el *tll, int *id){

	struct scope *scp_cld;
	struct tok_list_el *tll_cld;
	int i;

	for (i=0; i<tll->cld_cnt; ++i){

		tll_cld = tll->childs[i];

		if (tll_cld->id == FUNK ){
			scp_cld = add_scp_cld(scp_hd, tll_cld, id);
			build_scp_tree(scp_cld, tll_cld, id);
			//tll->childs[i] = NULL; // remove reference to FUNK 
			continue;
		}

		if (tll_cld->id == BLOCK && tll->childs[0]->id != FUNKSIYA ){
			scp_cld = add_scp_cld(scp_hd, tll_cld, id);
			build_scp_tree(scp_cld, tll_cld, id);
			//tll->childs[i] = NULL; // remove reference to BLOCK
			continue;
		}

	build_scp_tree(scp_hd, tll_cld, id);
	}
}

void build_scope(){

	int id = 0;

	printf("Building symbol tables \n");

	init_glb_scp_ptr();
	build_scp_tree(glb_scp, tree->first->next, &id);

}

void print_tip(int tip){

	printf("tip ");

	switch(tip){
		case 0:
			printf("tipsiz");
			return;
		case 1:
			printf("tam");
			return;
		case 2:
			printf("kesr");
			return;
		case 3:
			printf("simvol");
			return;
	}
}

void print_dec_list(struct dec_table *dt){

	struct var *vr;
	int i;

	for (i=0; i<dt->count; ++i){
		vr = &dt->decs[i];
		printf("\t");
		print_tok(vr->as);
		printf("\t %s \t d1 %d \t d2 %d \t ofst %d\t ", vr->tok->tok, vr->d1, vr->d2, vr->tok->ofst);
		print_tip(vr->tip);
		printf("\n");	
	}
}

void print_usg_list(struct usg_table *ut){

	struct usg_list_el *ul;
	int i;

	for (i=0; i<ut->count; ++i){
		ul = &ut->usgs[i];
		printf("\t");
		print_tok(ul->as);
		printf("\t %s \t r %d \t c %d \t", ul->tok->tok, ul->tok->row, ul->tok->col);
		printf("\n");	
	}
}

void print_scp_tree(struct scope *scp){

	int i;

	printf("id %d, pid %d \n", scp->id, scp->par_id);

	printf("Declared variables\n");
	print_dec_list(&scp->dt);

	printf("Used variables \n");
	print_usg_list(&scp->ut);

	printf("childs:\n");

	for (i=0; i<scp->cld_cnt; ++i)
		print_scp_tree(scp->childs[i]);
}


void print_scope(){

	printf("Scope tree\n");
	print_scp_tree(glb_scp);
}


int get_tip(struct tok_list_el *tll){

	switch (tll->id){
	case TAM:
		return 1;
	case KESR:
		return 2;
	case SIMVOL:
		return 3;
	default:
		printf("Sehv: yanlish tip.\n");	
		return 0;
	}
}

void add_dec_list_idt(struct scope *scp, struct tok_list_el *tll, int tip){

	struct dec_table *dt;
	struct token *tok;
	struct var *vr;

	dt = &scp->dt;
	vr = &dt->decs[dt->count];

	vr->tll = tll;
	vr->tok = tll->tok;
	vr->tip = tip;
	vr->as = IDT;
	vr->d1 = 1;
	vr->d2 = 1;

	//update tok's fields as well
	tok = vr->tok;
	tok->d1 = vr->d1;
	tok->d2 = vr->d2;
	tok->tip = vr->tip;
	
	dt->count++;
}


void add_dec_list_crg1(struct scope *scp, struct tok_list_el *tll, int tip){

	struct tok_list_el *tll1, *tll2;
	struct dec_table *dt;
	struct token *tok;
	struct var *vr;

	dt = &scp->dt;
	vr = &dt->decs[dt->count];

	tll1 = tll->childs[0];
	tll2 = tll->childs[1];

	if (tll2->childs[1]->id != NUMB){
		printf("Sehv: eded golenirdi\n");
		return;	
	}

	vr->tll = tll;
	vr->tok = tll1->tok;
	vr->tip = tip;
	vr->as = CRG1;
	vr->d1 = atoi(tll2->childs[1]->tok->tok);
	vr->d2 = 1;

	//update tok's fields as well
	tok = vr->tok;
	tok->d1 = vr->d1;
	tok->d2 = vr->d2;
	tok->tip = vr->tip;
	
	dt->count++;
}

//see parsing rule for CRG2
void add_dec_list_crg2(struct scope *scp, struct tok_list_el *tll, int tip){

	struct tok_list_el *tll1, *tll2;
	struct dec_table *dt;
	struct token *tok;
	struct var *vr;

	dt = &scp->dt;
	vr = &dt->decs[dt->count];

	tll1 = tll->childs[0];
	tll2 = tll->childs[1];

	if (tll1->childs[1]->childs[1]->id != NUMB ){
		printf("Sehv: eded gozlenirdi\n");
		return;	
	}

	if (tll2->childs[1]->id != NUMB ){
		printf("Sehv: eded gozlenirdi\n");
		return;	
	}

	vr->tll = tll;
	vr->tok = tll1->childs[0]->tok;
	vr->tip = tip;
	vr->as = CRG2;
	vr->d1 = atoi(tll1->childs[1]->childs[1]->tok->tok);
	vr->d2 = atoi(tll2->childs[1]->tok->tok);

	//update tok's fields as well
	tok = vr->tok;
	tok->d1 = vr->d1;
	tok->d2 = vr->d2;
	tok->tip = vr->tip;
	
	dt->count++;
}

void add_dec_list_el(struct scope *scp, struct tok_list_el *tll, int tip){

	if (tll->id == IDT){
		add_dec_list_idt(scp, tll, tip);
		return;	
	}

	if (tll->id == CRG1){
		add_dec_list_crg1(scp, tll, tip);
		return;	
	}

	if (tll->id == CRG2){
		add_dec_list_crg2(scp, tll, tip);
		return;	
	}
	
	if (tll->id != VERGUL && tll->id != SEQ){
		printf("Sehv: xx -den sonra gozlenilmeyen ifade.\n");
		return;	
	}
}

void build_dec_elems(struct scope *scp, struct tok_list_el *tll, int tip){

	int i;

	if (tll->id == IDT || tll->id == CRG1  || tll->id == CRG2 ){
		add_dec_list_el(scp, tll, tip);
		return;
	}

	for (i=0; i<tll->cld_cnt; ++i)
		build_dec_elems(scp, tll->childs[i], tip);	
	
}

void build_dec_list(struct scope *scp, struct tok_list_el *tll){

	int i, tip;

	for (i=0; i<tll->cld_cnt; ++i){

		if ( tll->childs[i]->id == BLOCK )
			continue;

		if ( tll->childs[i]->id == FUNK )
			continue;

		if ( tll->childs[i]->id == FCALL1 && i == 1 ){
			 /* the only possible case for this condition  is rule r1
			   here we add func params and funks body's declared variables to same scope
			 */

			struct tok_list_el *tll_dec;
			tll_dec = tll->childs[i]->childs[1];

			//according to parsing tll_dec is either BRK_VAL or BRK_SEQ
			tip = 0; // types of func params calculated later by usage, set default to VOID
			build_dec_elems(scp, tll_dec, tip);


			//body of function (i.e. BLOCK) is also belongs to the same scope of
			//FUNK so add them also
			// we now that tll has 3 childs and child[2] is funks BLOCK accrd to r1
			build_dec_list(scp, tll->childs[2]);
			continue;		
		}

		if ( tll->childs[i]->id == VDEC ){
			tip = get_tip(tll->childs[i]->childs[0]);
			build_dec_elems(scp, tll->childs[i]->childs[1], tip);
			continue;		
		}



		build_dec_list(scp, tll->childs[i]);
	}
}



void build_dec_table(struct scope *scp){

	int i;

	build_dec_list(scp, scp->tll);

	for (i=0; i<scp->cld_cnt; ++i)
		build_dec_table(scp->childs[i]);
}


void add_usg_idt(struct usg_table *ut, struct tok_list_el *tll){

	struct usg_list_el *ul;

	ul = &ut->usgs[ut->count];

	ul->tok = tll->tok;
	ul->as = IDT;
	
	ut->count++;
}

void add_usg_crg1(struct usg_table *ut, struct tok_list_el *tll){

	struct usg_list_el *ul;

	ul = &ut->usgs[ut->count];

	ul->tok = tll->childs[0]->tok;
	ul->as = CRG1;
	
	ut->count++;
}

void add_usg_crg2(struct usg_table *ut, struct tok_list_el *tll){

	struct usg_list_el *ul;

	ul = &ut->usgs[ut->count];

	ul->tok = tll->childs[0]->childs[0]->tok;
	ul->as = CRG2;
	
	ut->count++;
}

void add_usg_fcall1(struct usg_table *ut, struct tok_list_el *tll){

	struct usg_list_el *ul;

	ul = &ut->usgs[ut->count];

	ul->tok = tll->childs[0]->tok;
	ul->as = FCALL1;
	
	ut->count++;
}

void build_usg_list(struct scope *scp, struct tok_list_el *tll){

	int i;

	for (i=0; i<tll->cld_cnt; ++i){

		if ( tll->childs[i]->id == VDEC ){
			/* skip declaration  */				
			continue;		
		}

		if ( tll->childs[i]->id == BLOCK ){
			continue;
		}	

		if ( tll->childs[i]->id == FUNK ){
			//build_usg_list(scp, tll->childs[i]->childs[2]); //BLOCK of FUNK
			continue;
		}		

		if ( tll->childs[i]->id == FCALL1 && i == 1 ){
			 /* skip FCALL1 at FUNK declaration rule r1
			 */
			continue;
		}

		if ( tll->childs[i]->id == IDT ){
			add_usg_idt(&scp->ut, tll->childs[i]);
			continue;	
		}

		if ( tll->childs[i]->id == CRG1 ){
			add_usg_crg1(&scp->ut, tll->childs[i]);
			build_usg_list(scp, tll->childs[i]->childs[1]);
			continue;	
		}

		if ( tll->childs[i]->id == CRG2 ){
			add_usg_crg2(&scp->ut, tll->childs[i]);
			build_usg_list(scp, tll->childs[i]->childs[0]->childs[1]);
			build_usg_list(scp, tll->childs[i]->childs[1]);
			continue;	
		}

		if ( tll->childs[i]->id == FCALL1 && i != 1){
			add_usg_fcall1(&scp->ut, tll->childs[i]);
			build_usg_list(scp, tll->childs[i]->childs[1]);
			continue;	
		}

		build_usg_list(scp, tll->childs[i]);
	}
}


void build_usg_table(struct scope *scp){

	int i;

	build_usg_list(scp, scp->tll);

	for (i=0; i<scp->cld_cnt; ++i)
		build_usg_table(scp->childs[i]);

}



void build_params_table(struct scope *scp, struct tok_list_el *func_tll, struct func_table_new *ft){

	struct tok_list_el *fcall1_tll;
	struct dec_table *pt, *dt;
	struct var *dl, *pl;
	int i; 

	//init funck name IDT token
	fcall1_tll = func_tll->childs[1];
	ft->func_table[ft->count].htok = fcall1_tll->childs[0]->tok;

	//scope dec table  = func params + func body dec elemes
	dt = &scp->dt;
	pt = &scp->pt;

	//func params tip set to void by build_dec_table
	for (i=0; i<dt->count; ++i){
		if (dt->decs[i].tip != 0 )
			break;
		pl = &pt->decs[pt->count];
		dl = &dt->decs[i];
		*pl = *dl;
		pt->count++;
	}

	ft->func_table[ft->count].pt = pt;
}



void calc_ret_tip(struct tok_list_el *tll, struct func_table_new *ft){


}


struct dec_table *get_paramts_table(unsigned long hid){

	struct func_table_new *ft;
	int i;

	ft = &func_table_new_cont;

	for (i=0; i<ft->count; ++i)
		if (ft->func_table[i].htok->hid == hid)
			return ft->func_table[i].pt;
}

void build_func_table(struct scope *scp, struct func_table_new *ft){

	struct tok_list_el *tll;
	int i;

	ft->count = 0;

	for (i=0; i<scp->cld_cnt; ++i){
		tll = scp->childs[i]->tll;
		if (tll->id == FUNK){
			build_params_table(scp->childs[i], tll, ft);
			calc_ret_tip(tll, ft);
			ft->count++;
		}
	}
}

void print_func_table(struct func_table_new *ft){

	struct func_table_el_new *fl;
	int i;

	printf(" Funcs table\n\n");

	for (i=0; i<ft->count; ++i){
		fl = &ft->func_table[i];
		printf("\t %s \n", fl->htok->tok);
		printf("Func params\n");
		print_dec_list(fl->pt);
	}

}

static int get_tip_size(int tip){

	switch (tip){
	case 1: /* int tip, we use long int  */
		return sizeof(int);
	case 2:
		return sizeof(float);
	case 3:
		return sizeof(char);
    /* calculating of struct tip sizes to be implemented ...  */
	default:
		printf("Desteklenmeyen tip: sistem xetasi\n");
		return -1;
	}

}

void set_var_ofst(struct dec_table *dt){

	struct token *tok;
	int i,d1,d2,tipsz;

	dt->size = 0;

	for (i=0; i<dt->count; ++i){
		tok = dt->decs[i].tok;
		d1 = dt->decs[i].d1;
		d2 = dt->decs[i].d2;
		tipsz = get_tip_size(dt->decs[i].tip);
		dt->size += tipsz*d1*d2;
		tok->ofst = dt->size;
	}
}


struct var *get_dec_el(struct token *tok, struct scope *scp){

	struct dec_table *dt;
	unsigned long hid = tok->hid;
	int i;

	dt = &scp->dt;

	for (i=0; i<dt->count; ++i)
		if (dt->decs[i].tok->hid == hid)
			return &dt->decs[i];

	if (scp->par_scp != scp)// true if not glb_scp
		return get_dec_el(tok, scp->par_scp);

	//variable not found, error
	return NULL;
}

void set_usg_ofst(struct usg_table *ut, struct scope *scp){

	struct usg_list_el *ul;
	struct var *dl;
	int i;

	for (i=0; i<ut->count; ++i){
		ul = &ut->usgs[i];
		dl = get_dec_el(ul->tok, scp);
		if (dl != NULL){
			ul->tok->ofst = dl->tok->ofst; //get ofst from declaration
			ul->tok->tip = dl->tok->tip;
		}
		else{
			printf("(%d:%d): Sehv %s deyisheni elan olunmayib\n", ul->tok->row, ul->tok->col, ul->tok->tok);
		}
	}
}


void set_scope_decs_ofst(struct scope *scp){

	int i;

	set_var_ofst(&scp->dt);

	for (i=0; i<scp->cld_cnt; ++i)
		set_scope_decs_ofst(scp->childs[i]);

}


void set_scope_usgs_ofst(struct scope *scp){

	int i;

	set_usg_ofst(&scp->ut, scp);

	for (i=0; i<scp->cld_cnt; ++i)
		set_scope_decs_ofst(scp->childs[i]);

}

void symtab_new(){

	build_scope();
	build_dec_table(glb_scp);
	build_usg_table(glb_scp);
	build_func_table(glb_scp, &func_table_new_cont);

	//set offset to variables
	//ofst filed is used on calculation of var adrs
	set_scope_decs_ofst(glb_scp);

	set_scope_usgs_ofst(glb_scp);

	print_scope();
	print_func_table(&func_table_new_cont);

}