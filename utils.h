#ifndef UTILS_H
#define UTILS_H
#include "fmt/core.h"
#include <iostream>
#include <exception>
#include <vector>
#include <string>

using fmt::print;
using IntPair=std::pair<int,int>;
struct FileOpenException: public std::exception{
    virtual const char* what() const noexcept {
        return "File open failed";
    }
};

void readGraphFile(const std::string& path,std::vector<IntPair>& edges);

#endif // UTILS_H
