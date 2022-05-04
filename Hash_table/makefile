CC = gcc
CFLAGS = -o1

SRC = Hash_table.c
OBJ = Hash_table.o
RES = Hash_table

TEST_SRC = Test_hash_table.c
TEST_OBJ = Test_hash_table.o
TEST_RES = Test_hash_table

all: $(OBJ)
	$(CC) $(OBJ) -o $(RES)

$(OBJ): $(SRC)
	$(CC) -c $(SRC)

test: $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $(TEST_RES)

$(TEST_OBJ):
	$(CC) -c $(TEST_SRC)

clean:
	rm -rf *.txt *.o Hash_table
