
CC = g++
CFLAGS = -Wall -std=c++17

SRC = main.cpp
EXE = snake

INC_DIRS = -Isrc/include
LIB_DIRS = -Lsrc/lib

LIBS = -lmingw32 -lSDL2main -lSDL2 -g
DBG = gdb


all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(INC_DIRS) $(LIB_DIRS) $(SRC) -o $(EXE) $(LIBS)

run:
	./$(EXE)

debug:
	$(DBG) ./$(EXE)

clean:
	rm -f $(EXE)
