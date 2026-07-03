CC = gcc

CFLAGS = -Wall -Wextra -std=c17 -Iinclude

SRC = \
	src/core/main.c \
	src/core/file.c

OUT = build/seminar_system.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	.\$(OUT)

clean:
	del /Q build\*.exe 2>nul