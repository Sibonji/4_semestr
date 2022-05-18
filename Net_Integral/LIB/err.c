#include "lib.h"
#include "enum.h"

const char* get_enum_name (int enum_name) {
    switch (enum_name) {
        case success: return "success";
        case -incorrect_args: return "incorrect_args";
        case -bad_socket: return "bad_socket";
        case -bad_bind: return "bad_bind";
        case -bad_listen: return "bad_listen";
        case -bad_send: return "bad_send";
        case -bad_accept: return "bad_accept";
        case -bad_connect: return "bad_connect";
        default: return "undef_err_name";
    }
}

void print_error (int err_num) {
    fprintf (stderr, "Error received during programm work\n"
                     "Error name: %s, error num: %d\n", get_enum_name (err_num), err_num);

    abort ();
    exit (err_num);
}