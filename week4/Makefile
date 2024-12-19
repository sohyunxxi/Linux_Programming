all: ex_chmod ex_rm ex_mv

ex_chmod: ex_chmod.o
	gcc -O -o ex_chmod ex_chmod.o
ex_rm: ex_rm.o
	gcc -O -o ex_rm ex_rm.o
ex_mv: ex_mv.o
	gcc -O -o ex_mv ex_mv.o

ex_chmod.o: ex_chmod.c
	gcc -O -c -o ex_chmod.o ex_chmod.c
ex_rm.o: ex_rm.c
	gcc -O -c -o ex_rm.o ex_rm.c
ex_mv.o: ex_mv.c
	gcc -O -c -o ex_mv.o ex_mv.c

clean:
	rm -f *.o ex_chmod ex_rm ex_mv
