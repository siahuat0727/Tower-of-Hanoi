all: tower_of_hanoi.c
	gcc -Wall -std=c99 -o tower_of_hanoi tower_of_hanoi.c

clean:
	rm tower_of_hanoi

astyle:
	astyle --style=linux --indent=tab --max-code-length=80 --suffix=none tower_of_hanoi.c
