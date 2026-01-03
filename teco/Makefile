CC=g++
INPUT=./example.cpp
OUTPUT=./exc

GUI_FLAGS=-lSDL2 -lSDL2_ttf 
TUI_FLAGS=-lncurses

TECO_ENGINE=./teco_engine.cpp
TECO_GUI=./teco_gui.cpp
TECO_TUI=./teco_tui.hpp

RMCOMMAND=rm -f

.PHONY: all gui tui run clean

all: clean 
	$(CC) -o $(OUTPUT) $(INPUT) $(TECO_ENGINE) $(TECO_GUI) $(TECO_TUI) $(GUI_FLAGS) $(TUI_FLAGS)

gui: clean
	$(CC) -o $(OUTPUT) $(INPUT) $(TECO_ENGINE) $(TECO_GUI) $(GUI_FLAGS)

tui: clean
	$(CC) -o $(OUTPUT) $(INPUT) $(TECO_ENGINE) $(TECO_TUI) $(TUI_FLAGS)

run:
	$(OUTPUT)

clean:
	$(RMCOMMAND) $(OUTPUT)
