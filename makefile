# A simple Makefile for compiling small SDL projects

# set the compiler
CC=g++
# set the compiler flags
CFLAGS=`sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lm
# add header files here
HDRS=
# add source files here
SRCS=main.cpp

# generate names of object files
OBJS=$(SRCS:.cpp=.o)

# name of executable
EXEC=emu

# default recipe
all: $(EXEC)
 
# recipe for building the final executable
$(EXEC): 
	$(CC) -o $@ $(SRCS) $(CFLAGS)

# recipe for building object files
$(OBJS):
	$(CC) -o $@ $(@:.o=.cpp) -c $(CFLAGS)

# recipe to clean the workspace
clean: 
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean



