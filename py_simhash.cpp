//
// Created by Baofeng Shii on 2018/8/1.
//

#include <Python.h>
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

template<typename T>
class SimHashPy : public SimHash<T> {
public:
    SimHashPy(std::string const &s, unsigned int f = 128, unsigned int hash_bit = 16) : SimHash<T>(s, f, hash_bit) {

    }

    SimHashPy(unsigned int f = 128, unsigned int hash_bit = 16) : SimHash<T>(f, hash_bit) {

    }

    void build(list &features, list &weights) {
        std::vector <T> f;
        std::vector<int> w;
        const T t = 0;
        boost::python::ssize_t n = boost::python::len(features);
        std::string token;
        for (boost::python::ssize_t i = 0; i < n; i++) {
            token = boost::python::extract<std::string>(features[i]);
            f.push_back(bigint::atoi(token.c_str(), t, 10));
        }
        n = boost::python::len(weights);

        for (boost::python::ssize_t i = 0; i < n; i++) {
            w.push_back(boost::python::extract<int>(weights[i]));
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
        class_<SimHashPy<__int128>>("SimHash", init<std::string, int, int>())
                .def(init<int, int>())
                .def("build", &SimHashPy<__int128>::build)
                .def("hex", &SimHashPy<__int128>::hex)
                .def("similar", &SimHashPy<__int128>::is_similar)
                .def("distance", &SimHashPy<__int128>::get_distance)
                .def("parts", &SimHashPy<__int128>::PartList)
                .def("value", &SimHashPy<__int128>::string);
}
