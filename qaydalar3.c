

start -> prg2
start -> yeni_tip    prg2
start -> sinif_type  prg2
start -> strukt2     prg2

 PRG2 -> PRG1
 PRG2 -> PRG1  FUNK
 PRG2 -> PRG1  FUNKS

  PRG1 -> PRG OPNBLK VAR1 KOD1 CLSBLK
  PRG1 -> PRG OPNBLK KOD1 CLSBLK	

   KOD1 -> KOD  COLON
   KOD1	-> KOD  COLON  KODDATA

*	KODDATA	-> OPNBLK  KODDATA  CLSBLK
	KODDATA	-> (!YOXSA)  KODDATA  KODDATA
	KODDATA	-> EGER_EXP2  KODDATA  YOXSA  KODDATA
	KODDATA	-> NEQEDERKI2  KODDATA
	KODDATA	-> EGER_EXP2  KODDATA  (!YOXSA)
	KODDATA	-> SEC1  IDT  CLSBRK  OPNBLK  HAL1  CLSBLK
	KODDATA	-> SEC1  IDT  CLSBRK  OPNBLK  HAL2  CLSBLK
	KODDATA	-> SEC1  IDT  CLSBRK  OPNBLK  HAL3  CLSBLK
	KODDATA	-> SEC1  IDT  CLSBRK  OPNBLK  HAL2  HAL1  CLSBLK
	KODDATA	-> SEC1  IDT  CLSBRK  OPNBLK  HAL3  HAL1  CLSBLK
	KODDATA	-> DOVR_EXP  KODDATA
	KODDATA	-> FCALL1  NOQTEV
	KODDATA	-> SNMEM   NOQTEV
	KODDATA	-> DAXILET3
	KODDATA	-> CAPET3
	KODDATA	-> ASGN_OP
	KODDATA	-> EMPTYOP
	KODDATA	-> DAYAN1
	KODDATA	-> DAVAMET1
	KODDATA -> QAYTAR1


	FUNK_ELAN	->	FDECL YNT1 CLSBRK
	FUNK_ELAN	->	FDECL CLSBRK

	FUNK_BODY	->	OPNBLK VAR1 KOD1 CLSBLK
	FUNK_BODY	->	OPNBLK KOD1 CLSBLK


*	FUNKS		->	FUNK	FUNK
	FUNKS		->	FUNK	FUNKS
	FUNKS		->	FUNKS	FUNK
	FUNKS		->	FUNKS	FUNKS


	FUNK		->	FUNK_ELAN FUNK_BODY


	NEQEDERKI2	->	NEQEDERKI1  SHERT  CLSBRK  TEKRARET

	EGER_EXP2	->	EGER_EXP1  HSERT  CLSBRK  ONDA

