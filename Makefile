CC=gcc
INPUT=./example.c
OUTPUT=./exc

SDL2_FLAGS=-lSDL2

RMCOMMAND=rm -f

.PHONY: sdl2 run clean

sdl2: clean
	$(CC) -o $(OUTPUT) $(INPUT) $(SDL2_FLAGS)

run:
	$(OUTPUT)

clean:
	$(RMCOMMAND) $(OUTPUT)
