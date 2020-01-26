# Install
BIN = demo
CC = clang

ifeq ($(shell uname -s), Darwin)
	SDL2_DIR = $(HOME)/.brew/Cellar/sdl2/2.0.10/lib
	SDL2_INC_DIR = $(HOME)/.brew/Cellar/sdl2/2.0.10/include
	GLEW_DIR = $(HOME)/.brew/Cellar/glew/2.1.0_1/lib
	GLEW_INC_DIR = $(HOME)/.brew/Cellar/glew/2.1.0_1/include
else
	SDL2_DIR =.
	SDL2_INC_DIR =.
endif


# Flags
CFLAGS += -std=c99 -pedantic -O2
CFLAGS += -I$(SDL2_INC_DIR)/SDL2 -I$(SDL2_INC_DIR)
CFLAGS += -I$(GLEW_INC_DIR)/GL -I$(GLEW_INC_DIR)

SRC = main.c
OBJ = $(SRC:.c=.o)

ifeq ($(OS),Windows_NT)
BIN := $(BIN).exe
LIBS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lm -lGLU32 -lGLEW32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LIBS = -L$(SDL2_DIR) -lSDL2 -L$(GLEW_DIR) -framework OpenGL -lm -lGLEW
	else
		LIBS = -lSDL2 -lGL -lm -lGLU -lGLEW
	endif
endif

$(BIN):
#	@mkdir -p bin
#	rm -f bin/$(BIN) $(OBJS)
	$(CC) $(SRC) $(CFLAGS) -o $(BIN) $(LIBS)
