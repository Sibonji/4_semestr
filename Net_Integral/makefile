CC = gcc

LIB_SRC  = LIB/lib.c
LIB_OBJ  = lib.o

SERVER_SRC = SERVER/server.c
SERVER_OBJ = server.o
SERVER_RES = server

CLIENT_SRC = CLIENT/client.c
CLIENT_OBJ = client.o

serverex: $(SERVER_OBJ) $(LIB_OBJ)
	$(CC) $(SERVER_OBJ) $(LIB_OBJ) -o $(SERVER_RES)

$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) -c $(SERVER_SRC)

$(LIB_OBJ): $(LIB_SRC)
	$(CC) -c $(LIB_SRC)

clean:
	rm -rf *.o */*.o */*.d
