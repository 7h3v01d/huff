all:
	gcc -Wall -Wextra -std=c99 -ohuff huff.c example.c

clean:
	rm -f huff

distclean: clean
	rm -f output.dat

demo: all
	rm -f output.dat
	./huff -c huff output.dat
