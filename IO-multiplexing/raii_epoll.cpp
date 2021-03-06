//
// Created by ilya on 18.05.18.
//

#include <unistd.h>
#include <cstdio>
#include <stdexcept>
#include "raii_epoll.h"
#include "utils.h"
#include <sys/epoll.h>
#include <iostream>

using std::runtime_error;
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

raii_epoll::raii_epoll() noexcept : epoll_fd(-1) {}

void raii_epoll::close()
{
    if (::close(epoll_fd) == -1)
    {
        perror("Error closing epoll");
        throw runtime_error("Error closing epoll: " + get_error_description());
    }
    epoll_fd = -1;
}

raii_epoll::~raii_epoll()
{
    cout << "Destructed epoll " + std::to_string(epoll_fd) << endl;
    if (epoll_fd != -1)
    {
        try
        {
            close();
        }
        catch (...) {};
    }
}

void raii_epoll::create(int size)
{
    epoll_fd = epoll_create(size);
    if (epoll_fd == -1)
    {
        perror("Error creating epoll ");
        throw runtime_error("Error creatinh epoll: " + get_error_description());
    }
}

void raii_epoll::add_listener(raii_socket const &socket)
{
    int fd = socket.get_socket_descriptor();
    struct epoll_event event{};
    event.events = EPOLLIN;
    event.data.fd = fd;
    int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    if (res == -1)
    {
        perror("Error adding listener: ");
        throw runtime_error("Error adding listener: " + get_error_description());
    }
    cout << "Listener added " << endl;
}

vector<struct epoll_event> raii_epoll::await()
{
    epoll_event events[NUMBER_OF_EVENTS + 1];
    int events_count = epoll_wait(epoll_fd, events, NUMBER_OF_EVENTS, -1);
    if (events_count == -1)
    {
        perror("Wait error: ");
        throw runtime_error("Error waiting: " + get_error_description());
    }
    vector<epoll_event> answer;
    for (size_t i = 0; i < static_cast<size_t>(events_count); ++i)
    {
        answer.push_back(events[i]);
    }
    return answer;

}

void raii_epoll::add_new_event(int fd, uint32_t events)
{
    struct epoll_event event{};
    event.events = events;
    event.data.fd = fd;
    int res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    if (res == -1)
    {
        perror("Error adding new event: ");
        throw runtime_error("Error adding new event: " + get_error_description());
    }
}

void raii_epoll::delete_event(int fd)
{
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1)
    {
        perror("Error deleting event: ");
        throw runtime_error("Error deleting event: " + get_error_description());
    }
}

void raii_epoll::modify_event(int fd, uint32_t events)
{
    struct epoll_event event{};
    event.events = events;
    event.data.fd = fd;
    int res = epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
    if (res == -1)
    {
        perror("Error modifying event: ");
        throw runtime_error("Error modifying event: " + get_error_description());
    }
}



