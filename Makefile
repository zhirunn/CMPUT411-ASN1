OS := $(shell uname -s)

ifeq ($(OS), Linux)
  FLAGS = -I/usr/include -L/usr/lib -L/usr/lib/nvidia-340 -lglut -lGLEW -lGL -lGLU -lX11 -Wno-write-strings
endif

all:
	g++ main.cpp -o modelViewer $(FLAGS)

clean:
	-rm -f *.o modelViewer core