CC=gcc

.PHONY: saok2c

saok2c: file_to_array.c
	$(CC) -o $@ $<

