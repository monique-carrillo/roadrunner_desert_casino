CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: rdc

rdc: rdc.cpp log.cpp mcarrillo.cpp dbenavides.cpp jgaribay.cpp jchicas.cpp
	g++ $(CFLAGS) rdc.cpp log.cpp mcarrillo.cpp dbenavides.cpp jgaribay.cpp jchicas.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o rdc

clean:
	rm -f rdc
	rm -f *.o
