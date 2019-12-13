#ifndef UTILS_H
#define UTILS_H
#include "fmt/core.h"
#include <functional>
#include <memory>
#include <exception>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <unordered_set>
#include <chrono>
struct Community;
struct EgoNet;
using namespace std;
using fmt::print;
using IntPair=pair<int,int>;
using EdgeVec=vector<IntPair>;
struct FileOpenException: public exception{
    virtual const char* what() const noexcept {
        return "File open failed";
    }
};
struct ScopedTimer{
    chrono::high_resolution_clock::time_point start;
    ScopedTimer(){ start = chrono::high_resolution_clock::now(); }
    ~ScopedTimer(){
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        print("duration {} us\n",duration.count());
    }
};


void readGraphFile(const string& path,vector<IntPair>& edges);
void outputEgoNetToFile(const vector<EgoNet>& egoNets,string path);
template <typename T1,typename T2>
ostream& operator<< (ostream& os,const pair<T1,T2>& p){
    os<<"("<<p.first<<","<<p.second<<")";
    return os;
}
template <typename T>
void printContainer(const T& container,char delimeter=' '){
    for_each(container.begin(),container.end(),[=](typename T::const_reference a){
        cout<<a<<delimeter;
        //print("{}{}",a,delimeter);
    });
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
//template<typename R, typename... Args >
//R runningTime(R (*f)(Args...), Args&&... args)
template <typename Functor, typename ... Args>
auto runningTime(Functor f, Args && ... args)
    -> decltype(f(std::forward<Args>(args)...))
{
    //return std::function<R(Args...)>(f);
    ScopedTimer timer;
    return f(forward<Args>(args)...);
}

inline auto getRandomFunc(int min, int max) -> function<mt19937::result_type()>
{
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<mt19937::result_type> dist(min,max);
    return bind(dist,mt);
}

void mergeCommunitiesDeprecated(vector<Community> &communities);
void mergeSmallCommunities(vector<Community> &communities);

void fastMergeCommunities(vector<Community>& communities);
void mergeTwoCommus(vector<Community>& left,vector<Community>& right, vector<Community>& result);
#endif // UTILS_H