*	FE1		->	FE1  VERGUL  FE1
	FE1		->	IDT  ia

	EMPBRK		->	OPNBRK  CLSBRK

	EMPTYOP		->	OPNBLK  CLSBLK

	DAVAMET1	->	DAVAMET  NOQTEV

	DAYAN1		->	DAYAN  NOQTEV

	NEQEDERKI1	->	NEQEDERKI  OPNBRK

	EGER_EXP1	->	EGER  OPNBRK

	DOVR_EXP	->	DOVR  OPNBRK  IDT        ARTSIN   NUMB    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG1       ARTSIN   NUMB    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG2       ARTSIN   NUMB    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  STRMEM     ARTSIN   NUMB    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
 
	DOVR_EXP	->	DOVR  OPNBRK  IDT        ARTSIN   IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG1       ARTSIN   IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG2       ARTSIN   IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  STRMEM     ARTSIN   IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET  

	DOVR_EXP	->	DOVR  OPNBRK  IDT        ARTSIN   CRG1    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG1       ARTSIN   CRG1    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG2       ARTSIN   CRG1    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  STRMEM     ARTSIN   CRG1    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET 

	DOVR_EXP	->	DOVR  OPNBRK  IDT        ARTSIN   CRG2    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG1       ARTSIN   CRG2    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG2       ARTSIN   CRG2    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  STRMEM     ARTSIN   CRG2    NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET

	DOVR_EXP	->	DOVR  OPNBRK  IDT        ARTSIN   STRMEM  NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG1       ARTSIN   STRMEM  NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG2       ARTSIN   STRMEM  NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  STRMEM     ARTSIN   STRMEM  NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET

	DOVR_EXP	->	DOVR  OPNBRK  IDT        AZALSIN  IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG1       AZALSIN  IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  CRG2       AZALSIN  IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET
	DOVR_EXP	->	DOVR  OPNBRK  STRMEM     AZALSIN  IDT     NEGSIG  DEN  IDT NEGSIG  AA  KIMI  CLSBRK  TEKRARET              



	ASGN_OP		->	IDT     ASGN  NUMB    NOQTEV
	ASGN_OP		->	IDT     ASGN  CHR     NOQTEV
	ASGN_OP		->	IDT     ASGN  FLOAT   NOQTEV
	ASGN_OP		->	IDT     ASGN  IDT     NOQTEV
	ASGN_OP		->	IDT     ASGN  CRG1    NOQTEV
	ASGN_OP		->	IDT     ASGN  CRG2    NOQTEV
	ASGN_OP		->	IDT     ASGN  EXPR    NOQTEV
	ASGN_OP		->	IDT     ASGN  STRMEM  NOQTEV
	ASGN_OP		->	IDT     ASGN  FCALL1  NOQTEV
	ASGN_OP		->	IDT     ASGN  SNMEM   NOQTEV

	ASGN_OP		->	CRG1     ASGN  NUMB    NOQTEV
	ASGN_OP		->	CRG1     ASGN  CHR     NOQTEV
	ASGN_OP		->	CRG1     ASGN  FLOAT   NOQTEV
	ASGN_OP		->	CRG1     ASGN  IDT     NOQTEV
	ASGN_OP		->	CRG1     ASGN  CRG1    NOQTEV
	ASGN_OP		->	CRG1     ASGN  CRG2    NOQTEV
	ASGN_OP		->	CRG1     ASGN  EXPR    NOQTEV
	ASGN_OP		->	CRG1     ASGN  STRMEM  NOQTEV
	ASGN_OP		->	CRG1     ASGN  FCALL1  NOQTEV
	ASGN_OP		->	CRG1     ASGN  SNMEM   NOQTEV

	ASGN_OP		->	CRG2     ASGN  NUMB    NOQTEV
	ASGN_OP		->	CRG2     ASGN  CHR     NOQTEV
	ASGN_OP		->	CRG2     ASGN  FLOAT   NOQTEV
	ASGN_OP		->	CRG2     ASGN  IDT     NOQTEV
	ASGN_OP		->	CRG2     ASGN  CRG1    NOQTEV
	ASGN_OP		->	CRG2     ASGN  CRG2    NOQTEV
	ASGN_OP		->	CRG2     ASGN  EXPR    NOQTEV
	ASGN_OP		->	CRG2     ASGN  STRMEM  NOQTEV
	ASGN_OP		->	CRG2     ASGN  FCALL1  NOQTEV
	ASGN_OP		->	CRG2     ASGN  SNMEM   NOQTEV

	CAPET3		->	CAPET2	CLSBRK  NOQTEV

	CAPET2		->	CAPET1  VDEC
	CAPET2		->	CAPET1  inacsfE
	CAPET2		->	CAPET1  STRMEM


	DAXILET3	->	DAXILET2  CLSBRK  NOQTEV

	DAXILET2	->	DAXILET1  VDEC
	DAXILET2	->	DAXILET1  ia
	DAXILET2	->	DAXILET1  STRMEM


*	STRMEM		->	ia  DOT  ia  (!DOT || ARYOPN)  
	STRMEM		->	ia  DOT  STRMEM

*	SNMEM		->	ia  DOT  FCALL1
	SNMEM		->	ia  DOT  SNMEM


*	STRUKT2		->	STRUKT1  YNT2  CLSBLK  NOQTEV
	STRUKT2		->	STRUKT1  CLSBLK  NOQTEV
	STRUKT2		->	STRUKT2  STRUKT2


	CAPET1		->	CAPET  OPNBRK

