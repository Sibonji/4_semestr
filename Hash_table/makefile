CC = gcc
CFLAGS = -DTEST -MMD -MP

SRC = Hash_table.c
OBJ = Hash_table.o
RES = Hash_table

TEST_SRC = Test_hash_table.c
TEST_OBJ = Test_hash_table.o
TEST_RES = Test_hash_table

COVFILE = Hash_cover
COVDIR = report

test: $(TEST_OBJ) $(OBJ)
	$(CC) $(TEST_OBJ) $(OBJ) -lgcov -o $(TEST_RES)

$(TEST_OBJ): $(TEST_SRC)
	$(CC) $(CFLAGS) -c $(TEST_SRC)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) --coverage -c $(SRC)

code_cov:
	./$(TEST_RES)
	lcov -t $(COVFILE) -o $(COVFILE).info -c -d .
	genhtml -o $(COVDIR) $(COVFILE).info

mem_test:
	valgrind --leak-check=full -s ./$(TEST_RES)

clean:
	rm -rf *.o Hash_table Test_hash_table *.info *.gcno *.gcda *.d report *.txt Test/*.o
