//
// Created by Baofeng Shii on 2018/8/1.
//

#include <Python.h>
#include <vector>
#include <boost/python.hpp>
#include "SimHash.h"

using namespace boost::python;
//
//template<typename T>
//int distance(const SimHash<T> *self, const SimHash<T> *rhs) {
//    return self->distance(*rhs);
//}

template<typename T>
class SimHashPy : public SimHash<T> {
public:
    SimHashPy(std::string const &s, unsigned int f = 128, unsigned int hash_bit = 16) : SimHash<T>(s, f, hash_bit) {

    }

    SimHashPy(unsigned int f = 128, unsigned int hash_bit = 16) : SimHash<T>(f, hash_bit) {

    }

    void build(list &features, list &weights) {
        std::vector<T> f;
        std::vector<float> w;
        for (auto v:f) {
            f.push_back(v);
        }
        for (auto v:w) {
            w.push_back(v);
        }
        this->buildByFeatures(f, w);
    }

    list PartList() {
        list lst;
        auto parts = this->getParts();
        for (auto v:parts) {
            lst.append(v);
        }
        return lst;
    }

    bool is_similar(SimHashPy<T> const &another, int count, int distance) {
        return this->similar(another, count, distance);
    }

    unsigned get_distance(SimHashPy<T> const &another) {
        return this->distance(another);
    }
};

BOOST_PYTHON_MODULE (pysimhash) {
    class_<SimHashPy<unsigned __int128>>("SimHash", init<std::string, int, int>())
            .def(init<int, int>())
            .def("build", &SimHashPy<unsigned __int128>::build)
            .def("hex", &SimHashPy<unsigned __int128>::hex)
            .def("similar", &SimHashPy<unsigned __int128>::is_similar)
            .def("distance", &SimHashPy<unsigned __int128>::get_distance)
            .def("parts", &SimHashPy<unsigned __int128>::PartList)
            .def("value", &SimHashPy<unsigned __int128>::string);
}
