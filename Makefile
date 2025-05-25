CC = gcc
CFLAGS = -I. -I/opt/homebrew/opt/sdl2/include/SDL2 -I/opt/homebrew/opt/sdl2_ttf/include/SDL2
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2 -L/opt/homebrew/opt/sdl2_ttf/lib -lSDL2_ttf

SRC = main.c config/config.c ui/ui.c ui/swipe.c audio/audio.c comm/pc_comm.c
OBJ = $(SRC:.c=.o)
TARGET = pideck

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o config/*.o ui/*.o audio/*.o comm/*.o

deps:
	brew install sdl2 sdl2_ttf || echo "Pour Linux : sudo apt-get install libsdl2-dev libsdl2-ttf-dev"

.PHONY: all clean deps
