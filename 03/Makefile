all: prog

prog: 03.cpp matrix.h
	g++ 03.cpp -o my_prog -std=c++14 -O2

run: prog
	./my_prog 

test: 03.cpp 
	./my_prog

clean:
	rm -rf my_prog
