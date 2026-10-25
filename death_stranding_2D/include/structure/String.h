#pragma once

namespace GamesEngineeringBase::StrHelper
{
    inline int strLen(const char *str)
    {
        int len = 0;
        while (str[len] != '\0')
        {
            ++len;
        }
        return len;
    }

    inline void copyStr(char *dest, const char *src)
    {
        int i = 0;
        while (src[i] != '\0')
        {
            dest[i] = src[i];
            ++i;
        }
        dest[i] = '\0';
    }
}