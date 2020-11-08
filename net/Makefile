BIN := server
SOURCES := server.c
BIN1 := client
SOURCES1 := client.c
CC := gcc
.PHONY := clean
$(BIN): $(SOURCES)
		$(CC) $(SOURCES) -o $(BIN) -g
		$(CC) $(SOURCES1) -o $(BIN1) -pthread -g

clean:
		rm -rf $(BIN)
		rm -rf $(BIN1)