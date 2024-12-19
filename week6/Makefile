all: ex_fdfind ex_ls ex_df

ex_fdfind: ex_fdfind.o
	gcc -O -o ex_fdfind ex_fdfind.o
ex_ls: ex_ls.o
	gcc -O -o ex_ls ex_ls.o
ex_df: ex_df.o
	gcc -O -o ex_df ex_df.o

ex_fdfind.o: ex_fdfind.c
	gcc -O -c -o ex_fdfind.o ex_fdfind.c
ex_ls.o: ex_ls.c
	gcc -O -c -o ex_ls.o ex_ls.c
ex_df.o: ex_df.c
	gcc -O -c -o ex_df.o ex_df.c

clean:
	rm -f *.o ex_fdfind ex_ls ex_df
