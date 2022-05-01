//
// Created by Baofeng Shii on 2018/8/8.
//

#include "SimHashBase.h"

SimHashBase::SimHashBase(std::string const &s, unsigned int hash_bit, int base) : hash_bit(hash_bit) {

}

SimHashBase::SimHashBase(unsigned int hash_bit) : hash_bit(hash_bit) {
}


bool SimHashBase::similar(SimHashBase const &another, int count, unsigned int distance) const {
    if (this->dimension() != another.dimension())return false;
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
