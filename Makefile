CC=clang
CFLAGS=-Weverything

main:
	$(CC) $(CFLAGS) main.c -g -o links
