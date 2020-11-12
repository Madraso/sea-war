BIN := net/server
SOURCES := net/server.c
BIN1 := net/client
SOURCES1 := net/client.c
BIN2 := logics/log
SOURCES2 := logics/main.c
CC := gcc
.PHONY := clean
$(BIN): $(SOURCES)
		$(CC) $(SOURCES) -o $(BIN) -g
		$(CC) $(SOURCES1) -o $(BIN1) -pthread -g
		$(CC) $(SOURCES2) -o $(BIN2) -g

clean:
		rm -rf $(BIN)
		rm -rf $(BIN1)
		rm -rf $(BIN2)
