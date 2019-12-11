#ifndef SLPA_H
#define SLPA_H
#include "graph.h"
#include "utils.h"
class Slpa {
    static Slpa* instance;
public:
    static Slpa* getInstance(Graph&& g,int T=10,double r=0.1){
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
    }
    void getCommunity(vector<EdgeVec>& communities);
    //Only for DEBUG
    void outputLabelMem();
private:
    Slpa(Graph&& g,int tT,double tr):G(move(g)),r(tr),T(tT){
        print("Gsize: {}\n",G.getVertexNum());
        labelMem.resize(G.getVertexNum());
    }
    Slpa(const Slpa&)=delete;
    Slpa operator=(const Slpa&)=delete;

    ~Slpa()=default;

    int speakerRule(const vector<int>& labels);
    int listenerRule(const vector<int>& labels);
    void removeLabels(vector<int>& labels);
public:
    void SLPA();

private:
    Graph G;
    vector<vector<int>> labelMem;
    double r;
    int T;
};
#endif // SLPA_H
