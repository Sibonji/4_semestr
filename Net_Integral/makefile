CC = gcc

LIB_SRC  = LIB/lib.c
LIB_OBJ  = lib.o

SERVER_SRC = SERVER/server.c
SERVER_OBJ = server.o
SERVER_RES = server

CLIENT_SRC = CLIENT/client.c
CLIENT_OBJ = client.o
CLIENT_RES = client

ERR_SRC = LIB/err.c
ERR_OBJ = err.o

all: $(SERVER_OBJ) $(LIB_OBJ) $(ERR_OBJ) $(CLIENT_OBJ)
	$(CC) $(SERVER_OBJ) $(ERR_OBJ) $(LIB_OBJ) -o $(SERVER_RES)
	$(CC) $(CLIENT_OBJ) $(ERR_OBJ) $(LIB_OBJ) -o $(CLIENT_RES)

serverex: $(SERVER_OBJ) $(LIB_OBJ) $(ERR_OBJ)
	$(CC) $(SERVER_OBJ) $(ERR_OBJ) $(LIB_OBJ) -o $(SERVER_RES)

clientex: $(CLIENT_OBJ) $(LIB_OBJ) $(ERR_OBJ)
	$(CC) $(CLIENT_OBJ) $(ERR_OBJ) $(LIB_OBJ) -o $(CLIENT_RES)

$(CLIENT_OBJ): $(CLIENT_SRC)
	$(CC) -c $(CLIENT_SRC)

$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) -c $(SERVER_SRC)

$(LIB_OBJ): $(LIB_SRC)
	$(CC) -c $(LIB_SRC)

$(ERR_OBJ): $(ERR_SRC)
	$(CC) -c $(ERR_SRC)

clean:
	rm -rf *.o */*.o */*.d $(CLIENT_RES) $(SERVER_RES)
