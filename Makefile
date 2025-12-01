.PHONY: clean

all: simple_threads

simple_threads: simple_threads.c
	gcc simple_threads.c -lpthread -o simple_threads

debug: simple_threads.c
	gcc simple_threads.c -lpthread -g -o simple_threads

clean:
	rm simple_threads
