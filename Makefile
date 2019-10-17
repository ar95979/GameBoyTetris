CC	= ../../../bin/lcc -Wa-l -Wl-m

BINS	= main.gb

all:	$(BINS)

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

%.gb:	%.o
	$(CC) -o $@ $<

clean:
	rm -f *.o *.lst *.map *.gb

# Link file, and write 0x80 at position 0x143 in header
main.gb:	main.o
	$(CC) -Wl-yp0x143=0x80 -o main.gb main.o
