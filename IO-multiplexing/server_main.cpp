//
// Created by ilya on 18.05.18.
//

#include <iostream>
#include <csignal>
#include <unistd.h>
#include "raii_socket.h"
#include "raii_epoll.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::to_string;

const int SERVER_SIZE = 10000;

raii_socket listener{};
raii_epoll epoll;
bool need_to_terminate = false;

void sigint_handler(int signum)
{
    cout << "SIGINT catched, exiting...";
    listener.close();
    epoll.close();
    need_to_terminate = true;
}


int main(int argc, char* argv[])
{
    signal(SIGINT, &sigint_handler);
    if (argc != 2)
    {
        cout << "Running: server <port to bind on>" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Welcome to yet another implementation of hello server!" << endl;
    int porn_num = atoi(argv[1]);
    listener.create();
    listener.bind(static_cast<uint16_t>(porn_num));
    listener.listen();

    epoll.create(SERVER_SIZE);
    epoll.add_listener(listener);


    while (!need_to_terminate)
    {
        vector<epoll_event> events = epoll.await();
        for (epoll_event const& event: events)
        {
            //cout << event.data.fd << " " << (event.events & EPOLLRDHUP)<< endl;
            if (event.data.fd == listener.get_socket_descriptor())
            {
                int client_fd = listener.accept();
                cout << "Connected new client, file descriptor number is " +
                        to_string(client_fd) << endl;
                epoll.add_for_reading(client_fd);
            }
            else if (event.events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                if (event.events & EPOLLERR)
                {
                    cout << "Error on client occured, closing..." << endl;
                }
                int fd = event.data.fd;
                cout << "Closing client " + to_string(fd) << endl;
                if (close(fd) == -1)
                {
                    perror("Cannot close client");
                    exit(EXIT_FAILURE);
                }
            }
            else if(event.events & EPOLLIN)
            {
                int fd = event.data.fd;
                cout << "Client with file descriptor " + to_string(fd) + " is ready for reading" << endl;

            }
        }
    }

    exit(EXIT_SUCCESS);

}
