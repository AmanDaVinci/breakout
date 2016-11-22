breakout: breakout.c Makefile
	clang -ggdb3 -O0 -std=c99 -Wall -Werror -Wno-unused-variable -o breakout breakout.c -lcs -lm

clean:
	rm -f *.o core breakout
