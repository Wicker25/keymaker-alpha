linenoise: linenoise.h linenoise.c
	$(CC) -Wall -W -Os -c -fPIC -o linenoise.o linenoise.c

clean:
	rm -f linenoise.o
