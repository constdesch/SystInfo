CC=gcc
AR=ar
CFLAGS=-g -Wall -W -I/usr/include/SDL -pthread
LDFLAGS=
SRC=fractal.c tools.c
OBJ=$(SRC:.c=.o)
LIBFILE=libfractal.a
all: $(SRC) $(LIBFILE)
$(LIBFILE): $(OBJ)
	$(AR) r $@ $(OBJ)
*.o:
	$(CC) -o $@ -c $< $(CFLAGS)
clean: $(LIBFILE) $(OBJ)
	rm $(LIBFILE) $(OBJ)
