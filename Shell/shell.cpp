#include <iostream>
#include <vector>
#include <stddef.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include <wait.h>

using std::string;
using std::vector;

void print_invitation()
{
    printf("shell:~$ ");
    fflush(stdout);
}

void process_command(string const& s)
{
    if (s == "exit")
    {
        printf("exiting...\n");
        exit(0);
    }
    vector<char*> arguments;
    string cur_arg;
    int i = 0;

    while (i < s.length())
    {
        while (i < s.length() && s[i] != ' ')
        {
            cur_arg += s[i];
            i++;
        }
        arguments.push_back(strdup(cur_arg.c_str()));
        cur_arg = "";
        i++;
    }
    if (arguments.empty())
    {
        perror("cannot specify program from output");
        return;
    }

    arguments.push_back(nullptr);

    pid_t pid_id = fork();
    if (pid_id == 0)
    {
        int res = execvp(arguments[0], arguments.data());
        if (res < 0)
        {
            perror("program terminated with non-zero exit code");
            exit(res);
        }
    }
    else if (pid_id > 0)
    {
        int status = 0;
        int res = waitpid(pid_id, &status, 0);
        if (res == -1)
        {
            perror("error waiting");
            exit(-1);
        }
        int exit_result = WEXITSTATUS(status);
        printf("\nprogram finished with return code ");
        printf("%d\n", exit_result);
        fflush(stdout);
    }
    else
    {
        perror("error - fork failed");
        exit(2);
    }
    print_invitation();
}

void fill_zero(char* const buff, ssize_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        buff[i] = '\0';
    }
}

int main()
{
    printf("type exit for exiting\n");
    print_invitation();

    while (true)
    {
        const size_t BUFFER_SIZE = 20;
        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        string cur_command;

        while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0)
        {
            string cur_chars(buffer);
            for (char cur_char : cur_chars)
            {
                if (cur_char == '\n')
                {
                    process_command(cur_command);
                    cur_command = "";
                }
                else
                {
                    cur_command += cur_char;
                }
            }
            fill_zero(buffer, bytes_read);
        }
        if (bytes_read == -1)
        {
            perror("error reading from standart output");
            exit(1);
        }
    }
}