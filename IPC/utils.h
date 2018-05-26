//
// Created by ilya on 25.05.18.
//

#ifndef IPC_UTILS_H
#define IPC_UTILS_H

#include <string>

std::string get_error_description();

void write_int(int fd, int x);

int read_int(int fd);

#endif //IPC_UTILS_H
