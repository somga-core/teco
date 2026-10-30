CC=gcc
INPUT=./example.c
OUTPUT=./exc

RMCOMMAND=rm -f

.PHONY: sdl2 run clean all

sdl2: clean
	$(CC) -o $(OUTPUT) ./examples/$(FILE).c -lSDL2 -lSDL2_ttf -lm

all: clean
	$(CC) -o $(OUTPUT) ./examples/$(FILE).c

run:
	$(OUTPUT)

clean:
	$(RMCOMMAND) $(OUTPUT)
