//
// Created by ilya on 25.05.18.
//

#include <iostream>
#include "unix_socket.h"
#include "utils.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::to_string;

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        cout << "Running:" << endl <<
             "client <name of file for UNIX socket>" <<
             " <first number to add>, <second number to add>" << endl;
        exit(EXIT_FAILURE);
    }

    int a = atoi(argv[2]);
    int b = atoi(argv[3]);

    unix_socket socket;
    socket.create();

    socket.connect(argv[1]);

    int send_to_server_fd = socket.get_file_descriptor();
    int receive_from_server_fd = socket.get_file_descriptor();

    cout << "Received file descriptors: " << to_string(send_to_server_fd) <<
            " " << to_string(receive_from_server_fd) << endl;

    write_int(send_to_server_fd, a);
    write_int(send_to_server_fd, b);
    cout << "Sum of " << std::to_string(a)<<
         " and " << std::to_string(b) << " is " <<
         read_int(receive_from_server_fd) << endl;

    exit(EXIT_SUCCESS);
}