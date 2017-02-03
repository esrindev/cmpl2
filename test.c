
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void dump_to_file(char *fname){

  int fd;

  fd = open(fname, O_CREAT | O_WRONLY, 0666);
  write(fd, "hey", 3);
  close(fd);

}


int main(){
 
  dump_to_file("prg.bin_test");
}
