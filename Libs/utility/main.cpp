#include <dlfcn.h>
#include <cstdlib>
#include <cstdio>

#include "../static/print_sum.h"
#include "../static/add_numbers.h"

#include "../shared/print_sub.h"
#include "../shared/sub_numbers.h"


int main()
{
    print_sum(42, 17);
    print_sub(42, 17);

    using extern_func = void (*) (int, int);

    void* extern_library = dlopen("../loadable/loadable_lib.so", RTLD_LAZY);

    if (extern_library == nullptr)
    {
        printf("%s\n", dlerror());
        exit(1);
    }

    auto print_mul = (extern_func)dlsym(extern_library, "print_mul");

    if (print_mul == nullptr)
    {
        printf("%s\n", dlerror());
        dlclose(extern_library);
        exit(2);
    }

    print_mul(42, 17);
    dlclose(extern_library);
    return 0;
}