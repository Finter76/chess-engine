CC = gcc
SRCS = src/board.c src/eval.c src/movegen.c src/search.c src/uci.c 

CFLAGS_COMMON = -std=c11 -Wall -Wextra -Wshadow -Wconversion -pedantic
CFLAGS_RELEASE = $(CFLAGS_COMMON) -O2
CFLAGS_DEBUG = $(CFLAGS_COMMON) -O0 -g3 -fsanitize=address,undefined -fno-omit-frame-pointer -fno-inline -DDEBUG
MKDIR = mkdir -p build

all:
	$(MKDIR)
	$(CC) $(CFLAGS_RELEASE) main.c $(SRCS) -o build/chess

debug:
	$(MKDIR)
	$(CC) $(CFLAGS_DEBUG) main.c $(SRCS) -o build/chess
perft:
	$(MKDIR)
	$(CC) $(CFLAGS_DEBUG) tests/perft.c $(SRCS) -o build/perft
    
clean:
	rm -rf build

.PHONY: all clean debug perft
