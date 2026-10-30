CC=gcc
INPUT=./example.c
OUTPUT=./exc

SDL2_FLAGS=-lSDL2 -lSDL2_ttf

RMCOMMAND=rm -f

.PHONY: sdl2 run clean all

sdl2: clean
	$(CC) -o $(OUTPUT) ./examples/$(FILE).c $(SDL2_FLAGS)

all: clean
	$(CC) -o $(OUTPUT) ./examples/$(FILE).c

run:
	$(OUTPUT)

clean:
	$(RMCOMMAND) $(OUTPUT)
