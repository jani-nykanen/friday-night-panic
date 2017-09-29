SRCDIR := "./src"
OBJDIR := "./obj"

SRCS := $(shell find $(SRCDIR) -name "*.c")
OBJ_FILES := $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
LD_FLAGS := -lSDL2 -lSDL2_image -ltmx -lxml2 -lz -lm
CC_FLAGS := -Wall

game: $(OBJ_FILES)
	gcc -Wall -o $@ $^ $(LD_FLAGS)

obj/%.o: src/%.c
	gcc $(CC_FLAGS) -c -o $@ $<
