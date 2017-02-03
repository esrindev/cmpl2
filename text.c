

  #include "buta.h"
  
 
int f_text(int *st){

  int tid = next_tok();

  if (tid < 0)
    return 0;

  printf("st = %d, tid = %d\n",*st,tid);

  switch(*st){
  case ST0:  
    if ( tid == ID_PRG){
      *st = ST1; return 1;}
    else{
      *st = ST0E; return 0;}

  case ST1:
    if ( tid == ID_VAR){
      *st = ST2; return 1;}

   if ( tid == ID_KOD){
      *st = ST3; return 1;}   

   *st = ST1E; return 0;  

  case ST2:  
    if ( tid == ID_COLON){
      *st = ST_VAR;
    /* this is also starting of M_var */
      M_var(st);
      return 1;}

    *st = ST2E; return 0;   


  case ST_KOD: 
     /* transitioning to  M_kod   */
      M_kod(st);
      return 1;
  
  case M_VAR_GOOD:
    
    if ( tid == ID_COLON){
      *st = ST_KOD;
     /* transitioning to  M_kod   */
      M_kod(st);
      return 1;}

    *st = ST3E; return 0;  

  case M_VAR_BAD:
    return 0;     

  case M_KOD_GOOD:
    if (tid == ID_DOT)
      *st = ST9;
    return 1;

    *st = ST5E;
    return 0;

  case M_KOD_BAD:
    return 0; 

   }
}

/* Deterministic Finit State Automata to lexic and syntax
analyse of program text  */
void M_text(int *st){

  int i,m;

  while(1){
    m = f_text(st);
    if (m == 0) 
      break;
  }
}

/* check result of M_text DFA  */
void M_text_chk(int st){


  printf("text check st = %d\n",st);

  switch(st){
  case ST0E:
  case ST0:
    printf("compilyasiya sehvi: proqram sozu gozlenilirdi\n");
    break;
  case ST1E:

    printf("compilyasiya sehvi: var ve ya kod  sozu gozlenilirdi\n");
    break;
  case ST2E:
    printf("var sozunden sonra  : isharesi gozlenilirdi\n");
    break;
  case ST3E:
  case M_VAR_GOOD:
   printf("kod sozunden sonra  : isharesi gozlenilirdi\n");
    break;
  case ST5E:
    printf(" . isharesi gozlenilirdi\n");
  case ST5:
   printf(" Proqramin kod hissesi ugurla kompilyasiya olundu\n");
    break;
  case ST6:
    printf("Proqramin kod hisssinde sehv, sehvleri duzeldin ve proqrami yeniden icra edin\n");
    break;
  case ST7:
   printf(" Proqramin deyishenler hissesi ugurla kompilyasiya olundu\n");
    break;
  case ST8:
    printf("Proqramin deyishenler hisssinde sehv, sehvleri duzeldin ve proqrami yeniden icra edin\n");
    break;
  case ST9:
    printf("Proqramin klompilyasiyasi ugurla tamamlandi\n");
    break;
  case ST_VAR:
    printf("kod sozu gozlenilirdi\n");
    break;
  case M_KOD_BAD:
    printf("Proqramin kod hissesinde sehvleri duzeldin ve proqrami yeniden kompilyasiya edin\n");
    break;
  case M_KOD_GOOD:
    printf("proqramin sonunda . isharesi gelmelidir\n");
    break;
  default:
    printf(" taninmayan xeta\n");
  }
}

void analyse(){

  int st = ST0;

  /* run finit state machine */
  M_text(&st);

  /* machine runned and produced output, now check it  */
  M_text_chk(st);

}

  

