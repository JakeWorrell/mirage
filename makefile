# A simple Makefile for compiling small SDL projects

# set the compiler
CC=g++
ZZ=z80asm
ZFLAGS=-b -orom.bin rom/rom.z80

# set the compiler flags
CFLAGS=`sdl2-config --libs --cflags` -ggdb3 -O0 --std=c++11 -Wall -lSDL2_image -lm
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
	rm -f rom.bin
	rm -f roms/*.o

roms:
	$(ZZ) $(ZFLAGS)


.PHONY: all clean



