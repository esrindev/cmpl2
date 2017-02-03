 
symtab_new.cpp:

   print_ttable funksiyasinda 

   if (strcmp(ttab->tips[i].name, "\0")!=0) muqaisesi niye aparilir?  
   adi "\0" olan tipler neyi bildirir?  onlari yox elemek olmaz?

  

======================================


symtab_new.cpp

buid_symtab(){
   
   mence parsing agaci bir nece defe tekrar axtarilir.
	   Bunlarin sayini azaltmaq olmaz?
	   axtarishi optimallashdirmaq olmaz?

simvollar cedvelinin qurulmasi alqoritminin tekmilleshdirilmesi
   
   }
 

=====================================

	   deyishenlerin lokal ve qlobal olmasi blok daxilinde temin olunur,
	   lakin evvelce elan et sonra muraciet et prinsipi temin olunmur.
	   yeni ki, deyishene blok daxilinde elan setrinden evvel de muraciet etmek olar.

===============================================
create_tok_list_el() her defe malloc ile tll yaradir, amma hec de hemishe 
onlar consume olunmur. lakin consume olmayan ttl -ler free -de olunmur, yaddashi israf eliyir
======================================================
addto_band_new() -da 

    strcpy(band[*size].tok, tok->tok);

     tok NULL ile muqaise olunur. Icra lentine tok -un yerleshdirilmesi niye lazimdir?
=====================================================
BLOCK -un consumu, funksiyalar ve sinifler ucun bir cur aparilir,
koddata ucun bir cur. NEticede BLOCK -u cons eliyende ilk tll -i IDT ile
muqaise eliyirik. bu ise sirf blokdan ibaret kod olanda metni TEXT -e consume
elemir. Ashagidaki kod ishlemez, sintaksis sehv verecek:

	 {
		 capet("Salam dunya");
	 }

 BLOCK -un consume meselesini tekmilleshdirmek lazimdir.
==========================================================
son ; proqramdan cixish funkisiyasini elave et

=============================================================
eger menfi simvolu ile eded arasinda probel varsa onda eded menfi qebul olunmur

  xaricet( -4 ); // normal
   
  xaricet( - 4); // sintaksis sehv: '-' ishresinden sonra ifade gozlenirdi

=============================================================================
  decrement ve increment ifade daxilinde desteklenmir. 
  yalniz ayri shekilde desteklenir/

  y + x-- +4 // sintaksis sehv
  xaricet(x--) //sitaksis sehv

  x--; //dogru
  --x; //dogru

===============================================================================

 +=
 -=
 *=
 /=
 %=  
  emeliyyatlari desteklenmir
  bit emeliyyatlari << >> ~ v.s. desteklenmir
===============================================================================

can we print fcall like

funksiya cem(tam x, tam y){
	qaytar x+y;
}

xaricet(cem(3,4));  //????

==============================================================================