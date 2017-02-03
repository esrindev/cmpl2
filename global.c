/*  frame for global data  */
#include <stdio.h>

#include "buta.h"

int number_of_lines = 0;

struct setir metn[MAXLINE];

/*  hazirki setir  */
int cnt = 0;

char *lines[MAXLINE];

char *buf;

int prg_start, data_start, data_end, code_start, code_end, prg_end;

int first_nmp_ln, last_nmp_ln;




