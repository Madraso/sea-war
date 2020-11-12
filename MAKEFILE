BIN := /net/server
SOURCES := /net/server.c
BIN1 := net/client
SOURCES1 := /net/client.c
CC := gcc
.PHONY := clean
$(BIN): $(SOURCES)
		$(CC) $(SOURCES) -o $(BIN) -g
		$(CC) $(SOURCES1) -o $(BIN1) -pthread -g

clean:
		rm -rf $(BIN)
		rm -rf $(BIN1)