*	HAL1		->	HAL    NUMB  COLON
	HAL1		->	HAL    CHR  COLON
	HAL1		->	HAL1   HAL1
	HAL1		->	SUSMA  COLON

*	HAL2		->	HAL1   KODDATA  HAL1
	HAL2		->	HAL1   KODDATA  HAL2
	HAL2		->	HAL1   KODDATA  CLSBLK


*	HAL3		->	HAL2   HAL2
	HAL3		->	HAL2   HAL3
	HAL3		->	HAL3   HAL2
	HAL3		->	HAL3   HAL3

	SEC1		->	SEC    OPNBRK

	FCALL  VDEC	->	FCALL  inacsfEF
	FCALL  VDEC	->	FCALL  STRMEM
	FCALL		->	(!FUNKSIYA)  IDT  OPNBRK

*	VDEC		->	(!IDT)  inacsfE  VERGUL  inacsfE
	VDEC		->	(!IDT)  FCALL1   VERGUL  inacsfE
	VDEC		->	(!IDT)  STRMEM   VERGUL  inacsfE
	VDEC		->	(!IDT)  VDEC     VERGUL  inacsfE

	VDEC		->	(!IDT)  inacsfE  VERGUL  FCALL1
	VDEC		->	(!IDT)  FCALL1   VERGUL  FCALL1
	VDEC		->	(!IDT)  STRMEM   VERGUL  FCALL1
	VDEC		->	(!IDT)  VDEC     VERGUL  FCALL1

	VDEC		->	(!IDT)  inacsfE  VERGUL  STRMEM
	VDEC		->	(!IDT)  FCALL1   VERGUL  STRMEM
	VDEC		->	(!IDT)  STRMEM   VERGUL  STRMEM
	VDEC		->	(!IDT)  VDEC     VERGUL  STRMEM

	VDEC		->	(!IDT)  inacsfE  VERGUL  VDEC
	VDEC		->	(!IDT)  FCALL1   VERGUL  VDEC
	VDEC		->	(!IDT)  STRMEM   VERGUL  VDEC
	VDEC		->	(!IDT)  VDEC     VERGUL  VDEC


	FDECL		->	FUNKSIYA  IDT  OPNBRK


	STRUKT1		->	STRUKT  IDT  OPNBLK


	SINIF_HEAD_START->	SINIF  IDT

	SINIF_HEAD	->	SINIF_HEAD_START  OPNBLK

	SINIF_HEAD YNT  ->	SINIF_HEAD SINIF_BODY
	SINIF_HEAD FUNK ->	SINIF_HEAD SINIF_BODY  

	SINIF_HEAD    SINIF_BODY  ->	SINIF_HEAD (?MEXSUSI -fiktiv)  SINIF_DECL

	COLON YNT       ->	SINIF_HEAD SINIF_BODY
	COLON FUNK      ->	SINIF_HEAD SINIF_BODY

*	SINIF_BODY	->	SINIF_BODY SINIF_BODY
	SINIF_BODY	->	SINIF_BODY YNT2
	SINIF_BODY	->	SINIF_BODY FUNK

	SINIF_DECL	->	QORUNAN  COLON  SINIF_BODU
	SINIF_DECL	->	MEXSUSI  COLON  SINIF_BODU
	SINIF_DECL	->	SERBEST  COLON  SINIF_BODU
	SINIF_DECL	->	SINIF_DECL  SINIF_DECL


	SINIF_TYPE	->	SINIF_HEAD  SINIF_DECL  CLSBLK  NOQTEV

	DAXILET1	->    	DAXIL  OPNBRK

	VAR1		->	VAR  COLON


	YNT2		->	YNT1   YNTS  NOQTEV
	YNT2  		->	YNT1   NOQTEV
*	YNT2		->	YNT2   YNT2

