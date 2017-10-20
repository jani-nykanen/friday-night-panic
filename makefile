SRCDIR := "./src"
OBJDIR := "./obj"

# SRCS := src\main.c src\title.c src\vpad.c src\global.c src\intro.c src\pause.c src\engine\app.c src\engine\assets.c src\engine\audio.c src\engine\bitmap.c src\engine\controls.c src\engine\frame.c src\engine\graphics.c src\engine\list.c src\engine\scene.c src\engine\sprite.c src\engine\tilemap.c src\game\game.c src\game\player.c src\game\hud.c src\game\objcontrol.c src\game\obstacle.c  src\game\stage.c src\game\status.c
# OBJ_FILES := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
# LD_FLAGS := -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -mwindows -lm
# #  -ltmx -lxml2 -lz
# CC_FLAGS := -Wall -O3
#
# game.exe: $(OBJ_FILES)
# 	i686-w64-mingw32-g++ $(CC_FLAGS) -o $@ $^ $(LD_FLAGS)
#
# obj/%.o: src/%.c
#	i686-w64-mingw32-g++ $(CC_FLAGS) -c -o $@ $<

SRCDIR := "./src"
OBJDIR := "./obj"

SRCS := $(shell find $(SRCDIR) -name "*.c")
OBJ_FILES := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
LD_FLAGS := -lSDL2 -lSDL2_image -lSDL2_mixer -lm
#  -ltmx -lxml2 -lz
CC_FLAGS := -Wall

game: $(OBJ_FILES)
	gcc -Wall -o $@ $^ $(LD_FLAGS)

obj/%.o: src/%.c
	gcc $(CC_FLAGS) -c -o $@ $<
