//
// Created by ilya on 18.05.18.
//

#include <iostream>
#include <csignal>
#include <unistd.h>
#include <map>
#include <set>
#include "raii_socket.h"
#include "raii_epoll.h"
#include "utils.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::to_string;
using std::map;
using std::set;

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

map<int, string> cur_remainder;
map <int, vector<string>> responses;

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
                epoll.add_new_event(client_fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR);
                cur_remainder[client_fd] = "";
            }
            else if (event.events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                if (event.events & EPOLLERR)
                {
                    cout << "Error on client occured, closing..." << endl;
                }
                int fd = event.data.fd;

                if (cur_remainder.count(fd) > 0)
                {
                    cur_remainder.erase(fd);
                }

                if (responses.count(fd) > 0)
                {
                    responses.erase(fd);
                }

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
                raii_socket not_closable(fd);
                string remainder = cur_remainder[fd];
                read_result res = read_until_crlf(not_closable, remainder);
                if (!res.empty_input)
                {
                    cur_remainder[fd] = res.remainder;

                    string request = res.querry;
                    cout << "Request received from client with file descriptor "
                            + to_string(fd) + ": " + request << endl;

                    string response = "Hello, " + request;

                    if (responses.count(fd) == 0)
                    {
                        responses[fd] = vector<string>();
                        epoll.modify_event(fd, EPOLLOUT | EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR);
                    }
                    responses[fd].push_back(response);
                }
            }
            else if (event.events & EPOLLOUT)
            {
                int fd = event.data.fd;
                cout << "Client with file descriptor " + to_string(fd) + " is ready for writing" << endl;
                raii_socket not_closable(fd);

                string response = responses[fd].back();
                responses[fd].pop_back();
                not_closable.send(response + "\r\n");

                cout << "Response sent to client with file descriptor "
                        + to_string(fd) + ": " + response << endl;

                if (responses[fd].empty())
                {
                    responses.erase(fd);
                    epoll.modify_event(fd, EPOLLIN | EPOLLRDHUP | EPOLLHUP | EPOLLERR);
                }
            }
        }
    }

    exit(EXIT_SUCCESS);

}
