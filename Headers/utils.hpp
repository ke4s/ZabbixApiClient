#ifndef UTILS_HPP
 #define UTILS_HPP

#include  <cstring>

int strsncmp(const char *str1,  char **strarr, size_t count, size_t n)
{
    for (size_t i = 0; i < count; ++i)
    {
        if (::strncmp(str1, strarr[i], n) == 0)
            return i;
    }
    return -1;
}


#endif