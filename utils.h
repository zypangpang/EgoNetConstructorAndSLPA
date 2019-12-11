#ifndef UTILS_H
#define UTILS_H
#include "fmt/core.h"
#include <iostream>
#include <exception>
#include <vector>
#include <string>

using fmt::print;
using IntPair=std::pair<int,int>;
using EdgeVec=std::vector<IntPair>;
struct FileOpenException: public std::exception{
    virtual const char* what() const noexcept {
        return "File open failed";
    }
};

void readGraphFile(const std::string& path,std::vector<IntPair>& edges);
void outputEgoNetToFile(const std::vector<EdgeVec>& egoNets,std::string path);

template <typename T>
void printContainer(const T& container,char delimeter=' '){
    for_each(container.begin(),container.end(),[=](typename T::const_reference a){print("{}{}",a,delimeter);});
}
template <typename T>
inline void fillContainer(T& container, int begin=0,int step=1){
    int i=begin;
    for(auto it=container.begin();it!=container.end();++it){
        (*it)=i;
        i+=step;
    }
}
#endif // UTILS_H
