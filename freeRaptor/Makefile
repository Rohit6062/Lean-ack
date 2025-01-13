run : test
	./test

test: test.c raptor10.o gf2matrix.o raptor_consts.o queue.o
	gcc -std=c11 -I. -L. -Wall -Wextra -g -o test test.c raptor10.o raptor_consts.o gf2matrix.o queue.o -lm

raptor10.o: raptor10.c raptor_consts.c
	gcc -c -g -fPIC raptor_consts.c raptor10.c -I. -lm

gf2matrix.o: gf2matrix.c
	gcc -c -g -fPIC gf2matrix.c -I. -lm

stack.o: stack.c 
	gcc -c -g -fPIC stack.c -I -lm

queue.o: queue.c 
	gcc -c -g -fPIC queue.c -I -lm

raptor_consts.o:raptor_consts.c  
	gcc -c -g -fPIC raptor_consts.c i_ -lm

clean:
	rm test *.o
