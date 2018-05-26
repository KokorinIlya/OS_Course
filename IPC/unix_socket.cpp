//
// Created by ilya on 25.05.18.
//

#include <sys/socket.h>
#include <stdexcept>
#include <sys/un.h>
#include "unix_socket.h"
#include "utils.h"

using std::string;

unix_socket::unix_socket() noexcept : fd(-1) {}

void unix_socket::create()
{
    fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
    {
        perror("Socket creating error");
        throw std::runtime_error("error creating unix_socket: " +
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

unix_socket::~unix_socket()
{
    if (fd != -1)
    {
        try
        {
            close();
        }
        catch (...) {}
    }
}

void unix_socket::connect(char const* file_name)
{
    sockaddr_un address{};
    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, file_name);
    if (::connect(fd, (sockaddr *) &address, sizeof(address)) < 0)
    {
        perror("Connect error");
        throw std::runtime_error("Connect error "
                                 + get_error_description());
    }
}

void unix_socket::bind(const char* file_name)
{
    sockaddr_un socket_address{};
    socket_address.sun_family = AF_UNIX;
    strcpy(socket_address.sun_path, file_name);
    if (::bind(fd, (sockaddr*) &socket_address,
               sizeof(socket_address.sun_family) + strlen(socket_address.sun_path)) == -1)
    {
        perror("Bind error");
        throw std::runtime_error("Bind error "
                                 + get_error_description());
    }

}

void unix_socket::listen()
{
    if (::listen(fd, 0) == -1)
    {
        perror("Listening error");
        throw std::runtime_error("Error listening: "
                                 + get_error_description());
    }
}

void unix_socket::swap(unix_socket &other)
{
    std::swap(fd, other.fd);
}

void swap(unix_socket &a, unix_socket &b)
{
    a.swap(b);
}

unix_socket::unix_socket(unix_socket &&other) noexcept : fd(other.fd)
{
    other.fd = -1;
}

unix_socket &unix_socket::operator=(unix_socket &&other) noexcept
{
    swap(other);
    return *this;
}

unix_socket unix_socket::accept()
{
    int socket_fd = ::accept(fd, nullptr, nullptr);
    if (socket_fd == -1)
    {
        perror("Accepting error");
        throw std::runtime_error("Accepting error "
                                 + get_error_description());
    }
    return unix_socket(socket_fd);
}

unix_socket::unix_socket(int _fd) : fd(_fd) {}

void unix_socket::close()
{
    //std::cout << "Closed " << fd << std::endl;
    if (::close(fd) == -1)
    {
        perror("Error closing socket: ");
        throw std::runtime_error("Error closing socket: " + get_error_description());
    }
    fd = -1;
}

void unix_socket::send_file_descriptor(int fd_to_send)
{
    struct msghdr msg = {nullptr};
    char buf[CMSG_SPACE(sizeof(fd_to_send))];
    memset(buf, 0, sizeof(buf));

    struct iovec io = { .iov_base = (void *) "", .iov_len = 1 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd_to_send));

    memmove(CMSG_DATA(cmsg), &fd_to_send, sizeof(fd_to_send));

    msg.msg_controllen = cmsg->cmsg_len;

    if (sendmsg(fd, &msg, 0) < 0)
    {
        perror("Error sending file descriptor: ");
        throw std::runtime_error("Error sending file descriptor: "
                                 + get_error_description());
    }
}

int unix_socket::get_file_descriptor()
{
    msghdr message = {0};
    cmsghdr *structcmsghdr;
    char buffer[CMSG_SPACE(sizeof(int))];
    char duplicate[512];
    bzero(buffer, sizeof(buffer));
    iovec io = {.iov_base = &duplicate, .iov_len = sizeof(duplicate)};

    message.msg_control = buffer;
    message.msg_controllen = sizeof(buffer);
    message.msg_iov = &io;
    message.msg_iovlen = 1;

    if (recvmsg(fd, &message, 0) < 0) {
        perror("recvmsg");
        return -1;
    }


    structcmsghdr = CMSG_FIRSTHDR(&message);
    int received_fd;
    memcpy(&received_fd, (int *) CMSG_DATA(structcmsghdr), sizeof(int));
    return received_fd;
}

