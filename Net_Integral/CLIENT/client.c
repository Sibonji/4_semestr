#define _GNU_SOURCE

#include "../LIB/lib.h"
#include "../LIB/enum.h"
#include <pthread.h>
#include <sched.h>

int main (int argc, char* argv[]) {
    int thread_quant = check_args (argc, argv);
    if (thread_quant < 0)
        fprintf (stderr, "Incorrect command line arguments!\n");
    else
        while(1)
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
    if (serv_port_num != PORT_NUM)
        print_error (-bad_connect);

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

    Limits int_info;
    if (recv (sender_fd, &int_info, sizeof (int_info), 0) < 0)
        print_error (-bad_recv);

    printf ("Received limits info!\n");

    double res = integral_func (thread_quant, int_info);

    if (send (sender_fd, &res, sizeof (res), MSG_NOSIGNAL) < 0)
        print_error (-bad_send);

    close (sender_fd);

    printf ("Programm finished working!\n");

    return;
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

int empty_threads_create (int empty_threads_quant, pthread_t** empty_thread, int threads_quant, double range) {
    if (empty_threads_quant <= 0) return 0;
    *empty_thread = (pthread_t*) calloc (empty_threads_quant, sizeof (pthread_t));
    if (*empty_thread == NULL) return -bad_alloc;

    Thread_info* empty_threads_info = (Thread_info*) calloc (empty_threads_quant, sizeof (Thread_info));
    if (empty_threads_info == NULL) return -bad_alloc;

    int i = 0;
    for (i = 0; i < empty_threads_quant; i++) {
        empty_threads_info[i].lim_inf = 0;
        empty_threads_info[i].lim_sup = range / threads_quant;
        empty_threads_info[i].thread_num = i;

        if (pthread_create ((*empty_thread) + i, NULL, start_thread, empty_threads_info + i) != 0)
            return -bad_cr_thread;
    }

    return empty_threads_quant;
}

void* start_thread (void* data) {
    Thread_info* thread_info = (Thread_info*) data;
    cpu_set_t cpu_set;

    CPU_ZERO (&cpu_set);
    CPU_SET (thread_info -> thread_num, &cpu_set);

    pthread_setaffinity_np (pthread_self (), sizeof (cpu_set), &cpu_set);

    thread_info -> res[0] = count_int (thread_info);

    return NULL;
}

int create_threads (int thread_quant, pthread_t* thread, Thread_info* threads_info, Limits int_limits) {
    int CPU_quant = get_nprocs ();

    double range = int_limits.up - int_limits.down;

    pthread_t* empty_threads = NULL;
    int empty_threads_quant = empty_threads_create (CPU_quant - thread_quant, &empty_threads, thread_quant, range);
    if (empty_threads < 0) return empty_threads_quant;

    int i = 0;
    for (i = 0; i < thread_quant; i++) {
        threads_info[i].lim_inf = i * (range / thread_quant);
        threads_info[i].lim_sup = threads_info[i].lim_inf + range / thread_quant;
        threads_info[i].thread_num = (empty_threads_quant + i) % CPU_quant;

        if (pthread_create (thread + i, NULL, start_thread, threads_info + i) != 0) return -bad_cr_thread;
    }

    for (i = 0; i < empty_threads_quant; i++) {
        if (pthread_join (empty_threads[i], NULL) != 0) return -bad_join;
    }

    return thread_quant;
}


double count_res (int threads_quant, pthread_t* thread, Thread_info* threads_info) {
    double res = 0;

    int i = 0;
    for (i = 0; i < threads_quant; i++) {
        if (pthread_join (thread[i], NULL) != 0) return -bad_join;

        res += threads_info[i].res[0];
    }

    return res;
}

double count_int (Thread_info* thread_info) {
    double res = 0;
    double i = 0;

    for (i = thread_info -> lim_inf; i < thread_info -> lim_sup; i += DELTA) {
        res += count_func (i) * DELTA;
    }

    return res;
}

double count_func (double x) {
    return (sin(x) * sin(x));
}

double integral_func (int thread_quant, Limits int_limits) {
    int err_num = 0;

    pthread_t* threads = (pthread_t*) calloc (thread_quant, sizeof (pthread_t));
    if (threads == NULL) {
        print_error (bad_alloc);
        return -bad_alloc;
    }

    Thread_info* threads_info = (Thread_info*) calloc (thread_quant, sizeof (Thread_info));
    if (threads_info == NULL) {
        print_error (bad_alloc);
        return -bad_alloc;
    }

    err_num = create_threads (thread_quant, threads, threads_info, int_limits);
    if (err_num != thread_quant) {
        print_error (err_num);
        return err_num;
    }

    double res = count_res (thread_quant, threads, threads_info);
    if (res == -bad_join) {
        print_error (-bad_join);
        return -bad_join;
    }

    //printf ("Res is: %f!\n", res);
    

    free (threads);
    free (threads_info);

    return res;
}