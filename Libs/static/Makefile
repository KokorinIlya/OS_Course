.PHONY: all clean

all: static_lib.a

static_lib.a: print_sum.o add_numbers.o
	ar rcs static_lib.a $^
	ranlib static_lib.a

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

clean:
	rm -rf static_lib.a *.o


