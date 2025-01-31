CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBS = -lmicrohttpd
TARGET = Server
SRC = server.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
