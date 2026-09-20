#pragma once

#include <cstddef>
#include <vector>

#include "vector_record.hpp"


float squared_l2(
    const std::vector<float>& a,
    const std::vector<float>& b
);

// brute force kNN search
std::vector<int> flat_search(
    const std::vector<VectorRecord<float>>& dataset,
    const std::vector<float>& query,
    size_t k
);


double recall_at_k(
    const std::vector<int>& predicted,
    const std::vector<int>& ground_truth,
    size_t k
);