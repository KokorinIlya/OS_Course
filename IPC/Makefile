.PHONY: all clean

all: server client

%.o: %.cpp
	g++ -std=c++17 -c -o $@ $^

server: server_main.o raii_pipe.o unix_socket.o utils.o
	g++ -std=c++17 -o server $^

client: client_main.o unix_socket.o utils.o
	g++ -std=c++17 -o client $^

run-server: server 
	./server $(SOCK_NAME)

run-client: client 
	./client $(SOCK_NAME) $(A) $(B)

clean:
	rm -rf server client *.o