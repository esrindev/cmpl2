/* GPL  V.3  
  Esrin Compiler Main Module
  Ahmed Sadikhov, ahmed.sadikhov@gmail.com
*/

#include "esrin.h"

int main(int argc, char *argv[]){

  printf("Proqram kompilyasiya olunur...\n");
 
  tokenize("esrin_src_prg.esr"); 

  parse();
   
  //symtab();
  symtab_new();

 // exit(0);
 
  printf("Kod generasiya olunur...\n");

  generate();

  printf("Tamam.\n");

}
 
