all:second

first: second.c second.h
	gcc -Wall -Werror -fsanitize=address second.c -o second

clean:
	rm -rf second 
