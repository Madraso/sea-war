BIN := net/server
SOURCES := net/server.c
BIN1 := net/client
SOURCES1 := net/client.c
BIN2 := interface/srs/graphic
SOURCES2 := interface/src/graphic.c
CC := gcc
.PHONY := clean
$(BIN): $(SOURCES)
		$(CC) $(SOURCES) -o $(BIN) -g
		$(CC) $(SOURCES1) -o $(BIN1) -pthread -g
		$(CC) $(SOURCES2) -o $(BIN2) -g
		$(CC) $(SOURCES3) -o $(BIN3) -lncurses -g

clean:
		rm -rf $(BIN)
		rm -rf $(BIN1)
		rm -rf $(BIN2)
		rm -rf $(BIN3)
