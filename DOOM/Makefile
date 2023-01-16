# Install

BIN = Game
CC = clang
INC = includes/
OBJ_PATH = obj

SDL2_V = 2.26.2

ifeq ($(shell uname -s), Darwin)
	SDL2_DIR = /usr/local/Cellar/sdl2/$(SDL2_V)/lib
	SDL2_INC_DIR = /usr/local/Cellar/sdl2/$(SDL2_V)/include
else
	SDL2_DIR =.
	SDL2_INC_DIR =.
endif


# Flags
CFLAGS += -g -fsanitize=address
CFLAGS += -std=c99 -pedantic -O2
CFLAGS += -I$(SDL2_INC_DIR)/SDL2 -I$(SDL2_INC_DIR)

SRC = game.c

# PANEL = tool_panel.c\
# 	list_panel.c\
# 	map_panel.c\
# 	wave.c\
# 	edit_panel.c\
# 	sector_panel.c

# SAVE = save.c \
# 	load.c

# EXPORT = export.c\
# 		assets.c\
# 		map.c

OBJ = $(addprefix $(OBJ_PATH)/,$(SRC:.c=.o))
# OBJ += $(addprefix $(OBJ_PATH)/,$(PANEL:.c=.o))
# OBJ += $(addprefix $(OBJ_PATH)/,$(SAVE:.c=.o))
# OBJ += $(addprefix $(OBJ_PATH)/,$(EXPORT:.c=.o))

ifeq ($(OS),Windows_NT)
BIN := $(BIN).exe
LIBS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 #-lm #-lGLU32 -lGLEW32
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LIBS = -L$(SDL2_DIR) -lSDL2 #-framework OpenGL #-lm #-lGLEW
	else
		LIBS = -lSDL2 #-lGL #-lm #-lGLU -lGLEW
	endif
endif

#COLOR OUTPUT OPIONS
RED		=\033[0;31m
GREEN	=\033[0;32m
NC		=\033[0m

all: $(BIN)

re: clean all

$(BIN): $(OBJ)
	@mkdir -p maps
#	@mkdir -p bin
#	rm -f bin/$(BIN) $(OBJS)
#	@printf "[compile   src]\t[#######   ]\r" #print statement
	@$(CC) $(OBJ_PATH)/*.o $(CFLAGS) -o $(BIN) $(LIBS)
	@printf "[$(GREEN)  complete!  $(NC)]\t[##########]\n" #print statement

$(OBJ_PATH):
	@printf "[create    obj]\t[#         ]\r" #print statement
	@mkdir -p $@

$(OBJ_PATH)/%.o: src/%.c $(INC)/*.h| $(OBJ_PATH)
	@printf "[compile   obj]\t[##        ]\r" #print statement
	@$(CC) -I$(INC) $(CFLAGS) -o $@ -c $<
# $(OBJ_PATH)/%.o: src/panel/%.c $(INC)/*.h | $(OBJ_PATH)
# 	@printf "[compile   obj]\t[####      ]\r" #print statement
# 	@$(CC) -I$(INC) $(CFLAGS) -o $@ -c $<
# $(OBJ_PATH)/%.o: src/save_state/%.c $(INC)/*.h | $(OBJ_PATH)
# 	@printf "[compile   obj]\t[######    ]\r" #print statement
# 	@$(CC) -I$(INC) $(CFLAGS) -o $@ -c $<
# $(OBJ_PATH)/%.o: src/export/%.c $(INC)/*.h | $(OBJ_PATH)
# 	@printf "[compile   obj]\t[########  ]\r" #print statement
# 	@$(CC) -I$(INC) $(CFLAGS) -o $@ -c $<

clean:
	@printf "[$(RED)remove    obj$(NC)]\t[####      ]\r" #print statement
	@rm -f $(OBJ_PATH)/*.o
	@rm -rf $(OBJ_PATH)
	@printf "[$(RED)remove binary$(NC)]\t[######    ]\r" #print statement
	@rm -rf $(BIN)
	@rm -rf $(BIN).dSYM
	@printf "[$(RED)full    clean$(NC)]\t[##########]\n" #print statement
