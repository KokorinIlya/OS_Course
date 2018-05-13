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
    string connection_message = read_until_crlf(socket, "").querry;
    cout << "Connection message: " << connection_message << endl;
    socket.send("client some-client-info\r\n");
    string client_message = read_until_crlf(socket, "").querry;
    cout << "Client info message: " << client_message << endl;
}

string send_trivial_request(dict_socket& socket, string const& request)
{
    socket.send(request);
    cout << "Request sent" << endl;
    return read_until_crlf(socket, "").querry;
}

string get_request(vector<string> const& v)
{
    string res;
    for (const auto& s: v)
    {
        res += (s + " ");
    }
    res.pop_back();
    return res + "\r\n";
}

void process_request(dict_socket& socket, string const& request)
{
    vector<string> parts = parse_querry(request);
    string true_request = get_request(parts);

    if (parts.size() == 2 && parts[0] == "show" &&
            (parts[1] == "db" || parts[1] == "databases"))
    {
        cout << "SHOW DB request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else if (parts.size() == 2 && parts[0] == "show" &&
             (parts[1] == "strat" || parts[1] == "strategies"))
    {
        cout << "SHOW STRAT request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else if (parts.size() == 3 && parts[0] == "show" &&
             parts[1] == "info")
    {
        cout << "SHOW INFO request" << endl;
        cout << true_request << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else if (parts.size() == 4 && parts[0] == "match")
    {
        cout << "MATCH request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else if (parts.size() == 2 && parts[0] == "show" &&
             parts[1] == "sever")
    {
        cout << "SHOW SERVER request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else if (parts.size() == 1 && parts[0] == "status")
    {
        cout << "STATUS request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else if (parts.size() == 1 && parts[0] == "help")
    {
        cout << "HELP request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else if (parts.size() == 1 && parts[0] == "quit")
    {
        cout << "QUIT request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
    else
    {
        cout << "Warning: Unknown request" << endl;
        cout << "Response received: " << send_trivial_request(socket, true_request) << endl;
    }
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
        std::transform(request.begin(), request.end(), request.begin(), ::tolower);

        std::replace(request.begin(), request.end(), '\t', ' ');

        process_request(socket, request);
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

