all: algorithm.o
	gcc algorithm.o

algorithm.o: algorithm.c
	gcc -c algorithm.c

run:
	./a.out

clean:
	rm *.o
	rm ./a.out
