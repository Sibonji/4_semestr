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

}