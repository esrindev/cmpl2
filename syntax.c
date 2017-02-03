 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <unistd.h>
 #include <fcntl.h>

#include "buta.h"



int check_structure(){

  if (cnt == 0)
		exit(0);

  if (prg_start == -1 )
	{
	  printf("Proqram bashlama setri duz verilmeyib  \n" );
	  exit(0);
	}


  if ((first_nmp_ln!=-1) && (first_nmp_ln < prg_start))
	{
          printf("setir %d : \"%s\" setrinden evvel, \n \"%s\"  setri gelir\n ",first_nmp_ln, metn[prg_start].s,metn[first_nmp_ln].s);
	  exit(0);
	}


  if (code_start == -1 )
	{
	  printf("icraya bashlama setri duz verilmeyib \n ");
	  exit(0);
	}


  if ((data_start != -1) && (data_start > code_start)){
	   printf("verilenler setri icra setrinden evvel gelmelidir\n");
	   exit(0);
	}



 if (prg_end == -1 )
	{
	  printf("Proqram bitme setri duz verilmeyib \n ");
	  exit(0);
	}
  


  if ((last_nmp_ln!=-1) && (last_nmp_ln > prg_end))
	{
          printf("setir %d : \"%s\" setrinden sonra, \n \"%s\"  setri gelir\n ",last_nmp_ln, metn[prg_end].s,metn[last_nmp_ln].s);
	  exit(0);
	}




}


int check_data_line(int i){

	struct setir *ln = &metn[i];

	/* last non spacve word   lnw  */
	int lnw = ln->k-1;
        char *p;
	int k ;

	p = ln->els[lnw];
	k = strlen(p);

	/*
	printf("son soz \"%s\", son simvol \'%c\', son swoz nomre %d\n",ln->els[lnw],*(p+k-1),lnw);
	*/

	/* checking end of line*/
	if ( *(p+k-1) != '.'){
		printf("setir %d: \"%s\"\n\telan setrinin sonu \'.\' simvolu ile bitmeyib\n",i,ln->s);
		int j;
		return 1;
	}
	else
		remove_last_dot(ln);


	/* checking tip identifier  */
	if ((strcmp(ln->els[0],"tam")!=0)&&(strcmp(ln->els[0],"kesr")!=0)&&(strcmp(ln->els[0],"simvol")!=0)){
		printf("setir %d: \'%s\' dogru tip deyil\n",i,ln->els[0]);
		return 1;
	}
	
		

	return 0;
}

void check_data_decl(){

	int b,e,i;
	
	b = data_start;
	e = code_start;

	int k=0;

	for (i=b+1; i<e; ++i)
		if (!metn[i].emp)
			k += check_data_line(i);

	if (k==0)
		printf("deyishenlerin elani ugurla yoxlanildi\n");
	else{
		printf("deyishenlerin elaninda %d sehv var. ", k);
		printf("Sehvleri duzeldin ve proqrami yeniden kompilyasiya edin\n");	
		exit(0);	
	}

}

int is_char(char c){

  if ( ((c>='a') && (c<='z')) || ((c>='A')&&(c<='Z')))
  return 1;

return 0;

}


int is_digit(char c){

  if ((c>='0') && (c<='9'))
  return 1;

return 0;

}


int yanlish_ad(char *s){

	char c, *p;
	int  i, k;

	p = s;
	k = strlen(s);

	/* check begining of name */
	if (!is_char(*p))
		return 1;

	for (i=1; i<k; ++i){
		c = *(p+i);
		if ((c!='_')&&(!is_digit(c))&&(!is_char(c)))
			return 1;
	}

	return 0;
}


int check_names(int num){

	struct setir *ln = &metn[num];
	int k= ln->k;
	int i;
	int say = 0;

	int bad_vars[1000]={0};

	for (i=1; i<k; ++i)
		if (yanlish_ad(ln->els[i])){
			bad_vars[i]=1;
			say++;
		}

	if (say){
		for (i=1; i<k; ++i)
			if (bad_vars[i])
				printf("setir %d: sehv deyishen adi \'%s\' duzgun deyil\n",num,ln->els[i]);
		return say;
	}

	return 0;
}


void check_var_names(){

	int b,e,i;
	
	b = data_start;
	e = code_start;

	int k=0;

	for (i=b+1; i<e; ++i)
		if (!metn[i].emp)
			k += check_names(i);

	if (k==0)
		printf("deyishenlerin adlari  ugurla yoxlanildi\n");
	else{
		printf("deyishenlerin adlandirilmasinda  %d sehv var. ", k);
		printf("Sehvleri duzeldin ve proqrami yeniden kompilyasiya edin\n");		
		exit(0);
	}

}





int check_code_line(int i){

	int k=0;

	
	

	

}

void check_code_lines(int b, int e){

	int i;

 
	printf("checking code lines\n");

	for (i=b+1; i<e; ++i)
		if (!metn[i].emp)
			check_code_line(i);

}

void check_code(){

	int b,e,i;
	
	b = code_start;
	e = prg_end;

	check_code_lines(b,e);

}

void check_syntax(){

	check_structure();
	/* struktur ok! */

	check_data_decl();
	/* data decl ok! */

	check_var_names();
	/* var names ok! */

	check_code();

}
