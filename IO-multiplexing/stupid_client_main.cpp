//
// Created by ilya on 18.05.18.
//

#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "raii_socket.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

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

    raii_socket socket{};
    socket.create();
    socket.connect(address, 2628);

    cout << "Before sleep" << endl;
    usleep(5000 * 1000);
    cout << "After sleep" << endl;

    exit(EXIT_SUCCESS);
}
