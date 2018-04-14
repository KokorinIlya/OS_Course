#include "KMP.h"

namespace OS_Course
{
    using std::string;
    using std::vector;
    const size_t ALPHABET_SIZE = 256;

    KMP::KMP(string const& s) : jmp(s.length() + 1), string_size(s.length()), cur_index(0)
    {
        for (int i = 0; i <= string_size; ++i)
        {
            jmp[i].resize(ALPHABET_SIZE);
        }
        build(s);
    }

    void KMP::reset()
    {
        cur_index = 0;
    }

    bool KMP::go(unsigned char c)
    {
        cur_index = jmp[cur_index][c];
        return (cur_index == string_size);
    }

    vector<int> KMP::pref_fun(string const& s) 
    {
        vector<int> p(s.length());
        p[0] = 0;
        for (size_t i = 1; i < s.length(); ++i)
        {
            int k = p[i - 1];
            while (k > 0 && s[i] != s[k])
            {
                k = p[k - 1];
            }
            if (s[i] == s[k])
            {
                k++;
            }
            p[i] = k;
        }
        return p;
    }
    
    void KMP::build(string const& s)
    {
        vector <int> pref = pref_fun(s);
        for (size_t cc = 0; cc <= 255; ++cc)
        {
            auto c = static_cast<unsigned char>(cc);
            if (c == s[0])
            {
                jmp[0][c] = 1;
            }
            else
            {
                jmp[0][c] = 0;
            }
        }
        for (size_t i = 1; i < s.length(); ++i)
         {
            for (size_t cc = 0; cc <= 255; ++cc)
            {
                auto c = static_cast<unsigned char>(cc);
                if (c == s[i])
                {
                    jmp[i][c] = i + 1;
                }
                else
                {
                    jmp[i][c] = jmp[pref[i - 1]][c];
                }
            }                
        }
            
        for (size_t cc = 0; cc <= 255; ++cc)
        {
            auto c = static_cast<unsigned char>(cc);
            jmp[s.length()][c] = s.length();
        }
    } 
}

