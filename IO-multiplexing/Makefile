.PHONY: all clean

all: server client

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

server: server_main.o raii_socket.o raii_epoll.o utils.o
	g++ -std=c++17 -o server $^

client: client_main.o raii_socket.o raii_epoll.o utils.o
	g++ -std=c++17 -o client $^

run-server: server 
	./server $(PORT)

run-client: client
	./client $(ADDRESS) $(PORT)

clean:
	rm -rf server client *.o