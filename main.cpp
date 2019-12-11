#include <iostream>
#include <string>
#include <cstdio>
#include <memory>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <set>
#include "utils.h"
#include "graph.h"
#define DEBUG
using namespace std;
void genVertexIdMap(const EdgeVec& edges, unordered_map<int,int>& vid){
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
//Get the ego-nets.
void getEgoNets(const vector<Triangle>& tris, vector<EdgeVec>& egoNets,bool withEgoNode){
    for(auto tri: tris){
        int u=get<0>(tri);
        int v=get<1>(tri);
        int z=get<2>(tri);
        egoNets[u].push_back({v,z});
        egoNets[v].push_back({u,z});
        egoNets[z].push_back({u,v});
    }
    if(withEgoNode){
        unordered_set<int> vset;
        for(int u=0;u<egoNets.size();++u){
            vset.clear();
            int sz=egoNets[u].size();
            for(int i=0;i<sz;++i){
                auto e=egoNets[u][i];
                int v=e.first,z=e.second;
                if(vset.count(v)==0){
                    egoNets[u].push_back({u,v});
                    vset.insert(v);
                }
                if(vset.count(z)==0){
                    egoNets[u].push_back({u,z});
                    vset.insert(z);
                }
            }
        }
    }
}
int main()
{
#ifdef DEBUG
    int vn=5;
    auto g=genTestGraph(vn);
    g->outputG();
    cout<<endl;
    vector<Triangle> tris;
    g->enumTriangle(tris);
    vector<EdgeVec> egoNets(g->getVertexNum());
    getEgoNets(tris,egoNets,true);
    for(int i=0;i<egoNets.size();++i){
        auto net=egoNets[i];
        print("----Ego-net for node {}:\n", i);
        for(auto e: net){
            print("{}->{}\n",e.first,e.second);
        }
    }
    //outputEgoNetToFile(egoNets,"egoNets.txt");
#else
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
    print("finish gen G\n");
    g.outputG();
    return 0;

    vector<Triangle> tris;
    g.enumTriangle(tris);
    print("finish enumerate triangles\n");
    vector<EdgeVec> egoNets(g.getVertexNum());
    getEgoNets(tris,egoNets,true);
    print("finish getting ego nets\n");
    outputEgoNetToFile(egoNets,"egoNets.txt");
    print("finish writing to file\n");
#endif
    return 0;
}
