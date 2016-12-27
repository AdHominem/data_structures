all: 2_2 2_3 2_5

2_2: 2_2.c
    gcc -std=c99 2_2.c -o 2_2

2_3: 2_3.c
    gcc -std=c99 2_3.c -o 2_3

2_5: 2_5.c
    gcc -std=c99 2_5.c -o 2_5

clean:
    rm 2_2 2_3 2_5