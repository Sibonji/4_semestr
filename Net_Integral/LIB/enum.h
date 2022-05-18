#ifndef ENUM_H
#define ENUM_H

enum {
    success,
    
    incorrect_args,
    bad_socket,
    bad_bind,
    bad_listen,
    bad_send,
    bad_accept,
    bad_connect,
    bad_recv,
    undef_err_name,

    cont,
    stop,

    ok,

    bad_alloc,
    bad_cr_thread,
    bad_set,
    bad_join,

    no_clients
};

#endif