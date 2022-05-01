//
// Created by Baofeng Shii on 2018/7/27.
//

#ifndef SIMHASH_SIMHASH_HPP
#define SIMHASH_SIMHASH_HPP

#include <iostream>
#include <vector>

#include "bigint.hpp"
#include "SimHashBase.h"

template<typename T>
class SimHash : public SimHashBase {
    static const unsigned int f = sizeof(T) * 8;
    T _value;

public:
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

    void build(list &features, list &weights, int base) override {
        std::vector<T> feat;
        std::vector<int> w;
        const T t = 0;
        boost::python::ssize_t n = boost::python::len(features);
        std::string token;
        for (boost::python::ssize_t i = 0; i < n; i++) {
            token = boost::python::extract<std::string>(features[i]);
            feat.push_back(bigint::atoi(token.c_str(), t, base));
#ifdef DEBUG
            std::cout<<"features["<<i<<"]"<<token<<","<<bigint::itoa(feat[i], base)<<std::endl;
#endif
        }
        n = boost::python::len(weights);

        for (boost::python::ssize_t i = 0; i < n; i++) {
            w.push_back(boost::python::extract<int>(weights[i]));
#ifdef DEBUG
            std::cout<<"weights["<<i<<"]"<<w[i]<<std::endl;
#endif
        }

        this->buildByFeatures(feat, w);
    }

    void buildByFeatures(std::vector<T> &features, std::vector<int> &weights) {

        for (size_t i = weights.size(); i < features.size(); i++) {
            weights.push_back(1);
        }

        std::vector<long> values;
        for (size_t i = 0; i < this->f; i++) {
            values.push_back(0);
        }
        const T one = 1;
        auto wit = weights.begin();
        for (auto const &feature: features) {
            auto mask = one;
            for (auto &v: values) {
                v += feature & mask ? *wit : -*wit;
                mask <<= 1;
            }
#ifdef DEBUG
            std::cout<<bigint::itoa(feature,10)<<"\t"<<*wit<<std::endl;
#endif
            wit++;
        }
        T ans = 0;
        auto mask = one;
        for (auto const &v: values) {
            if (v >= 0) ans |= mask;
            mask <<= 1;
        }
        this->_value = ans;
        this->split();
    };

    unsigned distance(SimHashBase const &another) const override {
        if (this->dimension() != another.dimension())return -1;
        auto const &obj = dynamic_cast<SimHash<T> const &>(another);
        auto x = this->_value ^ obj._value;
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
    void split() override {
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

    void applyValue(std::string const &value, int base) override {
        this->_value = bigint::atoi(value.c_str(), this->_value, base);
#ifdef  DEBUG
        std::cout << "value: " << value << std::endl << "real value: " << bigint::itoa(this->_value) << std::endl;
#endif
    }
};


#endif //SIMHASH_SIMHASH_HPP
