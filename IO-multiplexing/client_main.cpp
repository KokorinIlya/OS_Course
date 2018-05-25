//
// Created by ilya on 19.05.18.
//

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include "raii_socket.h"
#include "utils.h"
#include "raii_epoll.h"

using std::cout;
using std::cin;
using std::getline;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::vector;

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        cout << "Running:" << endl << "client <address to connect> <port to connect>" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "type empty string, or ctrl+D for exiting" << endl;

    string address_string{argv[1]};
    address_string += ".";
    cout << address_string << endl;
    vector<string> parts_of_address;
    size_t last_pos = 0;
    size_t cur_pos;
    while ((cur_pos = address_string.find('.', last_pos)) != string::npos)
    {
        parts_of_address.push_back(address_string.substr(last_pos, cur_pos - last_pos));
        last_pos = cur_pos + 1;
    }

    if (parts_of_address.size() != 4)
    {
        cout << "Invalid IPv4 address" << endl;
        return EXIT_FAILURE;
    }

    uint32_t reverse_address = 0;

    for (size_t i = 0; i < parts_of_address.size(); ++i)
    {
        uint32_t cur_byte = static_cast<uint32_t>(stoul(parts_of_address[i]));
        reverse_address += (cur_byte << (8 * (parts_of_address.size() - i - 1)));
    }
    uint32_t address = htonl(reverse_address);

    uint16_t port_num = static_cast<uint16_t>(std::stoul(argv[2]));

    raii_socket socket{};
    socket.create();
    socket.connect(address, port_num);

    bool need_to_terminate = false;
    raii_epoll epoll;

    epoll.create(2);
    epoll.add_new_event(STDIN_FILENO, EPOLLIN);
    epoll.add_new_event(socket.get_socket_descriptor(), EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN);

    vector <string> requests;
    string remainder;

    size_t not_answered = 0;

    while (!need_to_terminate || not_answered > 0)
    {
        vector<epoll_event> events = epoll.await();

        for (const epoll_event& event: events)
        {
            if (event.data.fd == STDIN_FILENO)
            {
                string request;
                getline(cin, request);
                if (request.empty())
                {
                    need_to_terminate = true;
                    break;
                }
                //cout << request << endl;
                if (requests.empty())
                {
                    epoll.modify_event(socket.get_socket_descriptor(),
                                       EPOLLOUT | EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN);
                }
                requests.push_back(request);
            }
            else if (event.events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                if (event.events & EPOLLERR)
                {
                    cout << "Error on server occurred, closing..." << endl;
                }
                cout << "Closing socket..." << endl;
                socket.close();

                need_to_terminate = true;
                break;
            }
            else if (event.data.fd == socket.get_socket_descriptor())
            {
                if (event.events & EPOLLIN)
                {
                    read_result r = read_until_crlf(socket, remainder);
                    remainder = r.remainder;
                    if (r.empty_input)
                    {
                        continue;
                    }
                    string response = r.querry;
                    cout << "Response received: " + response << endl;
                    not_answered--;
                }
                if (event.events & EPOLLOUT)
                {
                    string request = requests.back();
                    requests.pop_back();
                    if (requests.empty())
                    {
                        epoll.modify_event(socket.get_socket_descriptor(),
                                           EPOLLRDHUP | EPOLLHUP | EPOLLERR | EPOLLIN);
                    }
                    socket.send(request + "\r\n");
                    not_answered++;
                }
            }
        }
    }

    exit(EXIT_SUCCESS);
}
