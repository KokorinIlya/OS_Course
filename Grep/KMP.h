#ifndef KMP_H
#define KMP_H

#include <string>
#include <vector>
#include <cstddef>

namespace OS_Course
{
    struct KMP
    {
    public:
        KMP(std::string const& s);

        void reset();
        
        bool go(unsigned char c);

    private:

        std::vector<std::vector<size_t>> jmp;
        size_t string_size;
        size_t cur_index;

        std::vector<int> pref_fun(std::string const& s);

        void build(std::string const& s);    
    };
}

#endif
