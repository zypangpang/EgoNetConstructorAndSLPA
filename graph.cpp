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

Graph::Graph(const EdgeVec &edges)
{
    vid.clear();
    genVertexIdMap(edges);
    G.resize(vid.size());
    for_each(vid.begin(),vid.end(),[&](const IntPair& p){G[p.second].id=p.first;});
    for(const auto& e: edges){
        addEdge(vid[e.first],vid[e.second]);
    }
}

void Graph::genVertexIdMap(const EdgeVec& edges){
    for(auto e : edges){
        int u=e.first, v=e.second;
        if(vid.count(u)==0)
            vid[u]=static_cast<int>(vid.size());
        if(vid.count(v)==0)
            vid[v]=static_cast<int>(vid.size());
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
    for(int u=0;u<G.size();++u){
        const auto& node=G[u];
        for(auto v:node.adjSet){
            if(u<v){
                print("{} {}\n",node.id,G[v].id);
            }
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

//Get the ego-nets.
void Graph::getEgoNets(vector<EgoNet>& egoNets,bool withEgoNode) const{
    vector<Triangle> tris;
    enumTriangle(tris);
    print("finish enumerating triangles\n");
    egoNets.resize(G.size());
    for(int i=0;i<egoNets.size();++i){
        egoNets[i].egoid=oid(i);
    }
    for(auto tri: tris){
        int u=get<0>(tri);
        int v=get<1>(tri);
        int z=get<2>(tri);
        egoNets[u].edges.push_back({oid(v),oid(z)});
        egoNets[v].edges.push_back({oid(u),oid(z)});
        egoNets[z].edges.push_back({oid(u),oid(v)});
    }
    if(withEgoNode){
        unordered_set<int> vset;
        for(int u=0;u<egoNets.size();++u){
            vset.clear();
            int sz=egoNets[u].edges.size();
            for(int i=0;i<sz;++i){ // Attention! Cannot use auto here, because edges have changed in the body.
                auto e=egoNets[u].edges[i];
                int v=e.first,z=e.second;
                if(vset.count(v)==0){
                    egoNets[u].edges.push_back({oid(u),v});
                    vset.insert(v);
                }
                if(vset.count(z)==0){
                    egoNets[u].edges.push_back({oid(u),z});
                    vset.insert(z);
                }
            }
        }
    }
}
