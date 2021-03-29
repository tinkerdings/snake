# Makefile template

INCLDIR = ./include
CC = g++
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)
CFLAGS += -g
LINK = -lSDL2 -lSDL2_image -lSDL2_ttf# libs
CFLAGS += $(LINK)
OUT = f_run#executable

OBJDIR = ./obj

_DEPS = # header files
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = main.o window.o render.o inputHandler.o game.o snake.o stateHandler.o# obj files
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
