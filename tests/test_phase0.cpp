#include <cassert>
#include <iostream>
#include <string>
#include <cstdint>
#include "search.hpp"
#include "vector_store_io.hpp"

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr<< "Usage:\n"<< "  test_phase0 <base_vectors.fvecs> <query_vectors.fvecs> <groundtruth.ivecs>\n";

        return 1;
    }

    std::string base_file = argv[1];
    std::string query_file = argv[2];
    std::string groundtruth_file = argv[3];

    try {

        std::cout<< " Phase 0 Tests \n\n";

        std::cout<< "1. Loading dataset--\n";

        auto dataset = VectorStoreIO::read_vec<float>(base_file);
        auto queries = VectorStoreIO::read_vec<float>(query_file);

        auto ground_truth =VectorStoreIO::read_vec<int32_t>(groundtruth_file);
        assert(queries.size() == 100);
        assert(ground_truth.size() == 100);
      

        std::cout<< "    Loaded: "<< dataset.size()<< " vectors\n";

        assert(
            dataset.size() == 10000
        );



        std::cout<< "2 Checking dimensions--\n";

        for (const auto& record : dataset) {

            assert(record.dimension() == 128);
        }

        for (const auto& record : queries) {
        
            assert(record.dimension() == 128); 
}
        for (const auto& record : ground_truth) {
   
            assert(record.dimension() == 100);
}



        std::cout<< "    All vectors have dimension 128\n";


   
        std::cout<< "3. Checking IDs--\n";

        for (size_t i = 0;i < dataset.size();++i)
        {
            assert(
                dataset[i].id == static_cast<int>(i)
            );
        }

        std::cout<< "   IDs are correct\n";



        std::cout<< "4. Testing squared L2--\n";

        std::vector<float> a = {1.0f,2.0f,3.0f};

        std::vector<float> b = {2.0f,4.0f,6.0f};

        float distance =
            squared_l2(a, b);


        assert(
            distance == 14.0f
        );

        std::cout<< "    squared L2 works\n";


   

        std::cout<< "5. Testing flat_search--\n";

      double total_recall = 0.0;

for (size_t i = 0; i < queries.size(); ++i) {
    auto results =
        flat_search(dataset, queries[i].vector, 100);

    double recall =
        recall_at_k(
            results,
            ground_truth[i].vector,
            100
        );

    total_recall += recall;
}
double average_recall = total_recall / queries.size();

std::cout << "Average Recall@100: " << average_recall << "\n";

assert(average_recall == 1.0);
    

    

        std::cout<< "    flat_search works\n";


       std::cout << "\n ALL PHASE 0 TESTS PASSED\n";

    }
    catch (const std::exception& e) {

        std::cerr<< "\nERROR: "<< e.what()<< "\n";

        return 1;
    }

    return 0;
}