/* GPL  V.3  
  Esrin Compiler Main Module
  Ahmed Sadikhov, ahmed.sadikhov@gmail.com
*/

#include "esrin.h"

int main(int argc, char *argv[]){

  printf("Proqram kompilyasiya olunur...\n");

  if (argc != 2){
	  printf("Istifade qaydasi: cmpl2  menbe_fayli\n");
	  exit(0);
  }

  tokenize(argv[1]);

  parse();

  exit(0);
   
  symtab();

  printf("Kod generasiya olunur...\n");

  generate();

  printf("Tamam.\n");

}
 
