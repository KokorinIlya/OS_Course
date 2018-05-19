.PHONY: all clean

all: server stupid_client

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

server: server_main.o raii_socket.o raii_epoll.o utils.o
	g++ -std=c++17 -o server $^

stupid_client: stupid_client_main.o raii_socket.o utils.o
	g++ -std=c++17 -o stupid_client $^

run-server: server 
	./server $(PORT)

clean:
	rm -rf server *.o