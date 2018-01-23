all: tower_of_hanoi.c
	gcc -Wall -std=c99 -o tower_of_hanoi tower_of_hanoi.c

clean:
	rm tower_of_hanoi
