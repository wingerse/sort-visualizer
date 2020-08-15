PROJ_NAME=sort-visualizer
LIBS=-lSDL2 -lSDL2_image -lm
include master-makefile/Makefile
CFLAGS += -Wno-conversion
