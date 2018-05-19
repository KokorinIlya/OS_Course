//
// Created by ilya on 17.05.18.
//

#include "utils.h"

using std::string;

string get_error_description()
{
    return strerror(errno);
}