.PHONY: all clean

all: utility

utility: main.o ../static/static_lib.a ../shared/shared_lib.so
	g++ -std=c++17 -L. -l:../static/static_lib.a -l:../shared/shared_lib.so -Wl,-rpath,. -o $@ $^ -ldl

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

../static/static_lib.a:
	./build_static.sh

../shared/shared_lib.so:
	./build_shared.sh

clean:
	rm -rf utility *.o


