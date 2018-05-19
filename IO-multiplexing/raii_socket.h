//
// Created by ilya on 17.05.18.
//

#ifndef IO_MULTIPLEXING_RAII_SOCKET_H
#define IO_MULTIPLEXING_RAII_SOCKET_H

#include <cstdint>
#include <string>

struct raii_socket
{
private:
    int fd;

    bool need_to_close;

public:
    // General:

    explicit raii_socket(int _fd) noexcept;

    raii_socket() noexcept;

    void close();

    raii_socket(raii_socket const& other) = delete;
    raii_socket& operator=(raii_socket const& other) = delete;

    void swap(raii_socket& other);

    friend void swap(raii_socket& a, raii_socket& b);

    raii_socket(raii_socket&& other) noexcept;

    raii_socket& operator=(raii_socket&& other) noexcept;

    void create();

    ~raii_socket();

    std::pair<std::string, ssize_t> recieve();

    void send(std::string const& s);

    int get_socket_descriptor() const;

    // Client:

    void connect(uint32_t address, uint16_t porn_num);

    // Server:

    void bind(uint16_t porn_num);

    void listen();

    int accept();
};

#endif //IO_MULTIPLEXING_RAII_SOCKET_H
