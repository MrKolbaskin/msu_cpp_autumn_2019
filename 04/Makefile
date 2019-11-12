all: prog

prog: main BigInt
	g++ -o my_prog 04.o BigInt.o

main: 04.cpp
	g++ 04.cpp -c -std=c++14 -O2

BigInt: BigInt.cpp
	g++ BigInt.cpp -c --std=c++14 -O2

run: prog
	./my_prog

clean:
	rm -rf  *.o my_prog
