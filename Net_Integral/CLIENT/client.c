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

    int is_work = 0;

    //recvfrom (connect_fd, &is_work, sizeof (serv_port_num), 0,
      //        (struct sockaddr *) peer_adr, &peer_adr_size);

    close (connect_fd); 

    //printf ("Port num is: %d\n", serv_port_num);       

    return serv_port_num;
}