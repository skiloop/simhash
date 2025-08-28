//
// Created by Baofeng Shii on 2018/7/27.
//

#ifndef SIMHASH_SIMHASH_HPP
#define SIMHASH_SIMHASH_HPP

#include <iostream>
#include <vector>

#include "bigint.hpp"
#include "SimHashBase.h"

#if defined(__SSE4_2__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(_M_X64)
#define HIGH64(x) (static_cast<__uint64_t>(x>>64))
#define LOW64(x) (static_cast<__uint64_t>(x&((1<<64)-1))
#else
#include <immintrin.h>		
#endif

template<typename T>
class SimHash : public SimHashBase {
    static const unsigned int f = sizeof(T) * 8;
    T _value;

public:
    bool similar2(SimHashBase const &another, int count, unsigned int distance) const{
         if (this->dimension() != another.dimension())return false;
        if (this == &another) {
            return true;
        }
        return false;
    }

    explicit SimHash(std::string const &s, unsigned int hash_bit = 16, int base = 16)
            : SimHashBase(s, hash_bit, base) {
        this->applyValue(s, base);
        this->split();
    }

    explicit SimHash(unsigned int hash_bit = 16) : SimHashBase(hash_bit) {
    }

    std::string string() const override {
        return bigint::itoa(this->_value);
    }

    std::string hex() const override {
        return bigint::itoa(this->_value, 16);
    }

    unsigned dimension() const override {
        return this->f;
    }

    void build(std::vector<std::string> &features, std::vector<int> &weights, int base) override {
        std::vector<T> feat;
        const T t = 0;

#ifdef DEBUG
        int i = 0;
#endif
        for (const auto& token:features) {

            feat.push_back(bigint::atoi(token.c_str(), t, base));
#ifdef DEBUG
            std::cout<<"features["<<i<<"]"<<token<<","<<bigint::itoa(feat[i], base)<<std::endl;
            i++;
#endif
        }

#ifdef DEBUG
        i = 0;
        for (const auto& vi:weights) {
            std::cout<<"weights["<<i++<<"]"<<vi<<std::endl;
        }
#endif

        this->buildByFeatures(feat, weights);
    }

    void buildByFeatures(std::vector<T> &features, std::vector<int> &weights) {

        for (size_t i = weights.size(); i < features.size(); i++) {
            weights.push_back(1);
        }

        std::vector<long> values;
        std::vector<T> masks;
        const T one = 1;
        for (size_t i = 0; i < this->f; i++) {
            values.push_back(0);
            masks.push_back(one<<i);
        }
        auto wit = weights.begin();

        for (auto const &feature: features) {
            auto mask = masks.begin();
            for (auto &v: values) {
                v += feature & *mask ? *wit : -*wit;
                mask++;
            }
#ifdef DEBUG
            std::cout<<bigint::itoa(feature,10)<<"\t"<<*wit<<std::endl;
#endif
            wit++;
        }
        T ans = 0;
        auto mask = masks.begin();
        for (auto const &v: values) {
            if (v >= 0) ans |= *mask;
            mask++;
        }
        this->_value = ans;
        this->split();
    };

    unsigned distance(SimHashBase const &another) const override {
        if (this->dimension() != another.dimension())return -1;
        auto const &obj = dynamic_cast<SimHash<T> const &>(another);
        auto x = this->_value ^ obj._value;
#if defined(__SSE4_2__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(_M_X64)
#ifdef DEBUG
        std::cout << "distance by popcnt" << std::endl;
#endif
        switch(sizeof(T)){
            case 16:
                return __popcnt64(static_cast<__uint64_t>(x)) + __popcnt64(static_cast<__uint64_t>(x>>64));
            case 8:
                return __popcnt64(x);
            default:
                return __popcnt(x);
        }

#elif defined(__POPCNT__)
#ifdef DEBUG
        std::cout << "distance by builtin popcount" << std::endl;
#endif
        switch(sizeof(T)){
            case 16:
                return __builtin_popcountll(x);
            case 8:
                return __builtin_popcountl(x);
            default:
                return __builtin_popcount(x);
        }
#else
	/**
#ifdef DEBUG
        std::cout << "use _mm_popcnt_u32" << std::endl;
#endif
        switch(sizeof(T)){
   	    case 16:
#if defined(__AVX512BW__) && defined(__AVX512DQ__)	
                return _mm_popcnt_u128(x);
#else
		break;
#endif
            case 8:
                return _mm_popcnt_u64(x);
            default:
                return _mm_popcnt_u32((unsigned int)x);
        }
#else

#ifndef __AVX512BW__
#ifndef __AVX512DQ__
	*/
#ifdef DEBUG
        std::cout << "distance by counting" << std::endl;
#endif
        unsigned ans = 0;
        while (x) {
            ans += 1;
            x &= x - 1;
        }
        return ans;
//#endif
//#endif
#endif
    };

    const std::vector<unsigned> &getParts() {
        return this->parts;
    }

private:
    void split() override {
        auto base = (static_cast<T>(1) << this->hash_bit) - 1;
        auto n = this->f / this->hash_bit;
        for (size_t i = 0; i < n; i++) {
            if (this->_value == 0) {
                this->parts.push_back(0);
                continue;
            }
            auto c = i * this->hash_bit;
            auto base_ = base << c;
            this->parts.push_back(static_cast<unsigned>((this->_value & base_) >> c));
        }
    }

    void applyValue(std::string const &value, int base) override {
        this->_value = bigint::atoi(value.c_str(), this->_value, base);
#ifdef  DEBUG
        std::cout << "value: " << value << std::endl << "real value: " << bigint::itoa(this->_value) << std::endl;
#endif
    }
};


#endif //SIMHASH_SIMHASH_HPP
