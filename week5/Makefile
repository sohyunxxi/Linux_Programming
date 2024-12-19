all: ex_ls ex_dir_cp ex_tree

ex_ls: ex_ls.o
	gcc -O -o ex_ls ex_ls.o
ex_dir_cp: ex_dir_cp.o
	gcc -O -o ex_dir_cp ex_dir_cp.o
ex_tree: ex_tree.o
	gcc -O -o ex_tree ex_tree.o

ex_ls.o: ex_ls.c
	gcc -O -c -o ex_ls.o ex_ls.c
ex_dir_cp.o: ex_dir_cp.c
	gcc -O -c -o ex_dir_cp.o ex_dir_cp.c
ex_tree.o: ex_tree.c
	gcc -O -c -o ex_tree.o ex_tree.c

clean:
	rm -f *.o ex_dir_cp ex_ls ex_tree
