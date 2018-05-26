//
// Created by ilya on 26.05.18.
//

#ifndef IPC_RAII_PYPE_H
#define IPC_RAII_PYPE_H

struct raii_pipe
{
private:
    int fds[2];

    void close_impl(size_t i);

public:
    raii_pipe() noexcept;

    ~raii_pipe() noexcept;

    void close();

    void create();

    std::pair <int, int> get_fds() noexcept;
};

#endif //IPC_RAII_PYPE_H