*	YNT1		->	YNT1   YNT1
	YNT1		->	IDT	ia

	YNT1  YNTS	->	YNT1    VERGUL  ia

	YNTS		->	YNTS  VERGUL   ia
	
	YENI_TIP	->	SINIF_TYPE   SINIF_TYPE
	YENI_TIP	->	STRUKT2      SINIF_TYPE
	YENI_TIP	->	YENI_TIP     SINIF_TYPE
	YENI_TIP	->	SINIF_TYPE   STRUKT2
	YENI_TIP	->	STRUKT2      STRUKT2
	YENI_TIP	->	YENI_TIP     STRUKT2
	YENI_TIP	->	SINIF_TYPE   YENI_TIP
	YENI_TIP	->	STRUKT2      YENI_TIP
*	YENI_TIP	->	YENI_TIP     YENI_TIP

*	SHERT		->	SHERT   VE    SHERT
	SHERT		->	SHERT   VEYA  SHERT
	SHERT		->	inacsf  cond  inacf
	SHERT		->	EXPR    cond  inacf
	SHERT		->	STRMEM  cond  inacf
	SHERT		->	inacsf  cond  EXPR
	SHERT		->	EXPR    cond  EXPR
	SHERT		->	STRMEM  cond  EXPR
	SHERT		->	inacsf  cond  STRMEM
	SHERT		->	EXPR    cond  STRMEM
	SHERT		->	STRMEM  cond  STRMEM

	OPNBRK  ina     CLSBRK    ->     ina  (HERE WE JUST REMOVE BRACKETS)
	OPNBRK  EXPR    CLSBRK    ->     ina  (HERE WE JUST REMOVE BRACKETS)
	OPNBRK  FCALL1  CLSBRK    ->     ina  (HERE WE JUST REMOVE BRACKETS)
	OPNBRK  SNMEM   CLSBRK    ->     ina  (HERE WE JUST REMOVE BRACKETS)
	OPNBRK  STRMEM  CLSBRK    ->     ina  (HERE WE JUST REMOVE BRACKETS)
	OPNBRK  SHERT   CLSBRK    ->     ina  (HERE WE JUST REMOVE BRACKETS)

*	CRG2		->	IDT  ARYOPN  NUMB  ARYCLS  ARYOPN  NUMB  ARYCLS
	CRG2		->	IDT ARYOPN  ina     ARYCLS ARYOPN  ina ARYCLS
	CRG2		->	IDT ARYOPN  EXPR    ARYCLS ARYOPN  ina ARYCLS  
	CRG2		->	IDT ARYOPN  STRMEM  ARYCLS ARYOPN  ina ARYCLS  
	CRG2		->	IDT ARYOPN  ina     ARYCLS ARYOPN  EXPR ARYCLS
	CRG2		->	IDT ARYOPN  EXPR    ARYCLS ARYOPN  EXPR ARYCLS  
	CRG2		->	IDT ARYOPN  STRMEM  ARYCLS ARYOPN  EXPR ARYCLS
	CRG2		->	IDT ARYOPN  ina     ARYCLS ARYOPN  STRMEM ARYCLS
	CRG2		->	IDT ARYOPN  EXPR    ARYCLS ARYOPN  STRMEM ARYCLS  
	CRG2		->	IDT ARYOPN  STRMEM  ARYCLS ARYOPN  STRMEM ARYCLS  

*	CRG1		->	IDT  ARYOPN  NUMB  ARYCLS
	CRG1		->	IDT ARYOPN  ina     ARYCLS
	CRG1		->	IDT ARYOPN  EXPR    ARYCLS
	CRG1		->	IDT ARYOPN  STRMEM  ARYCLS


	QAYTAR1		->	QAYTAR  inacsfE  NOQTEV
	QAYTAR1		->	QAYTAR  STRMEM   NOQTEV
	QAYTAR1		->	QAYTAR  NOQTEV

	FCALL1		->	FCALL  VDEC  CLSBRK
	FCALL1		->	FCALL  CLSBRK

