//
// Created by ilya on 12.05.18.
//

#include "dict_socket.h"
#include "constants_and_types.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdexcept>
#include <unistd.h>
#include <iostream>

using std::string;

dict_socket::dict_socket() noexcept : fd(-1) {}

void dict_socket::create()
{
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("Socket creating error");
        throw std::runtime_error("error creating dict_socket: " + errno);
    }
    int reusable = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reusable, sizeof(reusable)) == -1)
    {
        perror("Error setting socket options");
        throw std::runtime_error("error setting socket options: " + errno);
    }
}

dict_socket::~dict_socket()
{
    if (fd != -1)
    {
        close();
    }
}

void dict_socket::connect(uint32_t address)
{
    sockaddr_in socket_address{};
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(PORT_NUM);
    socket_address.sin_addr.s_addr = address;

    if (::connect(fd, (sockaddr *) &socket_address, sizeof(socket_address)) == -1)
    {
        perror("Connection error");
        throw std::runtime_error("cannot connect to server: " + errno);
    }
}

std::pair<std::string, ssize_t > dict_socket::recieve()
{
    char buffer[BUFFER_SIZE + 1];

    ssize_t bytes_read = ::read(fd, buffer, BUFFER_SIZE);

    if (bytes_read == -1)
    {
        perror("Read error");
        throw std::runtime_error("error reading from dict_socket: " + errno);
    }

    buffer[bytes_read] = '\0';

    return std::make_pair(string(buffer), bytes_read);
}

void dict_socket::send(std::string const &s)
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
            throw std::runtime_error("error writing to dict_socket: " + errno);
        }
        totally_sent += static_cast<size_t>(sent);
    }
}

void dict_socket::bind()
{
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT_NUM);
    address.sin_addr.s_addr = INADDR_ANY;

    if (::bind(fd, (sockaddr *) &address, sizeof(address)) == -1)
    {
        perror("Bind error");
        throw std::runtime_error("Cannot bind to the specified port: " + errno);
    }
}

void dict_socket::listen()
{
    if (::listen(fd, 0) == -1)
    {
        perror("Listening error");
        throw std::runtime_error("Error listening: " + errno);
    }
}

void dict_socket::swap(dict_socket &other)
{
    std::swap(fd, other.fd);
}

void swap(dict_socket &a, dict_socket &b)
{
    a.swap(b);
}

dict_socket::dict_socket(dict_socket &&other) noexcept : fd(other.fd)
{
    other.fd = -1;
}

dict_socket &dict_socket::operator=(dict_socket &&other) noexcept
{
    swap(other);
    return *this;
}

dict_socket dict_socket::accept()
{
    int socket_fd = ::accept(fd, nullptr, nullptr);
    if (socket_fd == -1)
    {
        perror("Accepting error");
        throw std::runtime_error("Error accepting socket " + errno);
    }
    return dict_socket(socket_fd);
}

dict_socket::dict_socket(int _fd) : fd(_fd) {}

void dict_socket::close()
{
    //std::cout << "Closed " << fd << std::endl;
    ::close(fd);
    fd = -1;
}




