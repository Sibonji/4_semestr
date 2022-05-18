#include "../LIB/lib.h"
#include "../LIB/enum.h"

int main (int argc, char* argv[]) {
    int thread_quant = check_args (argc, argv);
    if (thread_quant < 0)
        fprintf (stderr, "Incorrect command line arguments!\n");
    else
        client_start (thread_quant);

    return 0;
}

void client_start (int thread_quant) {
    int sender_fd;
    struct sockaddr_in server_adr;
    struct sockaddr_in peer_adr;
    struct sockaddr_in sender_adr;

    memset (&sender_adr, 0, sizeof (sender_adr));
    memset (&peer_adr, 0, sizeof (peer_adr));

    sender_adr.sin_addr.s_addr = htonl (INADDR_ANY);
    sender_adr.sin_port = htons (CONNECT_PORT_NUM);
    sender_adr.sin_family = AF_INET;

    int serv_port_num = make_client_connect (&peer_adr, &sender_adr);

    memset (&server_adr, 0, sizeof (server_adr));
    server_adr.sin_addr = peer_adr.sin_addr;
    server_adr.sin_port = htons (PORT_NUM);
    server_adr.sin_family = AF_INET;

    if ((sender_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
        print_error (-bad_socket);

    int pr_cnt = PR_CNT;
    int pr_idle_time = PR_IDLE_TIME;
    int pr_btw_time = PR_BTW_TIME;

    if (setsockopt (sender_fd, IPPROTO_TCP, TCP_KEEPCNT, &pr_cnt, sizeof (pr_cnt)) != 0)
        print_error (-bad_socket);
    if (setsockopt (sender_fd, IPPROTO_TCP, TCP_KEEPIDLE, &pr_idle_time, sizeof (pr_idle_time)) != 0)
        print_error (-bad_socket);
    if (setsockopt (sender_fd, IPPROTO_TCP, TCP_KEEPINTVL, &pr_btw_time, sizeof (pr_btw_time)) != 0)
        print_error (-bad_socket);

    socklen_t server_adr_size = sizeof (server_adr);
    if (connect (sender_fd, (struct sockaddr*) &server_adr, server_adr_size) < 0)
        print_error (-bad_connect);

    if (send (sender_fd, &thread_quant, sizeof (thread_quant), MSG_NOSIGNAL) != sizeof (thread_quant))
        print_error (-bad_send);
}

int make_client_connect (struct sockaddr_in* peer_adr, struct sockaddr_in* sender_adr) {
    socklen_t peer_adr_size = sizeof (peer_adr);
    int one = 1;
    int serv_port_num = 0;

    int connect_fd = 0;
    if ((connect_fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
        print_error (-bad_socket);

    if (setsockopt (connect_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (one)) < 0)
        print_error (-bad_socket);

    if (bind (connect_fd, (struct sockaddr *) sender_adr, sizeof (struct sockaddr_in)) < 0)
        print_error (-bad_bind);

    printf ("Waiting server message!\n");
    recvfrom (connect_fd, &serv_port_num, sizeof (serv_port_num), 0,
              (struct sockaddr *) peer_adr, &peer_adr_size);

    printf ("Server message received!\n");

    close (connect_fd); 

    //printf ("Port num is: %d\n", serv_port_num);       

    return serv_port_num;
}