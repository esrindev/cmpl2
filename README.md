# cmpl2
Əsrin kompilyatorunu 2-ci inkişafda olan buraxılışı

Visual Studio 2008 ilə kompilyasiya olunur/ C++

Kompilyasiya etdikdən sonra işçi qovluqda (mənbə faylları yerləşən qovluq)
cmpl2 adlı proqram - esrin kompilyatorunun 2-ci test versiyası yaranacaq.
Hal-hazırda ikinci buraxılışın yalnız səhvlərin tanınması sistemi tərtib olunub.

  Səhvlərin tanınma sistemin test eləmək üçün 
    cmpl2 mənbə_faylı 
  əmrini daxil etmək lazımdır. mənbə_faylında əsrin dilində olan proqram mətni 
  olmalıdır. Nümunə mənbə faylı və Çıxış nəticələri verilib
  
  Nümunə mənbə faylı - prg.esr
  
  

		  tam x, y, z 10], k;  // [ ommitted after z

		 x = 4 + 9   // ; ommitted

		 say(k, 1, 3){
			eger ( x >  )  // ommitted right operand of comparision
			 y =  z[x + 4] - 12 ;
		}
		
  Bu nümunəyə uyğun kompilyatorun çıxışı - sample_output.txt

		   [C:\\users\Aspire]# cmpl2  prg.esr
		Proqram kompilyasiya olunur...
		Checking grammar errors
		(3:13)sehv: `z` -dan sonra  dogru ifade yazilmayib  .
		(5:11)sehv: `9` -dan sonra  dogru ifade yazilmayib  .
		(8:14)sehv: `>` -dan sonra  dogru ifade yazilmayib  .
		(10:1)sehv: `}` -dan sonra  dogru ifade yazilmayib  .

		--------------------------------------------
		FILEBEG VDEC NUMB ARYCLS VERGUL IDT NOQTEV IDT ASGN EXPR SAY_TIP_1 OPNBLK EGER OPNBRK IDT GRT CLSBRK TEXT CLSBLK FILESON FILESON FILESON FILESON FI
		LESON
		TAM SEQ NUMB POSSIG NUMB SAY BRK_SEQ SIMPLE_OPER
		SEQ VERGUL IDT OPNBRK SEQ CLSBRK ASGN_OP
		IDT VERGUL IDT SEQ VERGUL NUMB IDT ASGN EXPR NOQTEV
		IDT VERGUL NUMB CRG1 NEGSIG NUMB
		IDT ARYBRK_VAL
		ARYOPN EXPR ARYCLS
		IDT POSSIG NUMB
  
  Kompilyator əvvəlcə parsing ağacını qurur, səhvləri yoxlayır və çap edir, 
  sonda isə parsing ağacını çap edir.


Hal-hazırda Simvollar cədvəllərinin yeni buraxılışı yığılmaqdadır.
