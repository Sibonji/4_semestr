#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>

#define poison 0xDEADBEEF

typedef struct _Data_set {
    char* word;
    long int hash_num;
    struct _Data_set* next_elem;
}Data_set;

typedef struct _Hash_table {
    int size;
    Data_set* elem; //Переименовать имя на ячейку (на английском)
}Hash_table;

long int count_hash (char* login);
int create_table (Hash_table* table, int _size);
void elem_initialize (Data_set* _elem);
int print_error (int err_num);
int delete_table (Hash_table *table);
void rec_free (Data_set *elem);

int add_elem (Hash_table* table, char* elem_name);
int find_empty (Data_set* elem, long int _hash_num, char* login);

int get_elem (Hash_table* table, char* login);
int find_elem (Data_set* elem, long int _hash_num, char* login);

int rm_elem (Hash_table* table, char* login);
void* rec_rm (Data_set* elem, long int _hash_num, char* login, int* status);

int check_table (Hash_table* table, char* login);

void foreach (Hash_table* table, void (*func) (Data_set* elem, void* ), void* data);
void print_elem (Data_set* elem, void* log_file);

#endif