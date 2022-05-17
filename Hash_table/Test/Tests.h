#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int CMD_QUANT = 10000;
const int MAX_CMD_SIZE = 10;

void* fp_open ();
void cmd_write (FILE* fp);
void word_write (char* word, FILE* fp);
