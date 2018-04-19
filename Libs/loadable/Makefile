.PHONY: all clean

all: loadable_lib.so

%.o: %.cpp
	g++ -std=c++17 -fPIC -c -o $@ $^

loadable_lib.so: print_mul.o mul_numbers.o
	g++ -std=c++17 -shared -o loadable_lib.so $^

clean:
	rm -rf loadable_lib.so *.o


