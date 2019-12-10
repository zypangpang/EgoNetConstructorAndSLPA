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

template <typename T>
void printContainer(const T& container,char delimeter=' '){
    for_each(container.begin(),container.end(),[=](typename T::const_reference a){print("{}{}",a,delimeter);});
}

#endif // UTILS_H
