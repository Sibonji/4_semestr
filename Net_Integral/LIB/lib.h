#ifndef LIB_H
#define LIB_H

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */

#include	<time.h>		/* old system? */

#include	<netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include	<arpa/inet.h>	/* inet(3) functions */
#include	<errno.h>
#include	<fcntl.h>		/* for nonblocking */
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	/* for S_xxx file mode constants */
#include	<sys/uio.h>		/* for iovec{} and readv/writev */
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		/* for Unix domain sockets */

#include    <sys/time.h>
#include    <math.h>
#include    <sys/sysinfo.h>

#include    <netinet/tcp.h>

#define WAIT_TIME_SEC 0
#define WAIT_TIME_USEC 1000

#define CONNECT_WAIT_TIME_SEC 1000
#define CONNECT_WAIT_TIME_USEC 0

#define PR_CNT 5
#define PR_IDLE_TIME 2
#define PR_BTW_TIME 1

#define PORT_NUM 4567
#define CONNECT_PORT_NUM 4577

#define __N 1024

int check_args (int argc, char* argv[]);

void server_start (int pc_quant);
void client_start (int thread_quant);

const char* get_enum_name (int enum_name);
void print_error (int err_num);

void make_serv_connect ();
int make_client_connect (struct sockaddr_in* peer_adr, struct sockaddr_in* sender_adr);

void listen_create (int __fd, int __n);
int accept_client (int fd, struct sockaddr* addr, socklen_t* addr_len);

#endif