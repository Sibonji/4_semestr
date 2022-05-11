#include "Enum.h"
#include "Hash_table.h"
#include "Test_hash_table.h"

int test_error (int err_name) {
    if (err_name == success || err_name == test_pass) return err_name;

    switch (err_name)
    {
        case -test_fail:
            fprintf (stderr, "Error: tests failed, fix your table!\n");
            break;

        case -undef_cmd_name:
            fprintf (stderr, "Error: undefined command name, please fix file with tests!\n");
            break;
    
        default:
            print_error (err_name);
            break;
    }

    return err_name;
}

int create_test_table (Test_table* table) {
    if (table == NULL) return -zero_pointer;

    table -> max_size = TEST_TABLE_MAX_SIZE;
    table -> cur_size = 0;
    table -> arr = (Test_elem*) calloc (table -> max_size, sizeof (Test_elem));
    if (table -> arr == NULL) return -bad_alloc;

    for (int i = 0; i < table -> max_size; i++) {
        table -> arr[i].valid = not_valid;
        table -> arr[i].login = NULL;
    }

    return success;
}

int destroy_test_table (Test_table* table) {
    if (table == NULL) return -zero_pointer;

    for (int i = 0; i < table -> max_size; i++) {
        if (table -> arr[i].valid) {
            free (table -> arr[i].login);
        }
    }

    free (table -> arr);

    return success;
}

int add_to_test_table (Test_table* test_table, char* word){
    if (test_table == NULL) return -zero_pointer;

    if (test_table -> cur_size == test_table -> max_size) {
        test_table -> max_size = test_table -> max_size * 2;
        test_table -> arr = (Test_elem*) realloc (test_table -> arr, sizeof (Test_elem) * test_table -> max_size);
        if (test_table -> arr == NULL) return -bad_alloc;

        for (int i = test_table -> cur_size; i < test_table -> max_size; i++) {
            test_table -> arr[i].valid = not_valid;
            test_table -> arr[i].login = NULL;
        }
    }

    for (int i = 0; i < test_table -> max_size; i++) {
        if (test_table -> arr[i].valid == valid) {
            if (strcmp (test_table -> arr[i].login, word) == 0) {
                return -elem_exist;
            }
        }
        else if (test_table -> arr[i].valid != valid) {
<<<<<<< HEAD

=======
            
>>>>>>> 18eff5f6dc5f7c596d4d017419ba92417ac1ff3d
            test_table -> arr[i].login = (char*) calloc (strlen (word) + 1, sizeof (char));
            if (test_table -> arr[i].login == NULL) return -bad_alloc;

            test_table -> arr[i].valid = valid;
            memcpy (test_table -> arr[i].login, word, strlen (word));
            (test_table -> cur_size)++;
            return success;
        }
    }

    return -undef_cmd_name;
}

int find_in_test_table (Test_table* test_table, char* word) {
    if (test_table == NULL) return -zero_pointer;

    for (int i = 0; i < test_table -> max_size; i++) {
        if (test_table -> arr[i].valid == valid) {
            if (strcmp (word, test_table -> arr[i].login) == 0)
                return found;
        }
    }

    return -not_exist;
}

int rm_from_test_table (Test_table* test_table, char* word) {
    if (test_table == NULL) return -zero_pointer;

    for (int i = 0; i < test_table -> max_size; i++) {
        if (test_table -> arr[i].valid == valid) {
            if (strcmp (word, test_table -> arr[i].login) == 0) {
                free (test_table -> arr[i].login);
                test_table -> arr[i].login = NULL;
                test_table -> arr[i].valid = not_valid;

                (test_table -> cur_size)--;

                return success;
            }
        }
    }

    return -not_exist;
}

void* fp_open () {
    FILE* fp = NULL;
    if ((fp = fopen ("Test/Tests.txt", "r")) == NULL) {
        fprintf (stderr, "Couldn't open file to write tests!\n");
        return NULL;
    }

    return fp;
}

int check_elem (FILE* tests, Test_table* test_table, Hash_table* hash_table) {
    char cmd_name = '\0';
    fscanf (tests, "%c", &cmd_name);

    char* word = (char*) calloc (MAX_CMD_SIZE, sizeof (char));
    fscanf (tests, "%s\n", word);
    
    int test_err_num = 0;
    int hash_err_num = 0;

    switch (cmd_name) {
        case 'a':
            hash_err_num = add_elem (hash_table, word);

            test_err_num = add_to_test_table (test_table, word);
            if ((test_err_num == -zero_pointer) || (test_err_num == -bad_alloc)) return test_err_num;
            else if (test_err_num != hash_err_num) return -test_fail;

            break;

        case 'd':
            hash_err_num = rm_elem (hash_table, word);

            test_err_num = rm_from_test_table (test_table, word);
            if (test_err_num == -zero_pointer) return test_err_num;
            else if (test_err_num != hash_err_num) return -test_fail;

            break;

        case 'f':
            hash_err_num = get_elem (hash_table, word);

            test_err_num = find_in_test_table (test_table, word);
            if (test_err_num == -zero_pointer) return test_err_num;
            if (test_err_num != hash_err_num) return -test_fail;

            break;

        default:
            return -undef_cmd_name;
    }
    
    free (word);

    return test_pass;
}

int file_test_cycle (Test_table* test_table, Hash_table* hash_table, FILE* tests) {
    int cmd_quant = 0;
    fscanf (tests, "%d\n", &cmd_quant);

    int err_num = 0;

    for (int i = 0; i < cmd_quant; i++) {
        err_num = check_elem (tests, test_table, hash_table);
        if (err_num != test_pass) return err_num;
    }

    return test_pass;
}

