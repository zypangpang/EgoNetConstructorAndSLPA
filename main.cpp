#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include "utils.h"
#include "graph.h"

using namespace std;
void genVertexIdMap(const vector<IntPair>& edges, unordered_map<int,int>& vid){
    for(auto e : edges){
        int u=e.first;
        int v=e.second;
        if(vid.count(u)==0)
            vid[u]=static_cast<int>(vid.size());
        if(vid.count(v)==0)
            vid[v]=static_cast<int>(vid.size());
    }
}
int main()
{
    vector<int> degreeOrder(5);
    accumulate(degreeOrder.begin(),degreeOrder.end(),0,[](int prev,int& cur)->int{cur=prev; return cur+1;});
    for_each(degreeOrder.begin(),degreeOrder.end(),[](int a){print("{} ",a);});
    return 0;
    int vn=0;
    vector<IntPair> edges;
    try{
        readGraphFile("data/Amazon/amazon.ungraph.txt",edges);
    }catch(const exception& e){
        cout<<e.what()<<endl;
    }
    unordered_map<int,int> vid;
    genVertexIdMap(edges,vid);
    print("vn: {}\n",vid.size());

    Graph g(vid.size());
    for_each(edges.begin(),edges.end(),[&](const IntPair& e){g.addEdge(vid[e.first],vid[e.second]);});
    g.outputG();
    return 0;
}
