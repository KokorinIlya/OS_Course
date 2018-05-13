//
// Created by ilya on 12.05.18.
//

#ifndef CLIENT_SERVER_UTILS_H
#define CLIENT_SERVER_UTILS_H

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include "dict_socket.h"

std::string get_error_description();

std::pair<std::string, std::string> read_until_crlf(dict_socket& socket, std::string const& init);

std::vector<std::string> parse_querry(std::string const &request);

#endif //CLIENT_SERVER_UTILS_H
