#pragma once
#include <cstddef> //to Define size_t.
#include <vector>
#include "metadata.hpp"
//using namespace std;

template <typename T>
struct VectorRecord {

    int id;

    std::vector<T> vector;
    Metadata metadata;

    size_t dimension() const {
        return vector.size();
    }



};