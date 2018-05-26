//
// Created by ilya on 26.05.18.
//

#include <unistd.h>
#include <cstdio>
#include <stdexcept>
#include <vector>
#include "raii_pipe.h"
#include "utils.h"

using std::pair;
using std::string;
using std::vector;

void raii_pipe::close_impl(size_t i)
{
    if (::close(fds[i]) == -1)
    {
        perror("Error closing pipe: ");
        throw std::runtime_error("Error closing pipe: " + get_error_description());
    }
    fds[i] = -1;
}

void raii_pipe::close()
{
    std::runtime_error e{""};
    bool error = false;

    for (size_t i = 0; i <= 1; i++)
    {
        try
        {
            close_impl(i);
        }
        catch (std::runtime_error& ee)
        {
            e = ee;
            error = true;
        }
    }

    if (error)
    {
        throw e;
    }

}

raii_pipe::~raii_pipe() noexcept
{
    if (fds[0] != -1)
    {
        try
        {
            close();
        }
        catch (...) {}
    }
}

raii_pipe::raii_pipe() noexcept
{
    fds[0] = fds[1] = -1;
}

void raii_pipe::create()
{
    if (pipe(fds) == -1)
    {
        perror("Error creating pipe: ");
        throw std::runtime_error("Error creating pipe");
    }
}

pair<int, int> raii_pipe::get_fds() noexcept
{
    return {fds[0], fds[1]};
}
