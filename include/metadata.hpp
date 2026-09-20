#pragma once


#include <any>
#include <string>
#include <unordered_map>
//using namespace std;

struct Metadata {
    std::unordered_map<std::string, std::any> data;
};