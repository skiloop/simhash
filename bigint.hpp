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
        const char *values = "0123456789ABCDEF";
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
        const char *start = v;
        int sign = 1;
        if (!std::is_unsigned<T>::value && *start == '-') {
            sign = -1;
            start++;
        }
        const char *end = v + strlen(v);
        T i = 0;
        T n = 1;
        while (end != start) {
            end--;
            if (*end >= 'A' && *end <= 'F') {
                i += n * (int) (*end - 'A') + 10;
            } else if (*end >= 'a' && *end <= 'f') {
                i += n * (int) (*end - 'a') + 10;
            } else {
                i += n * (int) (*end - '0');
            }
            n *= base;
        }
        return i * sign;
    }

}
#endif //SIMHASH_BIGINT_HPP
