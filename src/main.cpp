#include <iostream>
#include <string>
#include <vector>
#include "search.hpp"
#include "vector_store_io.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {


        cout << "Usage:\n"<< "  vector_search <base_vectors.fvecs>\n";
            
        return 1;
    }

    string base_file = argv[1];

    try {

        cout << "Loading dataset-\n";

        auto dataset =
            VectorStoreIO::read_vecs<float>(
                base_file
            );

        cout<< "Loaded "<< dataset.size()<< " vectors\n";

        if (dataset.empty()) {
            cout << "Dataset is empty\n";
            return 1;
        }

        cout<< "Dimension: "<< dataset[0].dimension()<< "\n";

     
        const auto& query =
            dataset[0].vector;

        size_t k = 100;

        cout << "Running flat search with k = "<< k<< "...\n";

        auto results =
            flat_search(dataset,query,k);

        cout << "Returned " << results.size()<< " results\n";

        cout << "\nNearest vector IDs:\n";

        for (size_t i = 0;i < results.size();++i)
        {
            cout  << i + 1 << ": "<< results[i]<< "\n";
        }

    }
    catch (const exception& e) {

        cerr  << "ERROR: "<< e.what() << "\n";
            
        return 1;
    }

    return 0;
}