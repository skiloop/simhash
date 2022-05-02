//
// Created by Baofeng Shii on 2018/8/1.
//

#include <vector>
#include <boost/python.hpp>
#include "SimHash.hpp"
#include "bigint.hpp"

using namespace boost::python;

class SimHashPy {
private:
    SimHashBase *s;
public:
    SimHashPy(std::string const &s, unsigned f = 128, unsigned int hash_bit = 16, int base = 16) {
        this->s = this->getSimHashObject(s, f, hash_bit, base);
    }

    ~SimHashPy() {
        delete this->s;
    }

    SimHashPy(unsigned f = 128, unsigned int hash_bit = 16) {
        this->s = this->getSimHashObject(f, hash_bit);
    }

    void build(list &features, list &weights, int base) {
        this->s->build(features, weights, base);
    }

    list PartList() {
        list lst;
        auto parts = this->s->getParts();
        for (auto v: parts) {
            lst.append(v);
        }
        return lst;
    }

    std::string string() {
        return this->s->string();
    }

    std::string hex() {
        return this->s->hex();
    }

    bool is_similar(SimHashPy const &another, int count, int distance) {
        return this->s->similar(*(another.s), count, distance);
    }

    unsigned get_distance(SimHashPy const &another) {
        return this->s->distance(*(another.s));
    }

    static SimHashBase *getSimHashObject(std::string const &s, unsigned f, unsigned hash_bit, int base = 16) {
        switch (f) {
            case 64:
                return new SimHash<__uint64_t>(s, hash_bit, base);
            case 32:
                return new SimHash<__uint32_t>(s, hash_bit, base);
            case 16:
                return new SimHash<__uint16_t>(s, hash_bit <= 16 ? hash_bit : 16, base);
            case 8:
                return new SimHash<__uint8_t>(s, hash_bit <= 8 ? hash_bit : 8, base);
            case 128:
            default:
                return new SimHash<unsigned __int128>(s, hash_bit, base);
        }
    }

    static SimHashBase *getSimHashObject(unsigned f, unsigned hash_bit) {
        switch (f) {
            case 64:
                return new SimHash<__uint64_t>(hash_bit);
            case 32:
                return new SimHash<__uint32_t>(hash_bit);
            case 16:
                return new SimHash<__uint16_t>(hash_bit <= 16 ? hash_bit : 16);
            case 8:
                return new SimHash<__uint8_t>(hash_bit <= 8 ? hash_bit : 8);
            case 128:
            default:
                return new SimHash<unsigned __int128>(hash_bit);
        }
    }
};

//BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS (f_build, build, 1, 3)
BOOST_PYTHON_MODULE (pysimhash) {

    class_<SimHashPy>("SimHash", init<std::string, unsigned, unsigned, int>())
            .def(init<unsigned, unsigned>())
            .def("build", &SimHashPy::build, (arg("features"), arg("weights") = list(), arg("base") = 16),
                 "build hash with features\n"
                 "features: feature with type of string, indicate a number\n"
                 "weights: weights of features with the same length, if len(weights)<len(features), 1 is taken as the missing weights\n"
                 "base: base of feature, default 16"
            )
            .def("hex", &SimHashPy::hex, "simhash as hex string")
            .def("similar", &SimHashPy::is_similar, "check if this hash is similar with another")
            .def("distance", &SimHashPy::get_distance, "get the distance between this hash and another")
            .def("parts", &SimHashPy::PartList, "return parts of the hash value")
            .def("value", &SimHashPy::string, "simhash as decimal string");
}
