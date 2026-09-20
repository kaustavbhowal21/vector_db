cmake_minimum_required(VERSION 3.15)

project(VectorSearch
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)



add_executable(vector_search
    src/main.cpp
    src/search.cpp
)

add_executable(test_phase0
    tests/test_phase0.cpp
    src/search.cpp
)