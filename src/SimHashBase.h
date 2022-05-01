//
// Created by Baofeng Shii on 2018/8/8.
//

#ifndef SIMHASH_CPP_SIMHASHBASE_H
#define SIMHASH_CPP_SIMHASHBASE_H

#include <iostream>
#include <boost/python.hpp>
#include <vector>


using namespace boost::python;

class SimHashBase {
public:
    unsigned int hash_bit;
    std::vector<unsigned> parts;

public:
    SimHashBase(std::string const &s, unsigned int hash_bit = 16, int base=16);

    explicit SimHashBase(unsigned int hash_bit = 16);

    virtual ~SimHashBase() {};

    virtual std::string string()const=0;

    inline std::string value() const{
        return this->string();
    }

    virtual std::string hex()const=0;

    virtual unsigned dimension()const=0;

    bool similar(SimHashBase const &another, int count, unsigned int distance) const;

    virtual void build(list &features, list &weights, int base)=0;

    virtual unsigned distance(SimHashBase const &another)const=0;

    inline const std::vector<unsigned> &getParts() const{
        return this->parts;
    }

private:
    virtual void applyValue(std::string const &v, int base)=0;

    virtual void split()=0;
};

#endif //SIMHASH_CPP_SIMHASHBASE_H
