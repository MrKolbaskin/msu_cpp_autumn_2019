all: prog

prog:
	g++ 05.cpp -o my_prog -std=c++17 -O2

main: 05.cpp
	g++ 05.cpp -c -std=c++17 -O2

run: prog
	./my_prog 

clean:
	rm -rf my_prog
