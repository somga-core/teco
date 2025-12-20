CC=g++
FLAGS=-lSDL2 -lSDL2_ttf -lncurses
INPUT=./example.cpp ./teco_engine.cpp ./teco_gui.cpp ./teco_tui.hpp
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
