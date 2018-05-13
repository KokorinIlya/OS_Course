//
// Created by ilya on 12.05.18.
//

#include <stdexcept>
#include <iostream>
#include "utils.h"

using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

string get_error_description()
{
    return strerror(errno);
}

read_result read_until_crlf(dict_socket& socket, string const& init)
{
    string res = init;

    string cur;
    bool has_cr = false;

    while (!(cur = socket.recieve().first).empty())
    {
        size_t crlf_pos = cur.find("\r\n");
        if (crlf_pos != string::npos)
        {
            res += cur.substr(0, crlf_pos);
            //cout << "Res: " << res  << ", Cur: " << cur << endl;
            return {res, cur.substr(crlf_pos + 2), false};
        }
        if (has_cr)
        {
            if (!cur.empty() && cur[0] == '\n')
            {
                //cout << "Res: " << res  << ", Cur: " << cur << endl;
                return {res.substr(0, res.size() - 1), cur.substr(1), false};
            }
        }
        if (!cur.empty())
        {
            has_cr = (cur[cur.size() - 1] == '\r');
        }
    }
    return {"", "", true};
}

vector<string> parse_querry(string const &request)
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