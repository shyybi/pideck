UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    SDL2_CFLAGS = -I/opt/homebrew/opt/sdl2/include/SDL2
    SDL2_TTF_CFLAGS = -I/opt/homebrew/opt/sdl2_ttf/include/SDL2
    SDL2_LDFLAGS = -L/opt/homebrew/opt/sdl2/lib -lSDL2
    SDL2_TTF_LDFLAGS = -L/opt/homebrew/opt/sdl2_ttf/lib -lSDL2_ttf
else
    SDL2_CFLAGS = -I/usr/include -I/usr/include/SDL2
    SDL2_TTF_CFLAGS = -I/usr/include/SDL2
    SDL2_LDFLAGS = -lSDL2
    SDL2_TTF_LDFLAGS = -lSDL2_ttf
endif

CC = gcc
CFLAGS = -I. -I./dep/SDL2 $(SDL2_CFLAGS) $(SDL2_TTF_CFLAGS)
LDFLAGS = $(SDL2_LDFLAGS) $(SDL2_TTF_LDFLAGS)

SRC = main.c src/*/*.c
OBJ = $(SRC:.c=.o)
TARGET = pideck

all: deps $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o src/*/*.o

deps:
ifeq ($(UNAME_S),Darwin)
	brew install sdl2 sdl2_ttf
else
	sudo apt-get install -y libsdl2-dev libsdl2-ttf-dev
endif

.PHONY: all clean deps
