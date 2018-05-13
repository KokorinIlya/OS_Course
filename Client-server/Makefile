.PHONY: all client server clean

all: client server

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

client: client_main.o dict_socket.o utils.o
	g++ -std=c++17 -o client $^

server: server_main.o dict_socket.o utils.o fill_db.o
	g++ -std=c++17 -o server $^

run-client: client 
	./client $(ADDRESS)

run-server: server 
	./server

clean:
	rm -rf client server *.o