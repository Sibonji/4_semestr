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

void listen_create (int __fd, int __n) {
    char* name;
    if ((name = getenv ("LISTENQ")) != NULL)
        __n = atoi (name);

    if (listen (__fd, __n) < 0)
        print_error (-bad_listen);

    return;
}

int accept_client (int fd, struct sockaddr* addr, socklen_t* addr_len) {
    int ret_num;

    if ((ret_num = accept (fd, addr, addr_len)) < 0) {
        if (errno == EAGAIN)
            return -no_clients;
        else
            print_error (-bad_accept);
    }
    return ret_num;
}