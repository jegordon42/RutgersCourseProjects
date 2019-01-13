all: sort

sort: simpleCSVsorter.c
	gcc -Wall -Werror -fsanitize=address simpleCSVsorter.c -o simpleCSVsorter

clean:
	rm -rf simpleCSVsorter
