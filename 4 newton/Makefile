#files
INCLS = 
OBJS = main.c

#compiler
CC = gcc

#flags
COMPILER_FLAGS = -Wall

#libraries
LINKER_FLAGS = -lraylib -lraylib  -lGL -lm -ldl -lrt -lX11

#compiled file name
OBJ_NAME = compiled

#wallah
all : $(OBJS)
	$(CC) $(INCLS) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
