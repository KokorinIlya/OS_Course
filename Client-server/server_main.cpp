//
// Created by ilya on 12.05.18.
//

#include "dict_socket.h"
#include "utils.h"
#include "fill_db.h"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <signal.h>
#include <algorithm>
#include <sstream>
#include <map>

using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::cerr;
using std::pair;
using std::endl;
using std::map;

// dicts: dictionary name -> dictionary
// dictionary: word -> word
map<string, map<string, string>> dicts;

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
    return "220 connected successfully\r\n";
}

void process_show_db(dict_socket &socket)
{
    if (dicts.empty())
    {
        socket.send("554 No databases present\r\n");
        return;
    }
    string res = "110 ";
    res += (std::to_string(dicts.size()) + string(" databases present -"));
    for (auto const& p: dicts)
    {
        res += " " + p.first + ",";
    }
    res += "\r\n";
    socket.send(res);
}

void process_show_info(dict_socket& socket, string const& name)
{
    if (dicts.count(name) == 0)
    {
        socket.send("550 Invalid database, use \"SHOW DB\" for list of databases\r\n");
        return;
    }
    string res = "112 name: " + name + ", size: " +
                 std::to_string(dicts[name].size()) + "\r\n";
    socket.send(res);
}

bool process_client(dict_socket& socket)
{
    read_result p = read_until_crlf(socket, "");
    string request = p.querry;
    std::replace(request.begin(), request.end(), '\t', ' ');
    std::transform(request.begin(), request.end(), request.begin(), ::tolower);
    vector<string> parts = parse_querry(request);
    if (!parts.empty() && parts[0] == "client")
    {
        cout << "CLIENT request: " << endl;
        socket.send("250 ok client information received\r\n");
        return true;
    }
    else
    {
        socket.send("500 Error, CLIENT request should be first\r\n");
        return false;
    }
}

void process_request(dict_socket& socket, string const& request)
{

    cout << "Request received: " + request << endl;
    vector<string> parts = parse_querry(request);
    for (size_t i = 0; i < parts.size(); ++i)
    {
        cout << "Part № " << i << " - " << parts[i] << endl;
    }

    if (parts.size() == 2 && parts[0] == "show" &&
            (parts[1] == "db" || parts[1] == "databases"))
    {
        cout << "SHOW DB request: " << endl;
        process_show_db(socket);
    }
    else if (parts.size() == 2 && parts[0] == "show" &&
             (parts[1] == "strat" || parts[1] == "strategies"))
    {
        cout << "SHOW STRAT request: " << endl;
        socket.send("555 No strategies available\r\n");
    }
    else if (parts.size() == 2 && parts[0] == "show" &&
             parts[1] == "server")
    {
        cout << "SHOW SERVER request: " << endl;
        socket.send("114 my dictionary server for OS Course\r\n");
    }
    else if (parts.size() == 3 && parts[0] == "show" &&
             parts[1] == "info")
    {
        cout << "SHOW INFO request: " << endl;
        process_show_info(socket, parts[2]);
    }
    else if (parts.size() == 4 && parts[0] == "match")
    {
        cout << "MATCH request: " << endl;
        socket.send("551 Invalid strategy, use \"SHOW STRAT\" for a list of strategies\r\n");
    }
    else if (!parts.empty() && parts[0] == "client")
    {
        cout << "CLIENT request: " << endl;
        socket.send("250 ok client information received\r\n");
    }
    else
    {
        socket.send("502 Command not implemented\r\n");
    }


}

int main(int argc, char* argv[])
{
    fill_databases(dicts);

    cout << "Dictionary initialization finished: " << endl;
    for (const auto& dict: dicts)
    {
        cout << "Dictionary: " << dict.first << endl;
        for (const auto& p: dict.second)
        {
            cout << p.first << " - " << p.second << endl;
        }
        cout << "Dictionary finished" << endl;
    }

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
            if (!process_client(socket))
            {
                continue;
            }

            try
            {
                read_result p = read_until_crlf(socket, "");
                string request = p.querry;
                std::replace(request.begin(), request.end(), '\t', ' ');
                std::transform(request.begin(), request.end(), request.begin(), ::tolower);
                process_request(socket, request);
            }
            catch (...)
            {
                cout << "Client is processed with error, continuing..." << endl;
            }


        }


    }
    catch (std::runtime_error& e)
    {
        cout << e.what();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

