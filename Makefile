CC = gcc
CFLAGS = -Wall -Wextra -g -I./include
LDFLAGS = $(shell pkg-config --cflags --libs sdl3 sdl3-ttf) -lm

SRC = $(wildcard src/**/*.c src/*.c)

OBJ = $(SRC:src/%.c=build/obj/%.o)

EXEC = build/game

all: $(EXEC)

$(EXEC): $(OBJ)
	@mkdir -p build
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/obj $(EXEC)

.PHONY: all clean
