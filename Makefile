CC=g++
FLAGS=-lSDL2 -lSDL2_ttf -lncurses
INPUT=./example.cpp ./teco.cpp
OUTPUT=./exc

RMCOMMAND=rm -f

.PHONY: clean build

all: clean build

build:
	$(CC) -o $(OUTPUT) $(INPUT) $(FLAGS)

run: all
	$(OUTPUT)

clean:
	$(RMCOMMAND) $(OUTPUT)
