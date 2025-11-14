CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c11
TARGET := 3dsnake
SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

SDL_CONFIG_CFLAGS := $(shell sdl2-config --cflags 2>/dev/null)
SDL_CONFIG_LIBS := $(shell sdl2-config --libs 2>/dev/null)

ifeq ($(strip $(SDL_CONFIG_CFLAGS)),)
SDL_CONFIG_CFLAGS := $(shell pkg-config --cflags sdl2 2>/dev/null)
SDL_CONFIG_LIBS := $(shell pkg-config --libs sdl2 2>/dev/null)
endif

SDL_CFLAGS := $(SDL_CONFIG_CFLAGS)
SDL_LIBS := $(SDL_CONFIG_LIBS)

ifeq ($(OS),Windows_NT)
GL_LIBS ?= -lopengl32
else
GL_LIBS ?= -lGL
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_LIBS) $(GL_LIBS) -lm

src/%.o: src/%.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
