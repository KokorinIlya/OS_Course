//
// Created by ilya on 18.05.18.
//

#ifndef IO_MULTIPLEXING_RAII_EPOLL_H
#define IO_MULTIPLEXING_RAII_EPOLL_H

#include <vector>
#include <sys/epoll.h>
#include "raii_socket.h"

struct raii_epoll
{
private:
    int epoll_fd;


public:

    static const int NUMBER_OF_EVENTS = 1000;

    raii_epoll() noexcept;

    void create(int size);

    ~raii_epoll();

    void close();

    void add_listener(raii_socket const& socket);

    void add_for_reading(int fd);

    std::vector<struct epoll_event> await();

};

#endif //IO_MULTIPLEXING_RAII_EPOLL_H
