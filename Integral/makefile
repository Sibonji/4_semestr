CC = gcc -pthread
CFLAGS = -g

SRC = integral.c
OBJ = integral.o

all: $(OBJ) 
	$(CC) $(OBJ) -o integral -lm

$(OBJ): $(SRC)
	$(CC) -MMD -MP -c $(SRC)

clear:
	rm -rf integral *.o *.d
