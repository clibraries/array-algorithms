.PHONY: clean test valgrind

test-array-alg: tests.c impl.c defs.h ../array_alg.h
	gcc -O2 tests.c impl.c -o $@

test: test-array-alg
	./test-array-alg

valgrind: test-array-alg
	valgrind --tool=exp-sgcheck ./test-array-alg 

clean:
	rm -f test-array-alg


