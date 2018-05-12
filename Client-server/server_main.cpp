//
// Created by ilya on 12.05.18.
//

#include "dict_socket.h"
#include "utils.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <signal.h>
#include <algorithm>
#include <sstream>

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

string get_connection_message()
{
    return "220, connected successfully\r\n";
}

vector<string> parse_request(string const& request)
{
    vector<string> tokens;
    std::string token;
    std::istringstream tokenStream(request);
    while (std::getline(tokenStream, token, ' '))
    {
        if (token != " " && !token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
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

            socket.send(get_connection_message());

            pair<string, string> p = read_until_crlf(socket, "");
            string request = p.first;

            cout << "Request received: " + request << endl;

            vector<string> parts = parse_request(request);
            for (size_t i = 0; i < parts.size(); ++i)
            {
                cout << "Part № " << i << " - " << parts[i] << endl;
            }

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

