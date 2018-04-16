.PHONY: all clean

all: JitCompiler

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

JitCompiler: compiler.o
	g++ -std=c++17 -o JitCompiler $^
	
clean:
	rm -rf JitCompiler *.o


