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

    string cur;
    bool has_cr = false;

    while (!(cur = socket.recieve().first).empty())
    {
        size_t crlf_pos = cur.find("\r\n");
        //cout << "Cur: " << cur << endl;
        if (crlf_pos != string::npos)
        {
            res += cur.substr(0, crlf_pos);
            string remainder = cur.substr(crlf_pos + 2);
            //cout << "Res: " << res  << ", Rem: " << remainder << endl;
            return {res, remainder, false};
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