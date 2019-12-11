#include <algorithm>
#include "utils.h"
#include "SLPA.h"

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

}

int Slpa::speakerRule(const vector<int> &labels)
{

}

int Slpa::listenerRule(const vector<int> &labels)
{

}
