//
// Created by ilya on 17.05.18.
//

#ifndef IO_MULTIPLEXING_UTILS_H
#define IO_MULTIPLEXING_UTILS_H

#include <string>
#include <cstring>
#include "raii_socket.h"

std::string get_error_description();

struct read_result
{
    std::string querry;
    std::string remainder;
    bool empty_input;

    read_result(std::string _querry, std::string _remainder, bool _empty) :
            querry(std::move(_querry)), remainder(std::move(_remainder)), empty_input(_empty) {}
};

read_result read_until_crlf(raii_socket& socket, std::string const& init);

#endif //IO_MULTIPLEXING_UTILS_H
