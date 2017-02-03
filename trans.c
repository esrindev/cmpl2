
#include "esrin.h"

/* Lisence GPL V3   
   program to translate text from esrin to cpp
   Ahmed Sadikhov 2014.22.oct
   ahmed.sadikhov@gmail.com
*/

void terc_simvol(struct node *nd){

  struct se *se;
  int i,k,id;
  
  se = &nd->se;
  k = se->end - se->start;

  for (i=se->start; i<=se->end; ++i){
    id = tokens[i].id;

    switch(id){
    case SIMVOL:
      fprintf(fp, " \n char  "); break;
    case COLON:
      break;
    case NOQTEV:
      fprintf(fp, " ; \n  "); break;      
    default:
      fprintf(fp, " %s ", tokens[i].tok);
      break;      
    }
  }

} 

void terc_kesr(struct node *nd){

  struct se *se;
  int i,k,id;
  
  se = &nd->se;
  k = se->end - se->start;

  for (i=se->start; i<=se->end; ++i){
    id = tokens[i].id;

    switch(id){
    case KESR:
      fprintf(fp, " \n double  "); break;
    case COLON:
      break;
    case NOQTEV:
      fprintf(fp, " ; \n  "); break;      
    default:
      fprintf(fp, " %s ", tokens[i].tok);
      break;      
    }
  }

} 



void terc_tam(struct node *nd){

  struct se *se;
  int i,k,id;
  
  se = &nd->se;
  k = se->end - se->start;

  for (i=se->start; i<=se->end; ++i){
    id = tokens[i].id;

    switch(id){
    case TAM:
      fprintf(fp, " \n int  "); break;
    case COLON:
      break;
    case NOQTEV:
      fprintf(fp, " ; \n  "); break;      
    default:
      fprintf(fp, " %s ", tokens[i].tok);
      break;      
    }
  }

} 

void terc_asgn(struct node *nd){
  /* hele ki ASGN operatrunun sinxronizasiyasi heyata kecirilmeyib
     qismete tercume olunur. Duzgunluyune emin deyilem. 
     Yoxlamali!!!   */  

  struct se *se;
  int i,k,id;
  
  se = &nd->se;
  k = se->end - se->start;

  fprintf(fp, " \n ");

  for (i=se->start; i<=se->end; ++i){
    id = tokens[i].id;

    switch(id){

    default:
      fprintf(fp, " %s ", tokens[i].tok);
      break;      
    }
  }

} 

void terc_daxil(struct node *nd){
  
  struct se *se;
  int i,k,id;
  
  se = &nd->se;
  k = se->end - se->start;

  for (i=se->start; i<=se->end; ++i){
    id = tokens[i].id;

    switch(id){
    case DAXIL:
      fprintf(fp, " \n std::cin >> ");      
      break;
    case VERGUL:
      fprintf(fp, "  >> ");      
      break;      
    case OPNBRK:
    case CLSBRK:
      break;      
    default:
      fprintf(fp, " %s ", tokens[i].tok);
      break;      
    }
  }

} 


void terc_eger(struct node *nd){
 
  struct se *se;
  int i,k,id;
  
  se = &nd->se;
  k = se->end - se->start;


  for (i=se->start; i<=se->end; ++i){
    id = tokens[i].id;

    switch(id){
    case EGER:
      fprintf(fp, " \n if ");      
      break;
    case YOXSA:
      fprintf(fp, "  else ");      
      break;     
    case VE:
      fprintf(fp, " && ");
      break;
   case VEYA:
      fprintf(fp, " || ");
      break;
   case ONDA:
     break;
   default:
      fprintf(fp, " %s ", tokens[i].tok);
      break;      
    }
  }

}

void terc_capet(struct node *nd){
 
  struct se *se;
  int i,k,id;
  
  se = &nd->se;
  k = se->end - se->start;

  for (i=se->start; i<=se->end; ++i){
    id = tokens[i].id;

    switch(id){
    case CAPET:
      fprintf(fp, " \n std::cout << ");      
      break;
    case VERGUL:
      fprintf(fp, "  << ");      
      break;      
    case OPNBRK:
    case CLSBRK:
      break;      
    case NOQTEV:
      fprintf(fp, " %s\n ", tokens[i].tok);
      break;        
    default:
      fprintf(fp, " %s ", tokens[i].tok);
      break;      
    }
  }

} 

void print_se(struct se se){

  int i;

  for(i=se.start; i<=se.end; ++i)
    fprintf(fp, "%s", tokens[i].tok);
 
}

void init_saygac(struct se *syg, struct se *sygb, struct se *syge, int *dir, struct node *nd){

  int i,j,m,n,k,p;
  
  n = nd->se.end;
  for (i=nd->se.start; i<=n; ++i)
    if (tokens[i].id == OPNBRK)
      break;
  syg->start = i+1;
  for (j=i+1; j<=n; ++j)
    if (tokens[j].id == ARTSIN || tokens[j].id == AZALSIN)
      break;
  if (tokens[j].id == ARTSIN)
    *dir = 1;
  else
    *dir = 0;
  syg->end = j-1;
  sygb->start = j+1;
  for (i=j+1; i<=n; ++i)
    if (tokens[i].id == DEN || tokens[i].id == DAN)
      break;
  sygb->end = i-2;
  syge->start = i+1;
  for (j=i+1; j<=n; ++j)
    if (tokens[j].id == KIMI)
      break;
  syge->end = j-3;
  

}

void terc_dovr(struct node *nd){

  struct se  syg, sygb, syge;
  int dir;

  init_saygac(&syg, &sygb, &syge, &dir, nd);

  fprintf(fp, "\n for ( ");
  print_se(syg);
  fprintf(fp, " = ");
  print_se(sygb);
  fprintf(fp, "; ");
  print_se(syg);
  if (dir)
    fprintf(fp, " <= ");
  else
    fprintf(fp, " => ");
  print_se(syge);
  fprintf(fp, "; ");
  print_se(syg);
  if (dir)
    fprintf(fp, "++ )");
  else
    fprintf(fp, "-- )");
}

void translate_to_cpp(void){

  struct node *nd;
  int i,id;

  for (i=0; i<ndc; ++i){
    nd = nodes[i];
    id = nd->id;
    switch(id){
    case ASGN:    
      terc_asgn(nd);   break;
    case DAXIL:    
      terc_daxil(nd);  break;
    case CAPET:    
      terc_capet(nd);  break;
    case DOVR:
      terc_dovr(nd);   break;
    case EGER:
    case YOXSA:
      terc_eger(nd);   break;
    case TAM:
      terc_tam(nd);     break;
    case KESR:
      terc_kesr(nd);     break;
    case SIMVOL:
      terc_simvol(nd);     break;
    case OPNBLK:
      fprintf(fp, " \n { \n"); break;
    case CLSBLK:
      fprintf(fp, " \n } \n"); break;      
      break;
    case SON:
      fprintf(fp, " \n } \n"); break;      
      break;
    default:    
      ;
    }
    
  }



}
