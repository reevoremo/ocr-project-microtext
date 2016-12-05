## Simple SDL mini code
 
CC=gcc 
CPPFLAGS= `pkg-config --cflags sdl gtk+-2.0`
CFLAGS= -g -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl gtk+-2.0` -lSDL_image -lm
 
SRC= image_blocking.c pixel_operations.c get_char.c neural_netX.c main.c
OBJ= ${SRC:.c=.o}
 
all: main
 
main: ${OBJ}
 
clean:
	rm -f *~ *.o
	rm -f main
 
# END
