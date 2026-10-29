CC=gcc
INPUT=./example.c
OUTPUT=./exc

GUI_FLAGS=-lSDL2
TUI_FLAGS=-lncurses

RMCOMMAND=rm -f

.PHONY: gui tui run clean

gui: clean
	$(CC) -o $(OUTPUT) $(INPUT) $(GUI_FLAGS)
	
tui: clean
	$(CC) -o $(OUTPUT) $(INPUT) $(TUI_FLAGS)

run:
	$(OUTPUT)

clean:
	$(RMCOMMAND) $(OUTPUT)
