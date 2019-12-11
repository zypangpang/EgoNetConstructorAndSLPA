#include <iostream>
#include <algorithm>
#include <numeric>
#include "fmt/core.h"
#include "graph.h"
#include "utils.h"

using fmt::print;
Graph::Graph(int n)
{
    G.resize(n);
    //vertexNum=n;
    for(int i=0;i<n;++i){
        G[i].id=i;
    }
}

void Graph::addEdge(int u, int v)
{
    if(G[u].adjSet.count(v)>0)
        return;
    G[u].adjSet.insert(v);
    G[v].adjSet.insert(u);
    G[u].degree++;
    G[v].degree++;
}

void Graph::outputG() const
{
    for(const auto& node: G){
        //print("#{}:\n",node.id);
        for(auto v:node.adjSet){
            if(node.id<v)
                print("{} {}\n",node.id,v);
        }
    }
}

void Graph::enumTriangle(vector<Triangle> &triangles) const
{
    vector<int> degreeOrder(G.size());
    fillContainer(degreeOrder);
    //accumulate(degreeOrder.begin(),degreeOrder.end(),0,
    //           [](int prev,int& cur)->int{cur=prev; return cur+1;});
    sort(degreeOrder.begin(),degreeOrder.end(),[&](int a, int b){return G[a].degree<G[b].degree;});
    vector<bool> mask(G.size(),false);
    for(auto u: degreeOrder){
        const auto& Nu=G[u].adjSet;
        for(auto vit=Nu.begin();vit!=Nu.end();++vit){
            auto v=*vit;
            auto zit=vit;
            for(++zit;zit!=Nu.end();++zit){
                auto z=*zit;
                if(v==z||mask[v]||mask[z]) continue;
                if(G[v].adjSet.count(z)){
                    triangles.push_back(make_tuple(u,v,z));
                }
            }
        }
        mask[u]=true;
    }
}
