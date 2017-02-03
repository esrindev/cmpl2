 //ferz qambit. shahmat taxtasinda 8 vezirin bir-birini
 // vurmamaq shertile yerleshdiren proqram

  // movqeler , qlobal cerge
	tam v[8];

  //ferqin mutleq qiymetini qaytaran funisya
  funksiya abs(tam x, tam y){
	
		eger (x >= y)
			qaytar x - y;
		
		qaytar (y - x);	
	
	}
	
	 /* kordinanatlari (i,v[i]) ve (j, v[j]) olan iki vezirin
	       bir -birini vurdugunu yoxlayan funksiya
				 
			vurursa 1 qaytar, yoxsa 0	 
      */				 
	
	funksiya yoxla_ij(tam i, tam j){
	
		tam dx, dy;
				
		dx = abs(i,j);
		dy = abs(v[i], v[j]);
		
		eger (dx == dy veya v[i] == v[j])
			qaytar 1; //vurur
			
		//vezirler bir-birin vurmur, kordinantlari cap et	
		//xaricet("(",i," , ",v[i],") , (",j, " , ", v[j], " \y");	
		
		qaytar 0;//vurmur
	}
	
	//butun 8 vezirin bir-birini vurdugunu yoxla
	funksiya yoxla(){
	
		tam k,m,n,vurur;
	
		vurur = 0;
		
		say(k 7){
			// hazirki buraxilishda dovrun saygacin ifade kimi
			// vermek mumkun olmadigina gore dovrden kenar hesablamaliyiq
			n = k+1;
			say(m n 8){
				vurur = yoxla_ij(k,m);
				eger (vurur == 1)
					qaytar;
			}
		}		

		movqeleri_capet();
	}
	
	funksiya movqeleri_capet(){
	
		tam i;
		
		say(i 8)
			xaricet("(",i," , ",v[i],") ");
			
		xaricet("\y");		
	}
	
	funksiya esas(){
	
	tam k, min, max, i, j;
	 
	k = 1;	
	min = 0;
	max = 0;
	
	say(i 8){
		max = max + k;
		k = k * 8;
		}
		
  max = 7 * max; 
		
	xaricet("min = ", min," max = ", max, "\y");	
		
	say (i min max)	{
		k = i;

		//movqeleri hesabla	
		say(j 8){
			v[j] = k % 8 + 1; // 0--7 -ni 1--8 -e inikas etmek ucun
			k = k/8;
		}
		
		yoxla();		
	}
	
 }
	
	esas();
	