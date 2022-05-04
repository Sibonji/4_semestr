#include "Hash_table.h"
#include "Test_hash_table.h"

void* fp_open () {
    FILE* fp = NULL;
    if ((fp = fopen ("Test/Tests.txt", "r")) == NULL) {
        fprintf (stderr, "Couldn't open file to write tests!\n");
        return NULL;
    }

    return fp;
}

int main () {
    FILE* fp = NULL;
    if ((fp = fp_open ()) == NULL) return -1;

    return 0;
}