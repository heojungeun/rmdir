.PHONY: all clean

all: rmdir

clean:
	rm -f rmdir

rmdir: rmdir.c
	gcc -O2 -pipe -o $@ $^
