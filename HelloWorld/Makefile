.PHONY: all clean

all: helloWorld

%.0: %.c
	gcc -c -o $@ $^ 

helloWorld: main.o helloImpl.o
	gcc -o helloWorld $^

clean: 
	rm -rf helloWorld *.o
