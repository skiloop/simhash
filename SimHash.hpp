//
// Created by Baofeng Shii on 2018/7/27.
//

#ifndef SIMHASH_SIMHASH_HPP
#define SIMHASH_SIMHASH_HPP

#include <iostream>
#include <vector>
#include "bigint.hpp"

template<typename T>
class SimHash {
    unsigned int f;
    unsigned int hash_bit;
    T _value;
    std::vector<unsigned> parts;

public:
    SimHash(std::string const &s, unsigned int f = 128, unsigned int hash_bit = 16) : f(f), hash_bit(hash_bit) {
        this->_value = bigint::atoi(s.c_str(), this->_value);
        this->split();
    }

    SimHash(unsigned int f = 128, unsigned int hash_bit = 16) : f(f), hash_bit(hash_bit) {
    }

    std::string string() {
        return bigint::itoa(this->_value);
    }

    std::string value() {
        return this->string();
    }

    std::string hex() {
        return bigint::itoa(this->_value, 16);
    }

    bool similar(SimHash<T> const &another, int count, unsigned int distance) {
        if (this == &another) {
            return true;
        }
        auto cnt = 0;
        auto it = this->parts.begin();
        auto ait = another.parts.begin();
        while (it != this->parts.end() && ait != another.parts.end()) {
            if (*it == *ait) {
                cnt++;
                if (cnt >= count) {
                    return this->distance(another) <= distance;
                }
            }
            it++;
            ait++;
        }
        return false;
    }

    void buildByFeatures(std::vector<T> &features, std::vector<int> &weights) {
        if (weights.begin() == weights.end()) {
            for (size_t i = 0; i < features.size(); i++) {
                weights.push_back(1.0);
            }
        }
        std::vector<T> values;
        for (size_t i = 0; i < this->f; i++) {
            values.push_back(0);
        }
        const T one = 1;
        auto wit = weights.begin();
        for (auto const &feature:features) {
            auto c = one;
            for (auto &v:values) {
                v += feature & c ? *wit : -*wit;
                c <<= 1;
            }
            wit++;
        }
        T ans = 0;
        auto c = one;
        for (auto const &v:values) {
            if (v >= 0)ans |= c;
            c <<= 1;
        }
        this->_value = ans;
        this->split();
    };

    unsigned distance(SimHash<T> const &another) const {
        assert(this->f == another.f);
        auto x = this->_value ^another._value;
        unsigned ans = 0;
        while (x) {
            ans += 1;
            x &= x - 1;
        }
        return ans;
    };

    const std::vector<unsigned> &getParts() {
        return this->parts;
    }

private:
    void split() {
        auto base = ((T) ((1 << this->hash_bit) - 1));
        auto n = this->f / this->hash_bit;
        for (size_t i = 0; i < n; i++) {
            if (this->_value == 0) {
                this->parts.push_back(0);
                continue;
            }
            auto c = i * this->hash_bit;
            auto base_ = base << c;
            this->parts.push_back((this->_value & base_) >> c);
        }
    }
};


#endif //SIMHASH_SIMHASH_HPP
