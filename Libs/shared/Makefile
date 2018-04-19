.PHONY: all clean

all: shared_lib.so

%.o: %.cpp
	g++ -std=c++17 -fPIC -c -o $@ $^

shared_lib.so: print_sub.o sub_numbers.o
	g++ -std=c++17 -shared -o shared_lib.so $^

clean:
	rm -rf shared_lib.so *.o


