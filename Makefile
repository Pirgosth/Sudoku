CC = g++
CFLAGS = -Wall

SOURCES = main.cpp game.cpp
OBJECTS = $(SOURCES:.cc=.o)
SDL = -lSDL2 -lSDL2_image -lSDL2_ttf -lSprite

sudoku: $(OBJECTS)
	$(CC) -no-pie $(CFLAGS) -o $@ $^ $(SDL)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $@ $(SDL)
