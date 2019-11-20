all: prog

prog:
	g++ 06.cpp -o my_prog -std=c++14 -O2

main: 05.cpp
	g++ 06.cpp -c -std=c++14 -O2

run: prog
	./my_prog 

clean:
	rm -rf my_prog
