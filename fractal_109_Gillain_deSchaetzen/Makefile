all: exerce

exerce: main.c libfract consumer_producer.o
	gcc -o main main.c consumer_producer.o -g libfractal/libfractal.a -Ilibfractal/ -pthread -lSDL

libfract:
	cd libfractal && $(MAKE) clean && $(MAKE)

consumer_producer.o: consumer_producer.c consumer_producer.h
	gcc consumer_producer.c -c

test:
	./main tests/input2.txt fileout -d

clean:
	rm *.o
	rm main
	cd libfractal && $(MAKE) clean

cleanbmp:
	rm -f *.bmp

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./main --maxthreads 8 tests/file.txt fileout
