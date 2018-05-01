all: exec

exec: fractal.c lib
	gcc -o main main.c libfraction/libfractal.a -Ilibfractal/-ILDS

lib:
	cd libfractal && $(MAKE) clean && $(MAKE)

clean:
	rm main
	cd libfractal && $(MAKE) clean
