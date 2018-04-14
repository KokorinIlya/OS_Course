#include "path.h"

namespace OS_Course
{
    using std::string;

    path::path(string const& s, size_t n) : path_name(s), path_length(n) {};

    path::path() : path_name(""), path_length(0) {};

    string const& path::get_name() const
    {
        return path_name;
    }

    size_t path::get_length() const
    {
        return path_length;
    }

    bool path::operator<(path const& other) const
    {
        if (path_length == other.path_length)
        {
            return path_name < other.path_name;
        }
        return path_length < other.path_length;
    }
}
