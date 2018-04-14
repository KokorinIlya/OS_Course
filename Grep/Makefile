.PHONY: all clean

all: mygrep

%.o: %.cpp
	g++ -std=c++11 -c -o $@ $^

mygrep: grep.o KMP.o path.o
	g++ -std=c++11 -o mygrep $^

clean: 
	rm -rf mygrep *.o
