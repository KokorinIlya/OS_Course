//
// Created by ilya on 25.05.18.
//

#include <iostream>
#include <vector>
#include <unistd.h>
#include <csignal>
#include <sys/stat.h>
#include <fcntl.h>
#include "unix_socket.h"
#include "raii_pipe.h"
#include "utils.h"

using std::cout;
using std::endl;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::to_string;

bool need_to_terminate = false;

void sigint_handler(int signum)
{
    cout << "SIGINT catched, exiting..." << endl;
    need_to_terminate = true;
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{

    struct sigaction a{};
    a.sa_handler = sigint_handler;
    a.sa_flags = 0;
    sigemptyset( &a.sa_mask );
    sigaction(SIGINT, &a, nullptr);

    if (argc != 2)
    {
        cout << "Running:" << endl <<
             "server <name of file for UNIX socket>" << endl;
        exit(EXIT_FAILURE);
    }

    // if file exits, unlink it
    struct stat stat_s{};
    if (stat(argv[1], &stat_s) != -1)
    {
        if (unlink(argv[1]) == -1)
        {
            perror("Cannot unlink file for UNIX socket");
            exit(EXIT_FAILURE);
        }
    }

    unix_socket server_socket;
    server_socket.create();
    server_socket.bind(argv[1]);
    server_socket.listen();

    while (!need_to_terminate)
    {
        cout << "Awaiting connection" << endl;
        unix_socket client_socket = server_socket.accept();
        cout << "New client connected" << endl;
        raii_pipe client_to_server_pipe;
        raii_pipe server_to_client_pipe;
        client_to_server_pipe.create();
        server_to_client_pipe.create();

        client_socket.send_file_descriptor(client_to_server_pipe.get_fds().second);
        client_socket.send_file_descriptor(server_to_client_pipe.get_fds().first);

        cout << "File descriptors sent: " <<
             to_string(client_to_server_pipe.get_fds().second) <<
             " " << to_string(server_to_client_pipe.get_fds().first) << endl;

        int write_fd = server_to_client_pipe.get_fds().second;
        int read_fd = client_to_server_pipe.get_fds().first;

        int first = read_int(read_fd);
        int second = read_int(read_fd);

        cout << "Request: " << std::to_string(first) <<
             " " << std::to_string(second) << endl;

        int sum = first + second;

        cout << "Response " << std::to_string(sum) << endl;

        write_int(write_fd, sum);
    }

    exit(EXIT_SUCCESS);
}