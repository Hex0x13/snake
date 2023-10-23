CC = g++
CFLAGS = -Wall -std=c++17

SRC = main.cpp
EXE = snake

INC_DIRS = -Isrc/include
LIB_DIRS = -Lsrc/lib

# Specify the static libraries (replace these with the actual library names)
LIBS = -lmingw32 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2_net -lSDL2main -lSDL2 -static-libgcc -static-libstdc++
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
