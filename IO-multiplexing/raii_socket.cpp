//
// Created by ilya on 17.05.18.
//

#include "raii_socket.h"

#include "constants_and_types.h"
#include "utils.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>

using std::string;
using std::pair;

raii_socket::raii_socket() noexcept : fd(-1), need_to_close(true) {}

void raii_socket::create()
{
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("Socket creating error");
        throw std::runtime_error("error creating raii_socket: " +
                                 get_error_description());
    }
    int reusable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reusable, sizeof(reusable)) == -1)
    {
        perror("Error setting socket options");
        throw std::runtime_error("error setting socket options: " +
                                 get_error_description());
    }
}

raii_socket::~raii_socket()
{
    if (fd != -1 && need_to_close)
    {
        try
        {
            close();
        }
        catch (...) {};
    }
}

void raii_socket::connect(uint32_t address, uint16_t port_num)
{
    sockaddr_in socket_address{};
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port_num);
    socket_address.sin_addr.s_addr = address;

    if (::connect(fd, (sockaddr *) &socket_address, sizeof(socket_address)) == -1)
    {
        perror("Connection error");
        throw std::runtime_error("cannot connect to server: "
                                 + get_error_description());
    }
}

pair<string, ssize_t> raii_socket::recieve()
{
    char buffer[BUFFER_SIZE + 1];

    ssize_t bytes_read = ::read(fd, buffer, BUFFER_SIZE);

    if (bytes_read == -1)
    {
        perror("Read error");
        throw std::runtime_error("error reading from raii_socket: "
                                 + get_error_description());
    }

    buffer[bytes_read] = '\0';

    return std::make_pair(string(buffer), bytes_read);
}

void raii_socket::send(std::string const &s)
{
    char const* buf = s.c_str();
    size_t len = s.size();

    size_t totally_sent = 0;
    while (totally_sent < len)
    {
        ssize_t sent = write(fd, buf + totally_sent, len - totally_sent);
        if (sent == -1)
        {
            perror("Write error");
            throw std::runtime_error("error writing to raii_socket: "
                                     + get_error_description());
        }
        totally_sent += static_cast<size_t>(sent);
    }
}

void raii_socket::bind(uint16_t porn_num)
{
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(porn_num);
    address.sin_addr.s_addr = INADDR_ANY;

    if (::bind(fd, (sockaddr *) &address, sizeof(address)) == -1)
    {
        perror("Bind error");
        throw std::runtime_error("Cannot bind to the specified port: "
                                 + get_error_description());
    }
}

void raii_socket::listen()
{
    if (::listen(fd, 0) == -1)
    {
        perror("Listening error");
        throw std::runtime_error("Error listening: "
                                 + get_error_description());
    }
}

void raii_socket::swap(raii_socket &other)
{
    std::swap(fd, other.fd);
}

void swap(raii_socket &a, raii_socket &b)
{
    a.swap(b);
}

raii_socket::raii_socket(raii_socket &&other) noexcept : fd(other.fd)
{
    other.fd = -1;
}

raii_socket &raii_socket::operator=(raii_socket &&other) noexcept
{
    swap(other);
    return *this;
}

int raii_socket::accept()
{
    int socket_fd = ::accept(fd, nullptr, nullptr);
    if (socket_fd == -1)
    {
        perror("Accepting error");
        throw std::runtime_error("Error accepting socket "
                                 + get_error_description());
    }
    return socket_fd;
}

raii_socket::raii_socket(int _fd) noexcept : fd(_fd), need_to_close(false) {}

void raii_socket::close()
{
    std::cout << "Destructed socket " + std::to_string(fd) << std::endl;
    if (::close(fd) == -1)
    {
        perror("Error closing socket");
        throw std::runtime_error("Error closing socket: " + get_error_description());
    }
    fd = -1;
}

int raii_socket::get_socket_descriptor() const
{
    return fd;
}
