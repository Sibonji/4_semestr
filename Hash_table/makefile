CC = gcc
CFLAGS = -o1

SRC = Hash_table.c
OBJ = Hash_table.o
RES = Hash_table

TEST_SRC = Tests/Tests.c
TEST_OBJ = Tests/Tests.o
TEST_RES = Tests/Tests

all: $(OBJ)
	$(CC) $(OBJ) -o $(RES)

$(OBJ): $(SRC)
	$(CC) -c $(SRC)

clean:
	rm -rf *.txt *.o Hash_table