*	EXPR		->	(!hp_opr)  inaf    lp_opr  inaf (!hp_opr)
	EXPR		->	(!hp_opr)  EXPR    lp_opr  inaf (!hp_opr)
	EXPR		->	(!hp_opr)  STRMEM  lp_opr  inaf (!hp_opr)
	EXPR		->	(!hp_opr)  inaf    lp_opr  EXPR (!hp_opr)
	EXPR		->	(!hp_opr)  EXPR    lp_opr  EXPR (!hp_opr)
	EXPR		->	(!hp_opr)  STRMEM  lp_opr  EXPR (!hp_opr)
	EXPR		->	(!hp_opr)  inaf    lp_opr  STRMEM (!hp_opr)
	EXPR		->	(!hp_opr)  EXPR    lp_opr  STRMEM (!hp_opr)
	EXPR		->	(!hp_opr)  STRMEM  lp_opr  STRMEM (!hp_opr)
	EXPR		->	inaf    hp_opr  inaf
	EXPR		->	EXPR    hp_opr  inaf
	EXPR		->	STRMEM  hp_opr  inaf
	EXPR		->	inaf    hp_opr  EXPR
	EXPR		->	EXPR    hp_opr  EXPR
	EXPR		->	STRMEM  hp_opr  EXPR
	EXPR		->	inaf    hp_opr  STRMEM
	EXPR		->	EXPR    hp_opr  STRMEM
	EXPR		->	STRMEM  hp_opr  STRMEM







 

/* match against id , nb  */
int match_in(int k){

  return (matchid(k, IDT) || matchid(k, NUMB));
  
 }


	ia		->	IDT
	ia		->	CRG1
	ia		->	CRG2



/* match against id , nb or ary, ary will be implemented in  future  */
int match_ina(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2);
  
 }

/* match against id , nb or ary, ary will be implemented in  future  */
int match_inaf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2)  || matchid(k, FLOAT) ;
  
 }

int match_inas(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, STR);
  
 }

int match_inac(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR);
  
 }


int match_inacf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, FLOAT) ;
  
 }


int match_inacs(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR);
  
 }

int match_inacsf(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT) ;
  
 }


int match_inacsfE(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT)   || matchid(k, EXPR) ;
  
 }

int match_inacsfEF(int k){

  return matchid(k, IDT) || matchid(k, NUMB) || matchid(k, CRG1) || matchid(k, CRG2) || matchid(k, CHR)  || matchid(k, STR)  || matchid(k, FLOAT)   || matchid(k, EXPR) || \
 matchid(k, FCALL1);
  
 }


/*
int match_opr(int k){

}
*/
int match_lp_opr(int k){

  return (matchid(k, POSSIG) ||  matchid(k, NEGSIG)) ;

}


int match_hp_opr(int k){

  return ( matchid(k, MULT) ||  matchid(k, DEL) ||  matchid(k, PRCT)) ;

}

int match_opr(int k){

  return match_lp_opr(k) || match_hp_opr(k);

}

int match_cond(int k){

  return (matchid(k, GRT) ||  matchid(k, LES) ||  matchid(k, BERABER) ||  matchid(k, FERQLI) ||  matchid(k, GRTEQU) ||   matchid(k, LESEQU) \
	      ||   matchid(k, VE)  ||   matchid(k, VEYA)) ;

}

int match_tsky(int k){

  return (matchid(k, TAM1) || matchid(k, SIMVOL1) || matchid(k, KESR1) || matchid(k, YNT1));

}

  
  if (matchid(a,PRG) && matchid(b, OPNBLK) && matchid(c,KOD1) && matchid(d,CLSBLK)  ){
       ptr->id = PRG1;




  if (matchid(a,DOVR)){
   if (matchid(b, OPNBRK)){
     if (match_ia(c) || matchid(c, STRMEM)){
     if (matchid(d, ARTSIN) || matchid(d, AZALSIN) ){
       if (match_ina(e) || matchid(e, STRMEM)){
       if(matchid(f, NEGSIG)){
        if(matchid(g,DEN) || matchid(g,DAN)){
	  if(match_ina(h) || matchid(h, STRMEM)){
	  if(matchid(i, NEGSIG)){
	   if(matchid(j,AA)||matchid(j,EE)||matchid(j,YA)||matchid(j,YE)){
	    if (matchid(k,KIMI)){
	     if(matchid(l, CLSBRK)){
	      if(matchid(m, TEKRARET)){
ptr->id = DOVR_EXP;



