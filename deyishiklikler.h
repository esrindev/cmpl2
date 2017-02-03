 

interpretatorda 
exec_crg2_adrs
		 _ofst
		 _val
		  
	 -da adrs -in hesablanmasi yenilenib
	 dusturda max_row -un yerine max_col olmalidir


===========================
//list_new.cpp setir 706
>>evvel
  case QAYTAR1:
    if (tll->cld_cnt == 3){
      addto_band_val(tll->childs[1], band, size);      
      addto_band_new(tll, band, size, QAYTAR1, 0); 
    }
      
    break;

>>sonra
  case QAYTAR1:
    if (tll->cld_cnt == 3){
      addto_band_val(tll->childs[1], band, size);      
      addto_band_new(tll, band, size, QAYTAR1, 0); 
    }
	else
	addto_band_new(tll, band, size, QAYTAR1, 0); 

	break;

	==son
================================


 token.cpp

 new line her yerde cur_row++ hissesinde \n ile yoxlanilir
 bir yerde ise fin_sps evvelce \r npvbetinin \n olmasi teleb olunurdu
