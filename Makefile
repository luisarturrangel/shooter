CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
LDFLAGS = $(shell pkg-config --cflags --libs sdl3)
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
EXEC = game

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
