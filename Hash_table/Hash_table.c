#include "Enum.h"
#include "Hash_table.h"

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
    if (table == NULL) return -zero_pointer; //tests done
    if (table -> elem == NULL) return -zero_pointer; //tests done

    for (int i = 0; i < table -> size; i++) {
        if (table -> elem[i].next_elem != NULL) rec_free (table -> elem[i].next_elem);
        if (table -> elem[i].hash_num != poison) free (table -> elem[i].word);
    }
    
    free (table -> elem);

    return success;
}

void rec_free (Data_set* elem) {
    if (elem -> next_elem != NULL) rec_free (elem -> next_elem);
    if (elem != NULL) {
        free (elem -> word);
        free (elem);
    }

    return;
}

int create_table (Hash_table* table, int _size) {
    if (_size <= 0) return -bad_size; //tests done
    if (table == NULL) return -zero_pointer; //tests done

    table -> size = _size;

    table -> elem = (Data_set *) calloc (_size, sizeof (Data_set));
    if (table -> elem == NULL) return -bad_alloc; //Unable to do test for this!
    
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

int check_table (Hash_table* table, char* login) {
    if (table == NULL) return -zero_pointer; //tests done
    else if (login == NULL) return -zero_pointer; //tests done
    else if (table -> size <= 0) return -bad_size; //tests done
    else if (table -> elem == NULL) return -zero_pointer; //tests done

    return success;
}

int add_elem (Hash_table* table, char* login) {
    int err = 0;
    if ((err = check_table (table, login)) != success) return err; //tests_done
    
    long int _hash_num = count_hash (login);
    if (_hash_num < 0) return _hash_num; //_hash_num will define error number; tests done
    
    int cell_num = _hash_num % table -> size;

    err = find_empty (&table -> elem[cell_num], _hash_num, login);
    if (err != success) return err;

    return success;
}

int find_empty (Data_set* elem, long int _hash_num, char* login) {
    if (elem -> hash_num == poison) {
        elem -> word = (char*) calloc (strlen (login) + 1, sizeof (char));
        memcpy (elem -> word, login, sizeof (char) * strlen (login));

        elem -> hash_num = _hash_num;
    }
    else if ((elem -> hash_num == _hash_num) && (strcmp (elem -> word, login) == 0)) {
        return -elem_exist; //tests done
    }
    else if (elem -> next_elem == NULL) {
        elem -> next_elem = (Data_set*) calloc (1, sizeof (Data_set));
        if (elem -> next_elem == NULL) return -bad_alloc; //unable to do tests

        elem_initialize (elem -> next_elem);
        find_empty (elem -> next_elem, _hash_num, login);
    }
    else
        return find_empty (elem -> next_elem, _hash_num, login);

    return success; //tests done
}

int get_elem (Hash_table* table, char* login) {
    int err = 0;
    if ((err = check_table (table, login)) != success) return err;

    long int _hash_num = count_hash (login);
    if (_hash_num == -zero_login) return _hash_num; //_hash_num will define error number; tests done
    
    int cell_num = _hash_num % table -> size;
    
    return find_elem (&table -> elem[cell_num], _hash_num, login);
}

int find_elem (Data_set* elem, long int _hash_num, char* login) {
    if (elem -> hash_num != poison) {
        if ((elem -> hash_num = _hash_num) && (strcmp (elem -> word, login) == 0)) {
            return found; //tests done
        }
    }
    if (elem -> next_elem == NULL) {
        return -not_exist; //tests done
    }
    else 
        return find_elem (elem -> next_elem, _hash_num, login);
}

int rm_elem (Hash_table* table, char* login) {
    int err = 0;
    if ((err = check_table (table, login)) != success) return err; //tests done

    long int _hash_num = count_hash (login);
    if (_hash_num < 0) return _hash_num; //_hash_num will define error number; tests done
    
    int cell_num = _hash_num % table -> size;
    
    Data_set* save_elem;
    save_elem = (Data_set*) rec_rm (&table -> elem[cell_num], _hash_num, login, &err);
    if (err == found) { //tests done
        table -> elem[cell_num].word = NULL;
        table -> elem[cell_num].hash_num = poison;
        table -> elem[cell_num].next_elem = save_elem;

        err = success;
    }
    
    return err;
}

void* rec_rm (Data_set* elem, long int _hash_num, char* login, int* status) {
    /*if (elem -> word == NULL && elem -> ) {
        *status = -not_exist; //tests done
        return NULL;
    }*/
    if (elem -> hash_num != poison) {
        if (elem -> hash_num = _hash_num && strcmp (elem -> word, login) == 0) {
            free (elem -> word);
            elem -> word = NULL;
            *status = found; //tests done
            return elem -> next_elem;
        }
    }
    if (elem -> next_elem == NULL) {
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

int print_error (int err_num) {
    if (err_num == success || err_num == found) return err_num;

    fprintf (stderr, "Error occured during programm execution!\n");

    switch (err_num) {
        case -zero_login: 
            fprintf (stderr, "Login is empty!\n\n");
            break;

        case -zero_pointer:
            fprintf (stderr, "Pointer has NULL value!\n\n");
            break;

        case -bad_size:
            fprintf (stderr, "Incorrect size of table!\n\n");
            break;

        case -elem_exist:
            fprintf (stderr, "Element alreade exist in table!\n\n");
            break;

        case -bad_alloc:
            fprintf (stderr, "Couldn't allocate the memory!\n\n");
            break;

        case -not_exist:
            fprintf (stderr, "Couldn't find this element in the table!\n\n");
            break;

        default:
            fprintf (stderr, "Undefined error!\n\n");
            err_num = -undef_cmd_name;
            break;
    }

    return err_num;
}