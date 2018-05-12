//
// Created by ilya on 12.05.18.
//

#include "dict_socket.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <signal.h>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::cerr;

dict_socket listener{};

void sigint_handler(int signum)
{
    cout << "SIGINT catched, exiting...";
    listener.close();
    exit(0);
}

int main(int argc, char* argv[])
{
    try
    {
        listener.create();
        signal(SIGINT, &sigint_handler);
        listener.bind();
        listener.listen();
    }
    catch (std::runtime_error& e)
    {
        cout << e.what();
        return EXIT_FAILURE;
    }

    dict_socket socket = listener.accept();

    size_t bytes_sent = 0;

    while (bytes_sent < string("world").size())
    {
        string r = socket.recieve().first;
        socket.send(r);
        bytes_sent += r.size();
    }


    return EXIT_SUCCESS;
}

