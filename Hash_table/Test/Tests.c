#include "Tests.h"

void* fp_open () {
    FILE* fp = NULL;
    if ((fp = fopen ("Tests.txt", "w+")) == NULL) {
        fprintf (stderr, "Couldn't open file to write tests!\n");
        return NULL;
    }

    return fp;
}

void cmd_write (FILE* fp) {
    int cmd = rand () % 3;

    switch (cmd)
    {
    case 0:
        fprintf (fp, "a ");
        break;
    
    case 1:
        fprintf (fp, "f ");
        break;

    case 2:
        fprintf (fp, "d ");
        break;

    default:
        fprintf (fp, "e ");
        break;
    }

    return;
}

void word_write (char* word, FILE* fp) {
    int cmd_len = rand () % MAX_CMD_SIZE + 2;
    for (int j = 0; j < cmd_len; j++) {
        word[j] = (char) (rand () % 74 + 48);
    }

    word[cmd_len] = '\0';

    fprintf (fp, "%s\n", word);

    return;
}

int main () {
    FILE* test = NULL;
    if ((test = fp_open ()) ==  NULL) return -1;

    srand (time (NULL));
    
    char word[MAX_CMD_SIZE + 2];
    int cmd = 0;

    fprintf (test, "%d\n", CMD_QUANT);

    for (int i = 0; i < CMD_QUANT; i++) {
        cmd_write (test);

        word_write (word, test);
    }

    fclose (test);

    return 0;
}