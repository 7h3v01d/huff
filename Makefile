all:
	gcc -Wall -Wextra -ggdb -std=c99 -ohuff huff.c example.c

clean:
	rm -f huff

distclean: clean
	rm -f encoded.dat decoded.dat

demo: all
	rm -f encoded.dat decoded.dat
	./huff -c huff encoded.dat
	./huff -d encoded.dat decoded.dat
	@echo
	@diff -s huff decoded.dat || echo
	@ls -lA huff decoded.dat encoded.dat
