#define DEBUG

#include "Hash_table.h"

#ifdef DEBUG

void dump_table (Hash_table *table);
void dump_elem (Data_set *elem, int num, int index, FILE* fp);
void go_through_list (Data_set *elem, int num, int index, FILE* fp);

void dump_table (Hash_table *table) {
    FILE* fp;
    fp = fopen ("log.txt", "w");
    for (int i = 0; i < table -> size; i++) {
        int num = 0;
        if (&table -> elem[i] != NULL) go_through_list (&table -> elem[i], num, i, fp);
    }
}

void go_through_list (Data_set *elem, int num, int index, FILE* fp) {
    dump_elem (elem, num, index, fp);

    if (elem -> next_elem != NULL) go_through_list(elem -> next_elem, ++num, index, fp);

    return; 
}

void dump_elem (Data_set *elem, int num, int index, FILE* fp) {
    fprintf (fp, "Element[%d] -> %d: %p\n", index, num, elem);
    fprintf (fp, "    word: %s\n", elem -> word);
    fprintf (fp, "    hash_num: %ld\n", elem->hash_num);
    fprintf (fp, "    next_elem: %p\n", elem -> next_elem);
}

#endif

long int count_hash (char* login) {
    long int hash_num = 0;
    long int pow = 1;

    if (strlen(login) <= 0) {
        return -zero_login;
    }

    for (int i = 0; i < strlen (login); i++) {
        hash_num += (long int)(login[i]) * pow;
        pow *= 2;
    }
        
    return hash_num;
}

int delete_table (Hash_table* table) {
    if (table -> elem == NULL) return -zero_pointer;

    for (int i = 0; i < table -> size; i++) {
        if (table -> elem[i].next_elem != NULL) rec_free (table -> elem[i].next_elem);
    }

    free (table -> elem);

    return success;
}

void rec_free (Data_set* elem) {
    if (elem -> next_elem != NULL) rec_free (elem -> next_elem);
    if (elem != NULL) free(elem);

    return;
}

int create_table (Hash_table* table, int _size) {
    if (_size <= 0) {
        table -> elem = NULL;
        return -bad_size;
    }

    table -> size = _size;

    table -> elem = (Data_set *) calloc (_size, sizeof (Data_set));
    if (table -> elem == NULL) return -bad_alloc;
    
    for (int i = 0; i < _size; i++) {
        elem_initialize (&table -> elem[i]);
    }
    
    return success;
}

void elem_initialize (Data_set* _elem) {    
    _elem -> hash_num = poison;
    _elem -> next_elem = NULL;
    _elem -> word = NULL;
    
    return; 
}

int add_elem (Hash_table* table, char* login) {
    long int _hash_num = count_hash (login);
    if (_hash_num < 0) return _hash_num; //_hash_num will define error number
    
    int cell_num = _hash_num % table -> size;
    int err = 0;

    if (table -> elem == NULL) return -zero_pointer;
    err = find_empty (&table -> elem[cell_num], _hash_num, login);
    if (err != success) return err;

    return success;
}

int find_empty (Data_set* elem, long int _hash_num, char* login) {
    if (elem -> hash_num == poison) {
        elem -> word = (char*) calloc (strlen (login), sizeof (char));
        memcpy (elem -> word, login, sizeof (char) * strlen (login));

        elem -> hash_num = _hash_num;
    }
    else if (elem -> hash_num = _hash_num && strcmp (elem -> word, login) == 0) {
        return -elem_exist;
    }
    else if (elem -> next_elem == NULL) {
        elem -> next_elem = (Data_set*) calloc (1, sizeof (Data_set));
        if (elem -> next_elem == NULL) return -bad_alloc;

        elem_initialize (elem -> next_elem);
        find_empty (elem -> next_elem, _hash_num, login);
    }
    else
        return find_empty (elem -> next_elem, _hash_num, login);

    return success;
}

int get_elem (Hash_table* table, char* login) {
    long int _hash_num = count_hash (login);
    if (_hash_num < 0) return _hash_num; //_hash_num will define error number
    
    int cell_num = _hash_num % table -> size;

    if (table -> elem == NULL) return -zero_pointer;
    
    return find_elem (&table -> elem[cell_num], _hash_num, login);
}

int find_elem (Data_set* elem, long int _hash_num, char* login) {
    if (elem -> hash_num = _hash_num && strcmp (elem -> word, login) == 0) {
        return found;
    }
    else if (elem -> next_elem == NULL) {
        return -not_exist;
    }
    else 
        return find_elem (elem -> next_elem, _hash_num, login);
}

int rm_elem (Hash_table* table, char* login) {
    long int _hash_num = count_hash (login);
    if (_hash_num < 0) return _hash_num; //_hash_num will define error number
    
    int cell_num = _hash_num % table -> size;

    int err = success;
    if (table -> elem == NULL) return -zero_pointer;
    
    Data_set* save_elem;
    save_elem = (Data_set*) rec_rm (&table -> elem[cell_num], _hash_num, login, &err);
    if (err == found) {
        table -> elem[cell_num].word = NULL;
        table -> elem[cell_num].hash_num = poison;
        table -> elem[cell_num].next_elem = save_elem;

        err = success;
    }
    
    return err;
}

void* rec_rm (Data_set* elem, long int _hash_num, char* login, int* status) {
    if (elem -> hash_num = _hash_num && strcmp (elem -> word, login) == 0) {
        free (elem -> word);
        *status = found;
        return elem -> next_elem;
    }
    else if (elem -> next_elem == NULL) {
        *status = -not_exist;
        return NULL;
    }
    else {
        Data_set* save_elem = (Data_set*) rec_rm (elem -> next_elem, _hash_num, login, status);
        if (*status == found) {
            free (elem -> next_elem);
            elem -> next_elem = save_elem;
            *status = success;
            return NULL;
        }
        else return NULL;
    }
}

void print_error (int err_num) {
    if (err_num == success || err_num == found) return;

    fprintf (stderr, "Error occured during programm execution!\n");

    switch (err_num) {
        case -zero_login: 
            fprintf (stderr, "Login is empty!\n");
            break;

        case -zero_pointer:
            fprintf (stderr, "Pointer has NULL value!\n");
            break;

        case -bad_size:
            fprintf (stderr, "Incorrect size of table!\n");
            break;

        case -elem_exist:
            fprintf (stderr, "Element alreade exist in table!\n");
            break;

        case -bad_alloc:
            fprintf (stderr, "Couldn't allocate the memory!\n");
            break;

        case -not_exist:
            fprintf (stderr, "Couldn't find this element in the table!\n");
            break;

        default:
            fprintf (stderr, "Undefined error!\n");
            break;
    }
}

int main () {
    Hash_table table;
    /*print_error (create_table (&table, 10));
    print_error (add_elem (&table, "fi"));
    print_error (add_elem (&table, "si"));
    print_error (add_elem (&table, "hi"));
    print_error (rm_elem (&table, "si"));*/

    print_error (create_table (&table, 50));
    srand (time (NULL));
    char array[12];
    int size = 0;
    for (int i = 0; i < 150; i++) {
        size = rand () % 10 + 2;
        for (int j = 0; j < size; j++) {
            array[j] = rand () % 20 + 'a';
        }
        array[size] = '\0';
        print_error (add_elem (&table, array));
    }

    #ifdef DEBUG
    dump_table (&table);
    #endif

    print_error (delete_table (&table));

    return 0;
}