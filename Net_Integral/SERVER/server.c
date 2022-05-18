#include "../LIB/lib.h"
#include "../LIB/enum.h"

int main (int argc, char* argv[]) {
    if (argc == 2 && strcmp(argv[1], "loop_mode") == 0) {
        while (1) {
            printf ("Enter pc quantity, if you want to quit enter -1: ");
            fflush (stdout);

            int pc_quant = 0;
            scanf ("%d", &pc_quant);
            if (pc_quant > 0)
                server_start (pc_quant);
            else if (pc_quant == -1)
                return 0;
            else
                printf ("Incorrect pc quantity!\n");
        }
    }

    int pc_quant = check_args (argc, argv);
    if (pc_quant < 0)
        fprintf (stderr, "Incorrect command line arguments!\n");
    else
        server_start (pc_quant);

    return 0;
}

void server_start (int pc_quant) {
    int one = 1;
    int receiver_fd = 0;
    struct sockaddr_in serv_adr;
    
    struct timeval wait_time  = {
        .tv_sec = WAIT_TIME_SEC,
        .tv_usec = WAIT_TIME_USEC
    };

    if ((receiver_fd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
        print_error (-bad_socket);

    if (setsockopt (receiver_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (one)) != 0)
        print_error (-bad_socket);
    if (setsockopt (receiver_fd, SOL_SOCKET, SO_RCVTIMEO, &wait_time, sizeof (wait_time)) != 0)
        print_error (-bad_socket);
    if (setsockopt (receiver_fd, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof (one)) != 0)
        print_error (-bad_socket);

    int pr_cnt = PR_CNT;
    int pr_idle_time = PR_IDLE_TIME;
    int pr_btw_time = PR_BTW_TIME;

    if (setsockopt (receiver_fd, IPPROTO_TCP, TCP_KEEPCNT, &pr_cnt, sizeof (pr_cnt)) != 0)
        print_error (-bad_socket);
    if (setsockopt (receiver_fd, IPPROTO_TCP, TCP_KEEPIDLE, &pr_idle_time, sizeof (pr_idle_time)) != 0)
        print_error (-bad_socket);
    if (setsockopt (receiver_fd, IPPROTO_TCP, TCP_KEEPINTVL, &pr_btw_time, sizeof (pr_btw_time)) != 0)
        print_error (-bad_socket);

    memset (&serv_adr, 0, sizeof (serv_adr));
    serv_adr.sin_addr.s_addr = htonl (INADDR_ANY);
    serv_adr.sin_port = htons (PORT_NUM);
    serv_adr.sin_family = AF_INET;

    if (bind(receiver_fd, (struct sockaddr *) &serv_adr, sizeof (serv_adr)) < 0)
        print_error (-bad_bind);

    listen_create (receiver_fd, __N);

    make_serv_connect ();

    int connect_fds[pc_quant];
    struct sockaddr_in serv_addrs[pc_quant];

    struct timeval connect_time_wait = {
        .tv_sec = CONNECT_WAIT_TIME_SEC,
        .tv_usec = CONNECT_WAIT_TIME_USEC
    };

    int client_num;
    for (client_num = 0; client_num < pc_quant; client_num++) {
        connect_fds[client_num] = accept_client (receiver_fd, NULL, NULL);

        if (connect_fds[client_num] == -no_clients) 
            break; //send msg to children that programm is finished, write that code

        if (setsockopt (connect_fds[client_num], SOL_SOCKET, SO_RCVTIMEO,
                        &connect_time_wait, sizeof (connect_time_wait)) != 0)
            print_error (-bad_socket);

        serv_addrs[client_num].sin_port = htons (CONNECT_PORT_NUM);
        serv_addrs[client_num].sin_family = AF_INET;

        printf ("Connected!\n");
    }



    printf ("Programm finished!\n");

    return;
}

void make_serv_connect () {
    int one = 1;
    
    struct sockaddr_in connect_adr;
    int connect_fd;
    
    if ((connect_fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
        print_error (-bad_socket);

    if (setsockopt (connect_fd, SOL_SOCKET, SO_BROADCAST, &one, sizeof (one)) < 0)
        print_error (-bad_socket);
    if (setsockopt (connect_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof (one)) < 0)
        print_error (-bad_socket);

    memset (&connect_adr, 0, sizeof (connect_adr));

    connect_adr.sin_addr.s_addr = INADDR_BROADCAST;
    connect_adr.sin_port = htons (CONNECT_PORT_NUM);
    connect_adr.sin_family = AF_INET;

    if (bind(connect_fd, (struct sockaddr *) &connect_adr, sizeof (connect_adr)) < 0)
        print_error (-bad_bind);

    int connect_msg = PORT_NUM;
    if (sizeof (connect_msg) != sendto (connect_fd, &connect_msg, sizeof (connect_msg), 0, 
                                    (struct sockaddr *) &connect_adr, sizeof (connect_adr)))
        print_error (-bad_send);

    close (connect_fd);
}