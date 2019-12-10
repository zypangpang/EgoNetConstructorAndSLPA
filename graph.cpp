#include <iostream>
#include <algorithm>
#include <numeric>
#include "fmt/core.h"
#include "graph.h"

using fmt::print;
Graph::Graph(int n)
{
    G.resize(n);
    vertexNum=n;
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
    for(auto node: G){
        print("---- Node {}:\n",node.id);
        for(auto v:node.adjSet){
            print("{}->{}\n",node.id,v);
        }
    }
}

void Graph::enumTriangle(vector<Triangle> &triangles)
{
    vector<int> degreeOrder(G.size());
    accumulate(degreeOrder.begin(),degreeOrder.end(),0,
               [](int prev,int& cur)->int{cur=prev; return cur+1;});
}
