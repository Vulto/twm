PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include -L$(PREFIX)/lib -lX11 -o twm twm.c

clean:
	rm -f twm

