#pragma once
#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "vector_record.hpp"



class VectorStoreIO {
public : 
template <typename T>
static std::vector<VectorRecord<T>>
read_vecs(const std::string& filename)
{
  
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Could not open file: " + filename
        );
    }

    std::vector<VectorRecord<T>> records;


    int id = 0;


    while (true){
        int32_t dimension;

        
      file.read(
    reinterpret_cast<char*>(&dimension),
    sizeof(dimension)
);

if (file.gcount() == 0 && file.eof()) {
    break;
}

if (file.gcount() != sizeof(dimension)) {
    throw std::runtime_error(
        "Incomplete vector dimension in: " + filename
    );
}


        if (dimension <= 0) 
        {

            throw std::runtime_error(

                "Invalid vector dimension in: " + filename

            );
        }

        VectorRecord<T> record;

        record.id = id;

        record.vector.resize(
           
            static_cast<std::size_t>(dimension)
        );

        const std::streamsize bytes =
            static_cast<std::streamsize>(
                
                static_cast<std::size_t>(dimension) * sizeof(T)
            );

        file.read(
            reinterpret_cast<char*>(record.vector.data()),
            bytes
        );

        if (!file) {
            throw std::runtime_error(
                "Unexpected end of file while reading vector."
            );
        }

        records.push_back(std::move(record));

        ++id;
    }

    return records;
}

};