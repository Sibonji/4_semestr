#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int check_args (int argc, char* argv[]);

void server_start (int pc_quant);

#endif