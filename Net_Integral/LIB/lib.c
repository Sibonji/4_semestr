#include "lib.h"
#include "enum.h"

int check_args (int argc, char* argv[]) {
    if(argc != 2)
        return -incorrect_args;
	
	char* num_end;
	int biggest = strtol(argv[1], &num_end, 0);
	if (errno == ERANGE)
        return -incorrect_args;
	else if (num_end != NULL && *num_end)
        return -incorrect_args;
	else if (biggest <= 0)
        return -incorrect_args;

    return biggest;
}