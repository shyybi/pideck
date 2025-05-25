CC = gcc
CFLAGS = -I. -I/opt/homebrew/opt/sdl2/include/SDL2
LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2

SRC = main.c config/config.c ui/ui.c audio/audio.c comm/pc_comm.c
OBJ = $(SRC:.c=.o)
TARGET = pideck

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o config/*.o ui/*.o audio/*.o comm/*.o

.PHONY: all clean
