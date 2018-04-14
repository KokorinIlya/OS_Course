#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];

void read_file(int fd)
{

    ssize_t read_count = 0;
    while ((read_count = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        write(STDOUT_FILENO, buffer, read_count);
    }
}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-") == 0)
        {
            read_file(STDIN_FILENO);
        }
        else
        {
            int fd = open(argv[i], O_RDONLY);
            if (fd < 0)
            {
                perror("open");
                continue;
            }
            read_file(fd);
            close(fd);
        }
    }
    return 0;
}
