//
// Created by Baofeng Shii on 2018/8/8.
//

#include "SimHash.hpp"

/**
bool SimHashBase::similar(SimHashBase const &another, int count, unsigned int distance) const {
    if (this->dimension() != another.dimension())return false;
    if (this == &another) {
        return true;
    }

    auto it = this->parts.begin();
    auto ait = another.parts.begin();
    while (it != this->parts.end() && ait != another.parts.end()) {
        if (*it == *ait) {
            count--;
            if (0 == count) {
                return this->distance(another) <= distance;
            }
        }
        it++;
        ait++;
    }
    return false;
}
*/