int file_tests () {
    FILE* tests = NULL;
    if ((tests = (FILE*) fp_open ()) == NULL) return -1;

    int err_num = 0;
    Test_table test_table;
    if ((err_num = create_test_table (&test_table)) != success) return err_num;

    Hash_table hash_table;
    if ((err_num = create_table (&hash_table, HASH_TABLE_SIZE)) != success) {
        free_mem (&test_table, &hash_table, tests);
        return err_num;
    }

    printf      ("-------------------------Starting file tests!----------------------------\n");

    if ((err_num = file_test_cycle (&test_table, &hash_table, tests)) != test_pass) {
        free_mem (&test_table, &hash_table, tests);
        return err_num;
    }
    else printf ("--------------------File tests passed successfully!----------------------\n\n");

    free_mem (&test_table, &hash_table, tests);

    printf      ("-------------------------Starting error tests!---------------------------\n");
    if (errors_test () != test_pass) return -test_fail;
    else printf ("-------------------Errors tests passed successfully!---------------------\n\n");

    printf      ("----------------- Starting print_error function tests!-------------------\n");
    if (print_func_test () != test_pass) return -test_fail;
    else printf ("------------Print_error function tests passed successfully!--------------\n\n"); 
    

    return success;
}

void free_mem (Test_table* test_table, Hash_table* hash_table, FILE* tests) {
    delete_table (hash_table);
    destroy_test_table (test_table);

    fclose (tests);
}

int print_func_test () {
    if (print_error (success) != success) return -test_fail;
    if (print_error (found) != found) return -test_fail;
    if (print_error (-zero_login) != -zero_login) return -test_fail;
    if (print_error (-zero_pointer) != -zero_pointer) return -test_fail;
    if (print_error (-bad_size) != -bad_size) return -test_fail;
    if (print_error (-bad_alloc) != -bad_alloc) return -test_fail;
    if (print_error (-elem_exist) != -elem_exist) return -test_fail;
    if (print_error (-not_exist) != -not_exist) return -test_fail;
    if (print_error (-test_fail) != -undef_cmd_name) return -test_fail;

    return test_pass;
}

int errors_test () {
    Hash_table table;
    
    Hash_table bad_table;
    bad_table.elem = NULL;
    bad_table.size = 0;

//                                  Check create_table func

    if (create_table (&table, 0) != -bad_size) {
        delete_table (&table);
        return -test_fail;
    }
    if (create_table (NULL, 1) != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (create_table (&table, 1) != success) {
        delete_table (&table);
        return -test_fail;
    }

//                                    Check add_elem func

    if (add_elem (NULL, "") != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (add_elem (&table, NULL) != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (add_elem (&table, "") != -zero_login) {
        delete_table (&table);
        return -test_fail;
    }
    if (add_elem (&bad_table, "a") != -bad_size) {
        delete_table (&table);
        return -test_fail;
    }
    bad_table.size = 1;
    if (add_elem (&bad_table, "a") != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (add_elem (&table, "a") != success) {
        delete_table (&table);
        return -test_fail;
    }
    if (add_elem (&table, "a") != -elem_exist) {
        delete_table (&table);
        return -test_fail;
    }

//                                    Check rm_elem func
    delete_table (&table);
    create_table (&table, 1);
    bad_table.size = 0;

    if (rm_elem (NULL, "") != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (rm_elem (&table, NULL) != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (rm_elem (&table, "") != -zero_login) {
        delete_table (&table);
        return -test_fail;
    }
    if (rm_elem (&bad_table, "a") != -bad_size) {
        delete_table (&table);
        return -test_fail;
    }
    bad_table.size = 1;
    if (get_elem (&bad_table, "a") != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    add_elem (&table, "a");
    add_elem (&table, "s");
    add_elem (&table, "c");
    add_elem (&table, "d");
    if (rm_elem (&table, "a") != success) {
        delete_table (&table);
        return -test_fail;
    }
    if (rm_elem (&table, "a") != -not_exist) {
        delete_table (&table);
        return -test_fail;
    }
    if (rm_elem (&table, "c") != success) {
        delete_table (&table);
        return -test_fail;
    }
    if (rm_elem (&table, "s") != success) {
        delete_table (&table);
        return -test_fail;
    }

//                                    Check get_elem func
    delete_table (&table);
    create_table (&table, 1);
    bad_table.size = 0;

    if (get_elem (NULL, "") != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (get_elem (&table, NULL) != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (get_elem (&table, "") != -zero_login) {
        delete_table (&table);
        return -test_fail;
    }
    if (get_elem (&bad_table, "a") != -bad_size) {
        delete_table (&table);
        return -test_fail;
    }
    bad_table.size = 1;
    if (get_elem (&bad_table, "a") != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (get_elem (&table, "a") != -not_exist) {
        delete_table (&table);
        return -test_fail;
    }
    add_elem (&table, "a");
    if (get_elem (&table, "a") != found) {
        delete_table (&table);
        return -test_fail;
    }
    if (get_elem (&table, "b") != -not_exist) {
        delete_table (&table);
        return -test_fail;
    }
    add_elem (&table, "b");
    rm_elem (&table, "a");
    if (get_elem (&table, "b") != found) {
        delete_table (&table);
        return -test_fail;
    }
//                                        Check delete_table func

    if (delete_table (NULL) != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }
    if (delete_table (&bad_table) != -zero_pointer) {
        delete_table (&table);
        return -test_fail;
    }

    delete_table (&table);
    return test_pass;
}

int main () {
    if (test_error (file_tests ()) != success) return -1;
    

    return 0;
}