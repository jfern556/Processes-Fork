#makefile - file name

CC = gcc
FILE = process.c
OUT_EXE = process.out

build:  $(FILE)	
	$(CC) $(FILE) -o $(OUT_EXE)

clean:  rm -rf *o $(OUT_EXE)

rebuild: clean build




