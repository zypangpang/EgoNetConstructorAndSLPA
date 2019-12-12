#ifndef GRAPH_H
#define GRAPH_H
#include <unordered_set>
#include <vector>
#include <tuple>
#include "utils.h"
using namespace std;
using Triangle=tuple<int,int,int>;
struct Node {
    int degree=0;
    int id;
    unordered_set<int> adjSet;
};
struct EgoNet{
    int egoid;
    EdgeVec edges;
};

class Graph {
    vector<Node> G;
    unordered_map<int,int> vid;
    //int vertexNum=0;

    void genVertexIdMap(const EdgeVec& edges);
    void enumTriangle(vector<Triangle>& triangles) const;
    int oid(int vertexId)const {return G[vertexId].id;}

public:
    Graph(int n);
    Graph(Graph&& rhs){G=move(rhs.G); vid=move(rhs.vid);}
    Graph& operator=(Graph&& rhs){
        G=move(rhs.G);
        vid=move(rhs.vid);
        return *this;
    }
    Graph(const EdgeVec& edges);

    void getEgoNets(vector<EgoNet>& egoNets,bool withEgoNode) const;
    void addEdge(int u,int v);

    void outputG() const;

    vector<Node>::size_type getVertexNum() const {return G.size();}
    const Node& operator[](vector<Node>::size_type i) const {
        return G[i];
    }

};

#endif // GRAPH_H
