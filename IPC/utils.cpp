//
// Created by ilya on 25.05.18.
//

#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "utils.h"

using std::string;
using std::vector;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

string get_error_description()
{
    return strerror(errno);
}

void write_int(int fd, int x)
{
    uint8_t buffer[4];
    size_t sent = 0;

    for (size_t i = 0; i < 4; i++)
    {
        buffer[3 - i] = static_cast<uint8_t>((x >> (i * 8)) & 0xFF);
    }

    /*for (size_t i = 0; i < 4; i++)
    {
        cout << static_cast<int>(buffer[i]) << " ";
    }
    cout << endl;*/

    while (sent < 4)
    {
        ssize_t count = write(fd, buffer + sent, 4 - sent);
        if (count == -1)
        {
            perror("Error writing int");
            throw std::runtime_error("Error writing int " + get_error_description());
        }
        sent += static_cast<size_t>(count);
    }
}

int read_int(int fd)
{
    size_t received = 0;
    uint8_t buffer[4];

    while (received < 4)
    {
        ssize_t count = read(fd, buffer + received, 4 - received);
        if (count == -1)
        {
            perror("Error reading int");
            throw std::runtime_error("Error reading int " + get_error_description());
        }
        received += static_cast<size_t>(count);
    }
    /*for (size_t i = 0; i < 4; i++)
    {
        cout << static_cast<int>(buffer[i]) << " ";
    }
    cout << endl;*/
    return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | (buffer[3]);
}