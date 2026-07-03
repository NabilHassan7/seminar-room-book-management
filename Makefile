CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
SRC = src/main.c
OUT = build/seminar_system

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	./$(OUT)

clean:
	rm -f $(OUT)