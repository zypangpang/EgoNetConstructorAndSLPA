#include <fstream>
#include <sstream>
#include <fmt/core.h>
#include <iterator>
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
        string::size_type sz;
        int u=stoi(line,&sz);
        int v=stoi(line.substr(sz+1));
        edges.push_back({u,v});
    }
}

void outputEgoNetToFile(const vector<EgoNet>& egoNets,string path){
    ofstream fout(path);
    if(!fout) throw FileOpenException();
    for(const auto& net: egoNets){
        string str=fmt::format("#{}:",net.egoid);
        fout<<str<<endl;
        for(auto e: net.edges){
            str=fmt::format("{} {}",e.first,e.second);
            fout<<str<<endl;
        }
    }
}
void mergeSmallCommunities(vector<Community> &communities){
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
void mergeCommunities(vector<Community> &communities)
{
    constexpr int MERGE_THRESHOLD=2000;
    if(communities.size()<MERGE_THRESHOLD){
        mergeSmallCommunities(communities);
        return;
    }
    using vvint_iter=decltype(communities.begin());
    vector<vector<Community>> commArray;
    auto it=communities.begin();
    auto end=communities.end();
    while(true){
        if(end-it<MERGE_THRESHOLD){
            commArray.push_back(vector<Community>(move_iterator<vvint_iter>(it),
                                                  move_iterator<vvint_iter>(end)));
            break;
        }else{
            commArray.push_back(vector<Community>(move_iterator<vvint_iter>(it),
                                        move_iterator<vvint_iter>(it+MERGE_THRESHOLD)));
            it+=MERGE_THRESHOLD;
        }
    }
    communities.clear();
    for(auto& comm: commArray){
        mergeSmallCommunities(comm);
        communities.insert(communities.end(),move_iterator<vvint_iter>(comm.begin()),
                                  move_iterator<vvint_iter>(comm.end()));
    }
}
