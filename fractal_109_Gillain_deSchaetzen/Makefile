all: exerce

exerce: main.c libfract consumer_producer.o
	gcc -o main main.c consumer_producer.o -g libfractal/libfractal.a -Ilibfractal/ -pthread -lSDL

libfract:
	cd libfractal && $(MAKE) clean && $(MAKE)

consumer_producer.o: consumer_producer.c consumer_producer.h
	gcc consumer_producer.c -c

test:
	./main input2.txt fileout

clean:
	rm *.o
	rm main
	cd libfractal && $(MAKE) clean
