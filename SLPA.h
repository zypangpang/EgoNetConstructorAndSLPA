#ifndef SLPA_H
#define SLPA_H
#include "graph.h"
class Slpa {
    static Slpa* instance;
public:
    static Slpa* getInstance(const Graph& g,int T,int r){
        if(instance){
            instance->setParam(g,T,r);
            return instance;
        }
        instance=new Slpa(g,T,r);
        return instance;
    }
public:
    void setParam(const Graph& g,int tT,int tr){G=g;T=tT;r=tr;}
    void SLPA();
private:
    Slpa(const Graph& g,int tT,int tr):G(g),T(tT),r(tr){};
    Slpa(const Slpa&)=delete;
    Slpa operator=(const Slpa&)=delete;

    ~Slpa()=default;

    int speakerRule(const vector<int>& labels);
    int listenerRule(const vector<int>& labels);

private:
    Graph G;
    int T,r;
    vector<vector<int>> labelMem;

};
#endif // SLPA_H
