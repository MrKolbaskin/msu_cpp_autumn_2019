all: prog

prog: 04.cpp BigInt.h
	g++ 04.cpp -o my_prog -std=c++14 -O2

run: prog
	./my_prog 

test: 04.cpp 
	./my_prog

clean:
	rm -rf my_prog
