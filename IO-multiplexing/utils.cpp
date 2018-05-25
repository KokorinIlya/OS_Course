//
// Created by ilya on 17.05.18.
//

#include "utils.h"
#include "raii_socket.h"

using std::string;

string get_error_description()
{
    return strerror(errno);
}

read_result read_until_crlf(raii_socket& socket, string const& init)
{
    size_t has_crlf = init.find("\r\n");
    if (has_crlf != string::npos)
    {
        return {init.substr(0, has_crlf), init.substr(has_crlf + 2), false};
    }

    string res = init;

    string cur = socket.recieve().first;
    size_t crlf_pos = cur.find("\r\n");
    if (crlf_pos != string::npos)
    {
        res += cur.substr(0, crlf_pos);
        string remainder = cur.substr(crlf_pos + 2);
        return {res, remainder, false};
    }
    return {"", init + cur, true};
}