//
// Created by Baofeng Shii on 2018/8/1.
//

#define PYBIND11_DETAILED_ERROR_MESSAGES
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include "SimHash.hpp"
#include "bigint.hpp"

namespace py = pybind11;


#ifdef _MSC_VER
bool waring_128 = true;
void warn128(){ 
    if(waring_128){ 
         std::cerr<<"128 bit not supported for this build, 64 bit is used."<<std::endl;
         waring_128=false;
    }
}
#endif

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

    void build(std::vector<std::string> &features, std::vector<int> &weights, int base) {
        this->s->build(features, weights, base);
    }

    std::vector<unsigned> PartList() {
        return this->s->getParts();
    }

    std::string string() {
        return this->s->string();
    }

    std::string hex() {
        return this->s->hex();
    }

    unsigned dimension() {
        return this->s->dimension();
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
#ifdef _MSC_VER
                warn128();
                return new SimHash<__uint64_t>(s, hash_bit, base);
#else
                return new SimHash<__uint128_t>(s, hash_bit, base);
#endif
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
#ifdef _MSC_VER
                warn128();
                return new SimHash<__uint64_t>(hash_bit);
#else
                return new SimHash<__uint128_t>(hash_bit);
#endif
        }
    }
};


PYBIND11_MODULE (pysimhash,m) {

    py::class_<SimHashPy>(m,"SimHash")
            .def(py::init<std::string, unsigned, unsigned, int>(),
            py::arg("s"), py::arg("f"), py::arg("hash_bit"), py::arg("base"))
            .def(py::init<unsigned, unsigned>(), py::arg("f"), py::arg("hash_bit"))
            .def("build",
                 &SimHashPy::build,
                 py::arg("features"), py::arg("weights") = std::vector<int>(), py::arg("base") = 16,
                 "build hash with features\n"
                 "features: feature with type of string, indicate a number\n"
                 "weights: weights of features with the same length, "
                 "if len(weights)<len(features), 1 is taken as the missing weights\n"
                 "base: base of feature, default 16"
            )
            .def("hex", &SimHashPy::hex, "simhash as hex string")
            .def("dimension", &SimHashPy::dimension, "simhash dimension")
            .def("similar", &SimHashPy::is_similar,
            py::arg("another"), py::arg("count"), py::arg("distance"),
             "check if this hash is similar with another")
            .def("distance", &SimHashPy::get_distance,py::arg("another"),
             "get the distance between this hash and another")
            .def("parts", &SimHashPy::PartList, "return parts of the hash value")
            .def("value", &SimHashPy::string, "simhash as decimal string");
}
