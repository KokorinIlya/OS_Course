//
// Created by ilya on 12.05.18.
//

#ifndef CLIENT_SERVER_CLIENT_SOCKET_H
#define CLIENT_SERVER_CLIENT_SOCKET_H

#include <netinet/in.h>
#include <string>

struct dict_socket
{
private:
    int fd;

    explicit dict_socket(int _fd);

public:
    // General:

    dict_socket() noexcept;

    void close();

    dict_socket(dict_socket const& other) = delete;
    dict_socket& operator=(dict_socket const& other) = delete;

    void swap(dict_socket& other);

    friend void swap(dict_socket& a, dict_socket& b);

    dict_socket(dict_socket&& other) noexcept;

    dict_socket& operator=(dict_socket&& other) noexcept;

    void create();

    ~dict_socket();

    std::pair<std::string, ssize_t> recieve();

    void send(std::string const& s);

    // Client:

    void connect(uint32_t address);

    // Server:

    void bind();

    void listen();

    dict_socket accept();
};



#endif //CLIENT_SERVER_CLIENT_SOCKET_H
