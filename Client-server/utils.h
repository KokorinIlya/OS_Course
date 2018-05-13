//
// Created by ilya on 12.05.18.
//

#ifndef CLIENT_SERVER_UTILS_H
#define CLIENT_SERVER_UTILS_H

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <utility>
#include "dict_socket.h"

struct read_result
{
    std::string querry;
    std::string remainder;
    bool empty_input;

    read_result(std::string _querry, std::string _remainder, bool _empty) :
            querry(std::move(_querry)), remainder(std::move(_remainder)), empty_input(_empty) {}
};

std::string get_error_description();

read_result read_until_crlf(dict_socket& socket, std::string const& init);

std::vector<std::string> parse_querry(std::string const &request);

#endif //CLIENT_SERVER_UTILS_H
