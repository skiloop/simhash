//
// Created by Baofeng Shii on 2018/8/1.
//

#include <vector>
#include <boost/python.hpp>
#include "SimHash.hpp"
#include "bigint.hpp"

using namespace boost::python;
//
//template<typename T>
//int distance(const SimHash<T> *self, const SimHash<T> *rhs) {
//    return self->distance(*rhs);
//}

class SimHashPy {
private:
    SimHashBase *s;
public:
    SimHashPy(std::string const &s, unsigned f = 128, unsigned int hash_bit = 16) {
        this->s = this->getSimHashObject(s, f, hash_bit);
//        std::cout << "f: " << f << std::endl;
        assert(this->s != nullptr);
    }

    ~SimHashPy() {
        delete this->s;
    }

    SimHashPy(unsigned f = 128, unsigned int hash_bit = 16) {
        this->s = this->getSimHashObject(f, hash_bit);
//        std::cout << "f: " << f << std::endl;
        assert(this->s != nullptr);
    }

    void build(list &features, list &weights) {
        this->s->build(features, weights);
    }

    list PartList() {
        list lst;
        auto parts = this->s->getParts();
        for (auto v:parts) {
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

    static SimHashBase *getSimHashObject(std::string const &s, unsigned f, unsigned hash_bit) {
        switch (f) {
            case 128:
                return new SimHash<unsigned __int128>(s, hash_bit);
            case 64:
                return new SimHash<__uint64_t>(s, hash_bit);
            case 32:
                return new SimHash<__uint32_t>(s, hash_bit);
            case 16:
                return new SimHash<__uint16_t>(s, hash_bit <= 16 ? hash_bit : 16);
            case 8:
                return new SimHash<__uint8_t>(s, hash_bit <= 8 ? hash_bit : 8);
            default:
                return nullptr;
        }
    }

    static SimHashBase *getSimHashObject(unsigned f, unsigned hash_bit) {
        switch (f) {
            case 128:
                return new SimHash<unsigned __int128>(hash_bit);
            case 64:
                return new SimHash<__uint64_t>(hash_bit);
            case 32:
                return new SimHash<__uint32_t>(hash_bit);
            case 16:
                return new SimHash<__uint16_t>(hash_bit <= 16 ? hash_bit : 16);
            case 8:
                return new SimHash<__uint8_t>(hash_bit <= 8 ? hash_bit : 8);
            default:
                return nullptr;
        }
    }
};


BOOST_PYTHON_MODULE (pysimhash) {
    class_<SimHashPy>("SimHash", init<std::string, unsigned, unsigned>())
            .def(init<unsigned, unsigned>())
            .def("build", &SimHashPy::build)
            .def("hex", &SimHashPy::hex)
            .def("similar", &SimHashPy::is_similar)
            .def("distance", &SimHashPy::get_distance)
            .def("parts", &SimHashPy::PartList)
            .def("value", &SimHashPy::string);
}
