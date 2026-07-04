CC = gcc

CFLAGS = -Wall -Wextra -std=c17 -Iinclude

SRC = \
	src/core/main.c \
	src/core/file.c \
	src/core/auth.c \
	src/core/menu.c \
	src/modules/book.c \
	src/modules/student.c \
	src/modules/transaction.c \
	src/modules/fine.c \
	src/modules/report.c \
	src/system/date.c

OUT = build/seminar_system.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run:
	.\$(OUT)

clean:
	del /Q build\*.exe 2>nul