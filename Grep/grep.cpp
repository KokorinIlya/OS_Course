#include "KMP.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstddef>
#include <cstdio>
#include <unordered_map>
#include "path.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <set>
#include <dirent.h>

using std::string;
using std::vector;
using std::unordered_map;
using OS_Course::path;
using OS_Course::KMP;
using std::set;
using std::pair;

unordered_map<ino_t, bool> files_visited;

bool check_file(string const& file_name, KMP& kmp)
{
    struct stat file_stat{};
    stat(file_name.c_str(), &file_stat);
    ino_t file_inode = file_stat.st_ino;
    if (files_visited.count(file_inode) != 0)
    {
        return files_visited[file_inode];
    }

    kmp.reset();
    int fd = open(file_name.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("cannot open file");
        return false;
    }
    bool has_substring = false;
    ssize_t chars_read = 0;
    size_t BUFFER_SIZE = 2048;
    char buffer[BUFFER_SIZE];

    while ((chars_read = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        for (ssize_t i = 0; i < chars_read; i++)
        {
            has_substring |= kmp.go(static_cast<unsigned char>(buffer[i]));
        }
    }
    if (chars_read < 0)
    {
        perror("Error reading file");
        has_substring = false;
    }

    close(fd);
    files_visited[file_inode] = has_substring;
    return has_substring;
}

void put_into_map(unordered_map<ino_t, path>& map, ino_t key, path const& value)
{
    if (map.count(key) == 0 || value < map[key])
    {
        map[key] = value;
    }
}

void dijkstra(string const& str_to_find)
{
    unordered_map<ino_t, path> min_path;
    unordered_map<ino_t, path> path_to_found;
    set<ino_t> used;

    KMP kmp(str_to_find);
    char start_dir_buffer[2048];
    getcwd(start_dir_buffer, 2048);
    string start_dir_name(start_dir_buffer);
    struct stat start_dir_stat{};
    stat(start_dir_name.c_str(), &start_dir_stat);
    path path_to_start_dir{start_dir_name, 0};
    min_path[start_dir_stat.st_ino] = path_to_start_dir;

    set<pair<path, ino_t >> queue;
    queue.insert({path_to_start_dir, start_dir_stat.st_ino});
    while (!queue.empty())
    {
        pair<path, ino_t> p = *(queue.begin());
        queue.erase(queue.begin());
        if (used.count(p.second) != 0)
        {
            continue;
        }
        used.insert(p.second);

        DIR* cur_dir = opendir(p.first.get_name().c_str());
        if (cur_dir == nullptr)
        {
            perror("error opening directory");
            continue;
        }

        dirent* cur_entry;
        while ((cur_entry = readdir(cur_dir)) != nullptr)
        {
            if (string(cur_entry->d_name) == "..")
            {
                continue;
            }
            string cur_name = p.first.get_name() + "/" + cur_entry->d_name;
            struct stat cur_stat{};
            stat(cur_name.c_str(), &cur_stat);
            ino_t cur_inode = cur_stat.st_ino;
            path maybe_min_path(cur_name, p.first.get_length() + 1);
            put_into_map(min_path, cur_inode, maybe_min_path);

            if (S_ISDIR(cur_stat.st_mode))
            {
                if (used.count(cur_inode) == 0)
                {
                    queue.insert({maybe_min_path, cur_inode});
                }
            }
            else if (S_ISREG(cur_stat.st_mode))
            {
                if (check_file(cur_name, kmp))
                {
                    put_into_map(path_to_found, cur_inode, maybe_min_path);
                }
            }
        }

        closedir(cur_dir);
    }
    for (auto const &it : path_to_found)
    {
        string output = it.second.get_name() + "\n";
        write(STDOUT_FILENO, output.c_str(), output.length());
    }
}

int main(int argc, const char* argv[])
{
    if (argc != 2)
    {
        string error_run = "1 argument required: string to find. Usage: ./mygrep <string-to-find>\n";
        write(STDERR_FILENO, error_run.c_str(), error_run.length());
        return 1;
    }
    files_visited.clear();
    dijkstra(string(argv[1]));
    return 0;
}
