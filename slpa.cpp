#include <algorithm>
#include <random>
#include "utils.h"
#include "SLPA.h"
Slpa* Slpa::instance=nullptr;
void Slpa::SLPA(){
    //Init
    auto n=G.getVertexNum();
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

void Slpa::getCommunity(vector<EdgeVec> &communities)
{
    communities.clear();
    unordered_map<int,EdgeVec> commMap;
    auto vn=G.getVertexNum();
    for(int u=0;u<vn;++u){
        unordered_set<int> uSet;
        for(auto l: labelMem[u]){
            uSet.insert(l);
        }
        const auto& uNode=G[u];
        for(auto v: uNode.adjSet){
            if(u>v) continue;
            for(auto l: labelMem[v]){
                if(uSet.count(l)) {
                    if(commMap.count(l)){
                        commMap[l].push_back({u,v});
                    }
                    else{
                        commMap[l]=EdgeVec{{u,v}};
                    }
                }
            }
        }
    }
    for(const auto& pr: commMap){
        communities.push_back(pr.second);
    }
}

void Slpa::outputLabelMem()
{
    for(int i=0;i<labelMem.size();++i){
        print("----Node id {}:\n",i);
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
