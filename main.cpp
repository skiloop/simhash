#include <iostream>
#include <vector>
#include "bigint.hpp"
#include "SimHash.hpp"

int main() {
    const char *v = "2479530857526804504083961063697536543";

    unsigned __int128 n = 0;
//    n = bigint::atoi(v, n);
    n = ~n;
    std::cout << "size of int: " << sizeof(unsigned) << std::endl;
    std::cout << "size of long: " << sizeof(long) << std::endl;
    std::cout << "size of long long: " << sizeof(long long) << std::endl;
    std::cout << "number: " << v << std::endl;
    std::cout << "itoa: " << bigint::itoa(n) << std::endl;
    std::cout << "itoh: " << bigint::itoa(n, 16) << std::endl;
    std::cout << "0x9F56E itoh: " << bigint::itoa(0x9F56E, 16) << std::endl;
    unsigned __int128 c = bigint::atoi(v, (unsigned __int128) 0);
    std::vector<unsigned __int128> vec;
    for (auto i = 0; i < 10; i++) {
        vec.push_back(c);
        c -= 100;
    }
    std::vector<float> weight;
    SimHash<unsigned __int128> sim(128);
    sim.buildByFeatures(vec, weight);
    std::cout << sim.string() << "\n" << sim.hex() << std::endl;
    std::cout << "parts:" << std::endl;
    for (auto const &part:sim.getParts()) {
        std::cout << bigint::itoa(part) << std::endl;
    }
    return 0;
}