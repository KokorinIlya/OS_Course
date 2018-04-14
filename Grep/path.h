#ifndef PATH_H
#define PATH_H

#include <string>
#include <cstddef>

namespace OS_Course
{
    struct path
    {
    public:
        path(std::string const& s, size_t n);

        path();

        std::string const& get_name() const;

        size_t get_length() const;

        bool operator<(path const& other) const;

    private:
        std::string path_name;
        size_t path_length; 
    };
}

#endif