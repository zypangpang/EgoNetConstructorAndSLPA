#ifndef GRAPH_H
#define GRAPH_H
#include <unordered_set>
#include <vector>
#include <tuple>
using namespace std;
using Triangle=tuple<int,int,int>;
struct Node {
    int degree=0;
    int id;
    unordered_set<int> adjSet;
};

class Graph {
    vector<Node> G;
    //int vertexNum=0;
public:
    Graph(int n);
    void addEdge(int u,int v);
    void outputG() const;
    void enumTriangle(vector<Triangle>& triangles) const;
    vector<Node>::size_type getVertexNum() const {return G.size();}
    const Node& operator[](vector<Node>::size_type i) const {
        return G[i];
    }

};

#endif // GRAPH_H
