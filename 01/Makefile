all: prog

prog: 01.cpp
	g++ 01.cpp -o my_prog -std=c++14 -O2

run: prog
	./my_prog 

test: test.py 
	python test.py

clean:
	rm -rf my_prog
