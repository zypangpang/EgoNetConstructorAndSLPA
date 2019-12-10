#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include "utils.h"
#include "graph.h"
#define DEBUG

using namespace std;
void genVertexIdMap(const vector<IntPair>& edges, unordered_map<int,int>& vid){
    for(auto e : edges){
        int u=e.first, v=e.second;
        if(vid.count(u)==0)
            vid[u]=static_cast<int>(vid.size());
        if(vid.count(v)==0)
            vid[v]=static_cast<int>(vid.size());
    }
}
shared_ptr<Graph> genTestGraph(int vn){
    auto g=make_shared<Graph>(vn);
    for(int step=1;step<3;++step){
        for(int i=0;i<vn;i+=step){
            g->addEdge(i,(i+step)%vn);
        }
    }
    return g;
}

int main()
{
#ifdef DEBUG
    int vn=5;
    auto g=genTestGraph(vn);
    g->outputG();
    vector<Triangle> tris;
    g->enumTriangle(tris);
    for(auto tri: tris){
        print("({},{},{})\n",get<0>(tri),get<1>(tri),get<2>(tri));
    }
#else
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
#endif
    return 0;
}
