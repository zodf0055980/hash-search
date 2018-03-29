test:test.o
	gcc -Wall -o test test.o

test.o: %.o: %.c
	gcc -Wall -c $< -o $@

clean:
	rm -rf *.o test
