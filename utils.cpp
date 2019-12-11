#include <fstream>
#include <sstream>
#include <fmt/core.h>
#include <set>
#include "utils.h"
#include "SLPA.h"

using namespace std;

void readGraphFile(const string& path,vector<IntPair>& edges){
    ifstream fin(path);
    if(!fin) throw FileOpenException();
    string line;
    while(getline(fin,line)){
        if(line[0]=='#')
            continue;
        stringstream ss(line);
        int u,v;
        ss>>u>>v;
        edges.push_back({u,v});
    }
}

void outputEgoNetToFile(const vector<EdgeVec>& egoNets,string path){
    ofstream fout(path);
    if(!fout) throw FileOpenException();
    for(int i=0;i<egoNets.size();++i){
        string str=fmt::format("#{}:",i);
        fout<<str<<endl;
        for(auto e: egoNets[i]){
            str=fmt::format("{} {}",e.first,e.second);
            fout<<str<<endl;
        }
    }
}

void mergeCommunities(vector<Community> &communities)
{
    vector<int> order(communities.size());
    fillContainer(order);
    vector<bool> mask(order.size(),false);
    sort(order.begin(),order.end(),[&](int a,int b){return communities[a].nodes.size()>communities[b].nodes.size();});
    for(int i=0;i<order.size()-1;++i){
        if(mask[order[i]]) continue;
        for(int j=i+1;j<order.size();++j){
            if(mask[order[j]]) continue;
            auto l1=order[i];
            auto l2=order[j];
            if(isSubset(communities[l1].nodes,communities[l2].nodes)){
                mask[l2]=true;
            }
        }
    }
    vector<Community> tcomm;
    for(int i=0;i<order.size();++i){
        if(!mask[i]){
            tcomm.push_back(move(communities[i]));
        }
    }
    communities=move(tcomm);
}
