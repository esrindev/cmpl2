 

sehvler
=====================
deyishenlein elaninda tip gostermeyende
duzgun sehv ismarici vermir
===============================

==================
funksiyaya oturulen parametrlerinin sayi
duzgun olmayanda sehv mesaji verilmir,
icra zamani ise qiymətlər steki kolanır və
proqram qeyri düzgün işləyir və ya qəza ilə
başa çatır.
=====================

==========================
//ashagidaki kod test funksiyasi tapilmadi
//sehvini verir, bir dene } moterizesi artiqdir
//funksiyani gormur

//proqramin esas hissesi
proqram {
	
	var:
		tam x,y;
	
	kod:
	
		x = 12;
		y = 34;
		
		//qlobal deyisheler cap olunur
		capet("x = ", x, "\y");
		capet("y = ", y, "\y");
		
		}
	
}

funksiya test ( ){
	
	var:
	
		// x ve y adli qlobal deyishenler
		// olmasina baxmayaraq funksiya daxilinde
		// eyni adli lokal deyishenler elan eded
		// bilerik
		tam x, y;

	kod:
	
	x = 17;
	y = 45;
	
	// capet lokal x ve y-in qiymetleri cap edecek
	capet("x = ", x, "\y");
	capet("y = ", y, "\y");
	
}
============================================

===
sinfin oz tipinden sinif elan eliyende 
compilyator donur
===

===
ashagidaki proqramda sehv odur ki yoxsa evezine else yazilib
sehv duzgun tapilmir
==
proqram {
	
var:
  tam x,y,z,h;

kod:

capet("Anadan oldugunuz ay ve gunu daxil edin.");
daxilet(x,y);

capet("Bugun ucun ay ve gunu daxil edin");
daxilet(z,h);

eger (x == z ve y == h)onda 
 capet("Tebrik edirik bu gun sizin ad gununuzdur.\y");
 yoxsa
   eger (x <= z ve y < h) onda 
     capet ("Sizin bu ilki ad gununuz kecib\y");
	else 
	 capet ("Sizin bu ilki ad gununuz hele qabaqdadir\y");








}
==

=========================
  capet("Salam dunya \"");

bunu komp edende error verir
string DFA -ni yenile

========================

token.c '\'' simvolunu tanımır
token.c üçün requlyar dil yaratmaq və həmin dilin tanınması üçün
DFA maşını qurmaq. Səhvlərin tanınmasını işləmək.

Hell olunub

=================================

  qiymet qaytarmali funksiyanin qaytardigi qiymet hemishe valstk -e 
  yerleshdirilir. Eger hemin qiymet istifade olunmasa bu stekin korlanmasi
  demekdir.

  x = f();
  Bu halda f() -in steke yerleshdirdiyi neticeni menimsetme operatoru goturecek.
  
  f();
  Bu halda f() -in steke yerleshdirdiyi netice hemishelik orada qalacaq. Bu ise stekin 
  korlanmasi demekder.


=================================

cergelerin indekslerinin qiymetinin kenardancixmasini compilyasiya vaxti yoxlamir.

  tam k[0];

  k[1] = 9;

  kodu kompilyasiya vaxti sehv ve ya xeberdarliq vermir.
	  Yalniz icra vaxti sehv melumat verir ve proqram basha catir.

=======================================================================
    
  say(10){
	}

     davamet ;

  error vermir ki davamet loopdan kenardi

=======================================================================

say(k x y)

  burada say -i parse edende k x ve y axtarir, ama x x ynt1-e parse daha evvel
  ==============================================================================
 

   

 

	==============================================================

		//tiplerin  yoxlanmasi xetasi

		 //bu proqram duz ishlemir

   
 
   
 funksiya f(){
    int emel;
    
	}
	
	
	f();
 
 // int tipinin elan olunmama sehvi iki defe verilir

	======================================

		elan olunmayan deyishenin her istifadesi elave xeta verir.

  ===============================================

  dovrde saygac qiymeti olaraq ededi ifade istifade etmek olmur

   say(x y+1 z) // sintaksis sehv verecek;

   ////////////////////////////////

   6 +9  -> NUMB NUMB kimi hesablanir


   ==================================

   you do not free memory consumed for creation of ptr in consume funcs of parser

   === 