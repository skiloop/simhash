//
// Created by Baofeng Shii on 2018/7/31.
//

#ifndef SIMHASH_BIGINT_HPP
#define SIMHASH_BIGINT_HPP

#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

namespace bigint {
    template<typename T>
    std::string itoa(T i, int base = 10) {
        const char *values = "0123456789abcdef";
        std::string res;
        int sign = 1;
        if (!std::is_unsigned<T>::value && i < 0) {
            sign = -1;
        }
        while (i != 0) {
            res += values[i % base];
            i /= base;
        }
        if (res.length() == 0) {
            res += '0';
        }
        if (sign == -1) {
            res += '-';
        }
        std::reverse(res.begin(), res.end());
        return res;
    }

    template<typename T>
    T atoi(const char *v, T t, int base = 10) {
        if (v == nullptr) {
            return 0;
        }
        const char *start = v;
        int sign = 1;
        if (!std::is_unsigned<T>::value && *start == '-') {
            sign = -1;
            start++;
        }
        T i = 0;
        while (*start) {
            if (*start >= 'a' && *start <= 'f') {
                i += (int) (*start - 'a') + 10;
            } else if (*start >= 'A' && *start <= 'F') {
                i += (int) (*start - 'A') + 10;
            } else {
                i += (int) (*start - '0');
            }
            start++;
            if (*start) {
                i *= base;
            }
        }
        return i * sign;
    }

}
#endif //SIMHASH_BIGINT_HPP
