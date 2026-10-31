CC=gcc
INPUT=./example.c
OUTPUT=./exc

RMCOMMAND=rm -f

.PHONY: ncurses sdl2 run clean all

sdl2: clean
	$(CC) -o $(OUTPUT) ./examples/$(FILE).c -lSDL2 -lSDL2_ttf -lm

ncurses: clean
	$(CC) -o $(OUTPUT) ./examples/$(FILE).c -lncurses -lm

all: clean
	$(CC) -o $(OUTPUT) ./examples/$(FILE).c -lm

run:
	$(OUTPUT)

clean:
	$(RMCOMMAND) $(OUTPUT)
