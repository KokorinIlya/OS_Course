//
// Created by ilya on 12.05.18.
//

#include "dict_socket.h"
#include "utils.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <signal.h>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::cerr;
using std::pair;
using std::endl;

dict_socket listener{};
bool need_to_terminate = false;

void sigint_handler(int signum)
{
    cout << "SIGINT catched, exiting...";
    listener.close();
    need_to_terminate = true;
}

int main(int argc, char* argv[])
{
    try
    {
        listener.create();
        signal(SIGINT, &sigint_handler);
        listener.bind();
        listener.listen();

        while (!need_to_terminate)
        {
            dict_socket socket = listener.accept();
            pair<string, string> p = read_until_crlf(socket, "");
            string request = p.first;

            cout << "Request received: " + request << endl;

            socket.send("502 Command not implemented\r\n");
        }


    }
    catch (std::runtime_error& e)
    {
        cout << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

