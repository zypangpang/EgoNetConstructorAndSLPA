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
#include "SLPA.h"
#define DEBUG
using namespace std;

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
    vector<Community> allCommunities;
    for(int i=0;i<egoNets.size();++i){
        auto net=egoNets[i];
        print("----Ego-net for node {}:\n", i);
        for(auto e: net){
            print("{}->{}\n",e.first,e.second);
        }
        auto slpa=Slpa::getInstance(Graph(net));
        slpa->SLPA();
        //slpa->outputLabelMem();
        vector<Community> communities;
        slpa->getCommunity(communities);
        using vvint_iter=decltype(communities.begin());
        allCommunities.insert(allCommunities.end(),move_iterator<vvint_iter>(communities.begin()),
                              move_iterator<vvint_iter>(communities.end()));
        /*slpa->getNodeCommunity(communities);
        for(const auto& c : communities){
            print("Community:\n");
            printContainer(c);
            print("\n");
        }
        using vvint_iter=decltype(communities.begin());
        allCommunities.insert(allCommunities.end(),move_iterator<vvint_iter>(communities.begin()),
                              move_iterator<vvint_iter>(communities.end()));
        vector<EdgeVec> EdgeCommunities;
        slpa->getEdgeCommunity(EdgeCommunities);
        for(const auto& c : EdgeCommunities){
            print("Edge Community:\n");
            for(const auto& p:c){
                print("{}->{}\n",p.first,p.second);
            }
            print("\n");
        }*/
    }
    vector<int> avlLabels;
    mergeCommunities(allCommunities);
    print("all communities:\n");
    for(const auto& c : allCommunities){
            print("Community:\n");
            print("nodes:\n");
            printContainer(c.nodes);
            print("\n");
            print("edges:\n");
            for(const auto& p:c.edges){
                print("{}->{}\n",p.first,p.second);
            }
            print("\n");
    }
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
