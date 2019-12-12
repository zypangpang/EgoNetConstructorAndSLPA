#include <algorithm>
#include <random>
#include "utils.h"
#include "SLPA.h"
//Slpa* Slpa::instance=nullptr;
void Slpa::SLPA(){
    //Init
    auto n=G.getVertexNum();
    if(n==0) return;
    for(int i=0;i<n;++i)
        labelMem[i].push_back(i);
    //Evolution
    vector<int> order(n);
    fillContainer(order);
    for(int i=0;i<T;++i){
        random_shuffle(order.begin(),order.end());
        for(auto i : order){
            const auto& listener=G[i];
            const auto& speakers=listener.adjSet;
            vector<int> labelList;
            for(int speaker: speakers){
                labelList.push_back(speakerRule(labelMem[speaker]));
            }
            labelMem[i].push_back(listenerRule(labelList));
        }
    }
    // Post-processing
    for(auto& labels: labelMem){
        removeLabels(labels);
    }
}

void Slpa::getCommunity(vector<Community> &communities, bool withEdges)
{
    if(G.getVertexNum()==0) return;
    unordered_map<int,Community> map_communities;
    getNodeCommunity(map_communities);
    if(withEdges)
        getEdgeCommunity(map_communities);
    for(const auto& pr: map_communities){
        communities.push_back(move(pr.second));
    }
}

void Slpa::getNodeCommunity(unordered_map<int, Community> &res_communities)
{
    unordered_map<int,vector<int>> commMap;
    for(int u=0;u<labelMem.size();++u){
        const auto& labels=labelMem[u];
        for_each(labels.begin(),labels.end(),[&](int l){
            if(commMap.count(l)) commMap[l].push_back(G[u].id);
            else commMap[l]=vector<int>{G[u].id};
        });
    }
    vector<int> labels;
    //vector<vector<int>> communities;
    for(const auto& comm : commMap){
        labels.push_back(comm.first);
        //communities.push_back(move(comm.second));
    }
    vector<int> order(labels.size());
    vector<bool> mask(labels.size(),false);
    fillContainer(order);
    sort(order.begin(),order.end(),[&](int a,int b){return commMap[labels[a]].size()>commMap[labels[b]].size();});
    for(int i=0;i<order.size()-1;++i){
        if(mask[order[i]]) continue;
        for(int j=i+1;j<order.size();++j){
            if(mask[order[j]]) continue;
            auto l1=labels[order[i]];
            auto l2=labels[order[j]];
            if(isSubset(commMap[l1],commMap[l2])){
                mask[order[j]]=true;
            }
        }
    }
    //avlLabels.clear();
    res_communities.clear();
    for(int i=0;i<labels.size();++i){
        if(!mask[i]){
            auto l=labels[i];
            //avlLabels.insert(labels[i]);
            res_communities[l]=Community{move(commMap[l]),EdgeVec()};
        }
    }
}

void Slpa::getEdgeCommunity(unordered_map<int,Community> &communities)
{
    if(communities.empty()){
        getNodeCommunity(communities);
    }
    unordered_map<int,EdgeVec> commMap;
    auto vn=G.getVertexNum();
    for(int u=0;u<vn;++u){
        unordered_set<int> uSet;
        for(auto l: labelMem[u]){
            if(communities.count(l))
                uSet.insert(l);
        }
        const auto& uNode=G[u];
        for(auto v: uNode.adjSet){
            if(u>v) continue;
            for(auto l: labelMem[v]){
                if(communities.count(l) && uSet.count(l)) {
                    if(commMap.count(l)){
                        commMap[l].push_back({G[u].id,G[v].id});
                    }
                    else{
                        commMap[l]=EdgeVec{{G[u].id,G[v].id}};
                    }
                }
            }
        }
    }
    for(const auto& pr: commMap){
        communities[pr.first].edges=move(pr.second);
    }
}

void Slpa::outputLabelMem()
{
    for(int i=0;i<labelMem.size();++i){
        print("----Node id {}:\n",G[i].id);
        printContainer(labelMem[i]);
        print("\n");
    }
}

int Slpa::speakerRule(const vector<int> &labels)
{
     auto dice=getRandomFunc(0,labels.size()-1);
     return labels[dice()];
}

int Slpa::listenerRule(const vector<int> &labels)
{
    unordered_map<int,int> labelCount;
    for(auto l: labels){
        if(labelCount.count(l))
            ++labelCount[l];
        else
            labelCount[l]=1;
    }
    int mostFreqLabel = 0;
    int maxFreq=-1;
    for(const auto& pr: labelCount){
        auto l=pr.first,c=pr.second;
        if(c>maxFreq) {
            maxFreq=c;
            mostFreqLabel=l;
        }
    }
    return mostFreqLabel;
}

void Slpa::removeLabels(vector<int> &labels)
{
    unordered_map<int,int> labelCount;
    for(auto l: labels){
        if(labelCount.count(l))
            ++labelCount[l];
        else
            labelCount[l]=1;
    }
    auto sum=labels.size();
    labels.clear();
    int mostFreqLabel;
    int maxFreq=-1;
    for(auto pr: labelCount){
        auto l=pr.first,c=pr.second;
        if(c>maxFreq) {
            maxFreq=c;
            mostFreqLabel=l;
        }
        double p=double(c)/sum;
        if(p>=r) labels.push_back(l);
    }
    if(labels.empty())
        labels.push_back(mostFreqLabel);
}
