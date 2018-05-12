//
// Created by ilya on 12.05.18.
//

#include <cstdio>
#include <cstdlib>
#include <string>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <utility>
#include <algorithm>

#include "constants_and_types.h"
#include "dict_socket.h"
#include "utils.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::pair;

void wait_for_connection(dict_socket& socket)
{
    string connection_message = socket.recieve().first;
    cout << "Connection message: " << connection_message << endl;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Running:" << endl << "client <address to connect>" << endl;
        exit(1);
    }
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

    dict_socket socket;
    try
    {
        socket.create();
        socket.connect(address);
        wait_for_connection(socket);

        cout << "Please, enter your request" << endl;
        string request;
        getline(cin, request);
        request += "\r\n";
        std::transform(request.begin(), request.end(), request.begin(), ::tolower);

        socket.send(request);
        cout << "Request sent" << endl;

        string answer = read_until_crlf(socket, "").first;
        cout << "Response received: " + answer << endl;
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

