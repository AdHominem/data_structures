all: 3_3 3_4 3_5 3_6 3_7

3_3: 3_3.c
	gcc -std=c99 3_3.c -o 3_3

3_4: 3_4.c
	gcc -std=c99 3_4.c -o 3_4

3_5: 3_5.c
	gcc -std=c99 3_5.c -o 3_5

3_6: 3_6.c
	gcc -std=c99 3_6.c -o 3_6

3_7: 3_7.c
	gcc -std=c99 3_7.c -o 3_7

clean:
	del 3_3 3_4 3_5 3_6 3_7