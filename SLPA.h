#ifndef SLPA_H
#define SLPA_H
#include "graph.h"
#include "utils.h"
struct Community{
    vector<int> nodes;
    EdgeVec edges;
};

class Slpa {
    static Slpa* instance;
public:
    static Slpa* getInstance(Graph&& g,int T=20,double r=0.1){
        //print("Gsize: {}\n",g.getVertexNum());
        if(instance){
            instance->setParam(move(g),T,r);
            return instance;
        }
        instance=new Slpa(move(g),T,r);
        return instance;
    }
public:
    void setParam(Graph&& g,int tT,double tr){
        G=move(g);T=tT;r=tr;
        labelMem.resize(G.getVertexNum());
      //  avlLabels.clear();
    }
    void getCommunity(vector<Community>& communities,bool withEdges=false);
    //Only for DEBUG
    void outputLabelMem();
private:
    Slpa(Graph&& g,int tT,double tr):G(move(g)),r(tr),T(tT){
        labelMem.resize(G.getVertexNum());
     //   avlLabels.clear();
    }
    Slpa(const Slpa&)=delete;
    Slpa operator=(const Slpa&)=delete;

    ~Slpa()=default;

    int speakerRule(const vector<int>& labels);
    int listenerRule(const vector<int>& labels);
    void removeLabels(vector<int>& labels);
    /*bool checkLabel(int l){
        return avlLabels.count(l);
    }*/

    void getNodeCommunity(unordered_map<int,Community>& communities);
    void getEdgeCommunity(unordered_map<int, Community> &communities);
public:
    void SLPA();

private:
    Graph G;
    vector<vector<int>> labelMem;
    //unordered_set<int> avlLabels;
    double r;
    int T;
};
#endif // SLPA_H
