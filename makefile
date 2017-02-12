all: 4_1

4_1: 4_1.c
	gcc -std=c99 4_1_functions.c 4_1_helper.c 4_1.c -o 4_1

clean:
	del 4_1.exe