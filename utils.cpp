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
    vector<bool> mask(communities.size(),false);
    for(int i=0;i<communities.size();++i){
        for(int j=i+1;j<communities.size();++j){
            if(mask[i]) break;
            if(mask[j]) continue;
            auto const& ci=communities[i].nodes;
            auto const& cj=communities[j].nodes;
            if(isSubset(ci,cj)){
                if(ci.size()>cj.size()) mask[j]=true;
                else mask[i]=true;
            }
        }
    }
    vector<Community> tcomm;
    for(int i=0;i<communities.size();++i){
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
void mergeTwoCommus(vector<Community>& left,vector<Community>& right, vector<Community>& result){
    vector<bool> mask1(left.size(),false);
    vector<bool> mask2(right.size(),false);
    for(int i=0;i<left.size();++i){
        for(int j=0;j<right.size();++j){
            if(mask1[i]) break;
            if(mask2[j]) continue;
            auto const& lc=left[i].nodes;
            auto const& rc=right[j].nodes;
            if(isSubset(lc,rc)){
                if(lc.size()>rc.size()) mask2[j]=true;
                else mask1[i]=true;
            }
        }
    }
    result.clear();
    for(int i=0;i<mask1.size();++i){
        if(!mask1[i]) result.push_back(move(left[i]));
    }
    for(int i=0;i<mask2.size();++i){
        if(!mask2[i]) result.push_back(move(right[i]));
    }

}
void fastMergeCommunities(vector<Community>& communities){
     if(communities.size()<10){
         mergeSmallCommunities(communities);
         return;
     }
     auto it=communities.begin();
     auto end=communities.end();
     auto mid=it+communities.size()/2;
     using cit_type=decltype (it);
     vector<Community> left_commus,right_commus;
     left_commus.insert(left_commus.end(),move_iterator<cit_type>(it),
                                  move_iterator<cit_type>(mid));
     right_commus.insert(right_commus.end(),move_iterator<cit_type>(mid),
                                  move_iterator<cit_type>(end));
     fastMergeCommunities(left_commus);
     fastMergeCommunities(right_commus);
     mergeTwoCommus(left_commus,right_commus,communities);
}
