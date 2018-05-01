CC=gcc
AR=ar
CFLAGS=-g -Wall -W -I/usr/include/SDL
LDFLAGS=
SRC=fractal.c tools.c
OBJ=$(SRC:.c=.o)
LIBFILE=libfractal.a

all: $(SRC) $(LIBFILE)

$(LIBFILE): $(OBJ)

	$(AR) r $@ $(OBJ)

*.o:
	$(CC) -o $@ -c $< $(CFLAGS)

gcc -c fractal.c -o fractal.o
gcc -c main.c -o main.o
gcc main.o fractal.o -o build