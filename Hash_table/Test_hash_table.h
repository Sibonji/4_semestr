#ifndef TEST_HASH_TABLE
#define TEST_HASH_TABLE

#define TEST_TABLE_MAX_SIZE 128
#define MAX_CMD_SIZE 12
#define LOGIN_MAX_SIZE 12
#define HASH_TABLE_SIZE 500

typedef struct _Test_elem {
    char* login;
    int valid;
} Test_elem;

typedef struct _Test_table {
    int max_size;
    int cur_size;
    Test_elem* arr;
} Test_table;

void* fp_open ();
int file_tests ();
int check_elem (FILE* fp, Test_table* test_table, Hash_table* hash_table);
int file_test_cycle (Test_table* test_table, Hash_table* hash_table, FILE* tests);
int test_error (int err_name);

int rm_from_test_table (Test_table* table, char* word);
int add_to_test_table (Test_table* test_table, char* word);
int find_in_test_table (Test_table* test_table, char* word);

int create_test_table (Test_table* table);
int destroy_test_table (Test_table* table);
void free_mem (Test_table* test_table, Hash_table* hash_table, FILE* tests);

int errors_test ();
int print_func_test ();

#endif