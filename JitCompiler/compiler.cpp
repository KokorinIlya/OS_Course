#include <iostream>
#include <sys/mman.h>
#include <cstring>

unsigned char return_value_code[] = {
        0x55,	//push   %rbp
        0x48, 0x89, 0xe5,	//mov    %rsp,%rbp
        0xb8, 0x2a, 0x00, 0x00, 0x00,	//mov    $0x2a,%eax
        0x5d,	//pop    %rbp
        0xc3	//retq
};

/*
 * 0x00_00_00_2a = 42
 * 5th-8th position in array
 */

const size_t CODE_SIZE = 11;

using func_type = int(*)();

void do_munmap(void* ptr, size_t size)
{
    int munmap_res = munmap(ptr, CODE_SIZE);
    if (munmap_res != 0)
    {
        perror("error clearing mapped memory");
        exit(-1);
    }
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        int change;
        std::string is(argv[1]);
        change = std::stoi(is);
        unsigned char bytes[4];
        bytes[0] = (change >> 24) & 0xFF;
        bytes[1] = (change >> 16) & 0xFF;
        bytes[2] = (change >> 8) & 0xFF;
        bytes[3] = change & 0xFF;

        return_value_code[8] = bytes[0];
        return_value_code[7] = bytes[1];
        return_value_code[6] = bytes[2];
        return_value_code[5] = bytes[3];
    }
    else if (argc != 1)
    {
        printf("required 1 or 0 arguments\nformat: JitCompiler [<number>]\n");
        exit(4);
    }
    void* ptr = mmap(nullptr, CODE_SIZE * sizeof(unsigned char),
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANONYMOUS,
                    -1, 0);
    if (ptr == (void*) -1)
    {
        perror("mmap failed");
        return -1;
    }

    memcpy(ptr, return_value_code, CODE_SIZE);

    if (mprotect(ptr, CODE_SIZE, PROT_READ | PROT_EXEC) != 0)
    {
        perror("error setting memory executable and readable");
        do_munmap(ptr, CODE_SIZE);
        exit(2);
    }
    func_type func = (func_type)ptr;
    printf("%d\n", func());
    if (mprotect(ptr, CODE_SIZE, PROT_READ | PROT_WRITE) != 0)
    {
        perror("error setting memory writable and readable");
        do_munmap(ptr, CODE_SIZE);
        exit(3);
    }
    do_munmap(ptr, CODE_SIZE);
    return 0;
}