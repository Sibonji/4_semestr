#ifndef INTEGRAL_H
#define INTEGRAL_H

#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <stdlib.h>
#include <sched.h>
#include <math.h>

#define PAGE_SIZE 4096
#define NON_DATA_SIZE sizeof (double) * 2 + sizeof (int)
const int RANGE = 15120;
const double DELTA = 0.002;

typedef struct _Thread_info {
    double lim_inf;
    double lim_sup;
    int thread_num;
    double res[(PAGE_SIZE - NON_DATA_SIZE) / sizeof (double)];
} Thread_info;

int create_threads (int thread_quant, pthread_t* threads, Thread_info* threads_info);
int empty_threads_create (int empty_threads_quant, pthread_t** empty_thread, int threads_quant);
void* start_thread (void* data);
double count_res (int threads_quant, pthread_t* thread, Thread_info* threads_info);
double count_func (double x);
double count_int (Thread_info* thread_info);
double integral_func (int thread_quant);

#endif