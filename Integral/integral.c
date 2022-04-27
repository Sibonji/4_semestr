#include "Integral.h"

int check_args (int argc, char* argv[], int* err_num) {
    if(argc != 2) {
        *err_num = -incorrect_args;
	    return 0;
	}
	
	char* num_end;
	int biggest = strtol(argv[1], &num_end, 0);
	if (errno == ERANGE) {
        *err_num = -incorrect_args;
	    return 0;
	}

	if (num_end != NULL && *num_end) {
        *err_num = -incorrect_args;
	    return 0;
	}
	
	if (biggest <= 0) {
        *err_num = -incorrect_args;
	    return 0;
	}

    return biggest;
}

void print_error (int err_num) {
    if (err_num == ok) return;
    fprintf (stderr, "Error has occured during programm execution!\n");

    switch (err_num) {
        case -incorrect_args:
            fprintf (stderr, "Incorrect arguments!\n\n");
            break;

        case -bad_alloc:
            fprintf (stderr, "Couldn't allocate the memory!\n\n");
            break;

        case -bad_join:
            fprintf (stderr, "Couldn't execute pthread_join!\n\n");
            break;
        
        case -bad_cr_thread:
            fprintf (stderr, "Couldn't create thread!\n\n");
            break;

        case -bad_set:
            fprintf (stderr, "Set not valid!\n\n");
            break;

        default:
            fprintf(stderr, "Undefined error!\n\n");
            break;
    }

    return;
}

int empty_threads_create (int empty_threads_quant, pthread_t** empty_thread, int threads_quant) {
    if (empty_threads_quant <= 0) return 0;
    *empty_thread = (pthread_t*) calloc (empty_threads_quant, sizeof (pthread_t));
    if (*empty_thread == NULL) return -bad_alloc;

    Thread_info* empty_threads_info = (Thread_info*) calloc (empty_threads_quant, sizeof (Thread_info));
    if (empty_threads_info == NULL) return -bad_alloc;

    for (int i = 0; i < empty_threads_quant; i++) {
        empty_threads_info[i].lim_inf = 0;
        empty_threads_info[i].lim_sup = RANGE / threads_quant;
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

int create_threads (int thread_quant, pthread_t* thread, Thread_info* threads_info) {
    int CPU_quant = get_nprocs ();

    pthread_t* empty_threads = NULL;
    int empty_threads_quant = empty_threads_create (CPU_quant - thread_quant, &empty_threads, thread_quant);
    if (empty_threads < 0) return empty_threads_quant;

    for (int i = 0; i < thread_quant; i++) {
        threads_info[i].lim_inf = i * (RANGE / thread_quant);
        threads_info[i].lim_sup = threads_info[i].lim_inf + RANGE / thread_quant;
        threads_info[i].thread_num = (empty_threads_quant + i) % CPU_quant;

        if (pthread_create (thread + i, NULL, start_thread, threads_info + i) != 0) return -bad_cr_thread;
    }

    for (int i = 0; i < empty_threads_quant; i++) {
        if (pthread_join (empty_threads[i], NULL) != 0) return -bad_join;
    }

    return thread_quant;
}


double count_res (int threads_quant, pthread_t* thread, Thread_info* threads_info) {
    double res = 0;

    for (int i = 0; i < threads_quant; i++) {
        if (pthread_join (thread[i], NULL) != 0) return -bad_join;

        res += threads_info[i].res[0];
    }

    return res;
}

double count_int (Thread_info* thread_info) {
    double res = 0;

    for (double i = thread_info -> lim_inf; i < thread_info -> lim_sup; i += DELTA) {
        res += count_func (i) * DELTA;
    }

    return res;
}

double count_func (double x) {
    return (sin(x));
}

int main (int argc, char* argv[]) {
    int err_num = 0;
    int thread_quant = check_args (argc, argv, &err_num);
    if (thread_quant == 0) {
        print_error (err_num);
        return err_num;
    }

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

    err_num = create_threads (thread_quant, threads, threads_info);
    if (err_num != thread_quant) {
        print_error (err_num);
        return err_num;
    }

    double res = count_res (thread_quant, threads, threads_info);
    if (res == -bad_join) {
        print_error (-bad_join);
        return -bad_join;
    }

    printf ("Res is: %f!\n", res);
    

    free (threads);
    free (threads_info);

    return 0;
}