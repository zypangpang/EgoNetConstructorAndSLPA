#ifndef UTILS_H
#define UTILS_H
#include "fmt/core.h"
#include <functional>
#include <memory>
#include <exception>
#include <vector>
#include <string>
#include <random>
#include <unordered_set>
struct Community;
using namespace std;
using fmt::print;
using randFuncType=function<mt19937::result_type()>;
using IntPair=pair<int,int>;
using EdgeVec=vector<IntPair>;
struct FileOpenException: public exception{
    virtual const char* what() const noexcept {
        return "File open failed";
    }
};

void readGraphFile(const string& path,vector<IntPair>& edges);
void outputEgoNetToFile(const vector<EdgeVec>& egoNets,string path);

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
template <typename T>
bool isSubset(const T& set1,const T& set2){
    const T *pset1=&set1,*pset2=&set2;
    if(set1.size()<set2.size()) swap(pset1,pset2);

    unordered_set<typename T::value_type> tset;
    for(const auto& x: (*pset1)){
        tset.insert(x);
    }
    for(const auto& x: (*pset2)){
        if(tset.count(x)==0)
            return false;
    }
    return true;
}
inline randFuncType getRandomFunc(int min, int max){
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<mt19937::result_type> dist(min,max);
    return bind(dist,mt);
}
void mergeCommunities(vector<Community> &communities);
#endif // UTILS_H
