//
// Created by ilya on 25.05.18.
//

#ifndef IPC_UNIX_SOCKET_H
#define IPC_UNIX_SOCKET_H

#include <unistd.h>
#include <string>

struct unix_socket
{
private:
    int fd;

    explicit unix_socket(int _fd);

public:
    // General:

    unix_socket() noexcept;

    void close();

    unix_socket(unix_socket const& other) = delete;
    unix_socket& operator=(unix_socket const& other) = delete;

    void swap(unix_socket& other);

    friend void swap(unix_socket& a, unix_socket& b);

    unix_socket(unix_socket&& other) noexcept;

    unix_socket& operator=(unix_socket&& other) noexcept;

    void create();

    ~unix_socket();

    void send_file_descriptor(int fd);

    int get_file_descriptor();


    // Client:

    void connect(char const* file_name);

    // Server:

    void bind(const char* file_name);

    void listen();

    unix_socket accept();
};

#endif //IPC_UNIX_SOCKET_H
