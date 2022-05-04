CC = gcc

TESTS_SRC = Tests.c
TESTS_OBJ = Tests.o
TESTS_RES = Tests

all: $(TESTS_OBJ)
	$(CC) $(TESTS_OBJ) -o $(TESTS_RES)
	./$(TESTS_RES)

$(TESTS_OBJ): $(TESTS_SRC)
	$(CC) -c $(TESTS_SRC)

clean:
	rm -rf *.o Tests
