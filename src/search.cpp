#include "search.hpp"
#include <algorithm>
#include <cmath>
#include <queue>
#include <stdexcept>
#include <unordered_set>
#include <utility>

float squared_l2(
    const std::vector<float>& a,
    const std::vector<float>& b
)
{
    if (a.size() != b.size())
     {

        
        throw std::invalid_argument(
            "Vectors must have the same dimension"
        );
    }

    float sum = 0.0f;


    for (size_t i = 0; i < a.size(); ++i) 
    {

        float difference = a[i] - b[i];

        sum += difference * difference;
    }

    return sum;
}


std::vector<int> flat_search(
    const std::vector<VectorRecord<float>>& dataset,
    const std::vector<float>& query,
    size_t k
)
{
    if (k == 0 || dataset.empty()) {
        return {};
    }

    if (k > dataset.size()) {
        k = dataset.size();
    }


    
    std::priority_queue<
        std::pair<float, int>
    > candidates;

    for (const auto& record : dataset) {

        float distance = squared_l2(
            query,
            record.vector
        );

        if (candidates.size() < k) {

            
            candidates.emplace(
                distance,
                record.id
            );

        }
        else if (distance < candidates.top().first) {

            // New vector is better than
            // our current worst candidate.

            candidates.pop();

            candidates.emplace(
                distance,
                record.id
            );
        }
    }

    
        

      //   we reverse the result.
    

    std::vector<int> result;

    result.reserve(candidates.size());

    while (!candidates.empty()) {

        result.push_back(
            candidates.top().second
        );

        candidates.pop();
    }

    std::reverse(
        result.begin(),
        result.end()
    );

    return result;
}


double recall_at_k(
    const std::vector<int>& predicted,
    const std::vector<int>& ground_truth,
    size_t k
)
{
    if (k == 0 || ground_truth.empty()) {
        return 0.0;
    }

    size_t actual_k =
        std::min(
            k,
            std::min(
                predicted.size(),
                ground_truth.size()
            )
        );

    if (actual_k == 0) {
        return 0.0;
    }

    std::unordered_set<int> truth;

    for (size_t i = 0; i < actual_k; ++i) {
        truth.insert(ground_truth[i]);
    }

    size_t correct = 0;

    for (size_t i = 0; i < actual_k; ++i) {

        if (truth.count(predicted[i])) {
            ++correct;
        }
    }

    return static_cast<double>(correct)
         / static_cast<double>(actual_k);
}