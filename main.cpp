#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <memory>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <set>
#include <sstream>
#include <charconv>
#include "omp.h"
#include "utils.h"
#include "graph.h"
#include "SLPA.h"
#define DEBUG
using namespace std;
constexpr int PARALLEL_NUM=12;
shared_ptr<Graph> genTestGraph(int vn){
    auto g=make_shared<Graph>(vn);
    for(int step=1;step<3;++step){
        for(int i=0;i<vn;i+=step){
            g->addEdge(i,(i+step)%vn);
        }
    }
    return g;
}
shared_ptr<Graph> genGraphFromFile(string path){
    vector<IntPair> edges;
    try{
        readGraphFile(path,edges);
    }catch(const exception& e){
        cout<<e.what()<<endl;
    }

    return make_shared<Graph>(edges);
}
void getAndWriteEgoNets(const Graph& g,string path){
    vector<EgoNet> egoNets;
    g.getEgoNets(egoNets,true);
    print("finish getting ego nets\n");
    outputEgoNetToFile(egoNets,path);
    print("finish writing to file\n");
}
void readEgoNetsFromFile(vector<EgoNet>& egoNets, const string& path){
    ifstream fin(path);
    if(!fin) throw FileOpenException();
    string line;
    EgoNet* curNet=nullptr;
    while(getline(fin,line)){
        if(line[0]=='#') {
            //if(egoNets.size()==10000) break; //Only for DEBUG
            egoNets.push_back(EgoNet{stoi(line.substr(1)),EdgeVec()});
            curNet=&(egoNets.back());
        }
        else{
            string::size_type sz;
            int u=stoi(line,&sz);
            int v=stoi(line.substr(sz+1));
            curNet->edges.push_back({u,v});
        }
    }
}
void fastReadCommFromFile(vector<vector<int>>& communities, const string& path){
    ifstream fin(path);
    if(!fin) throw FileOpenException();
    string line;
    while(getline(fin,line)){
        communities.emplace_back();
        auto & curComm=communities.back();
        string_view line_view(line.c_str());
        string::size_type sz=0;
        while(sz<line_view.size()){
            line_view=line_view.substr(sz);
            sz=line_view.find_first_of(' ');
            int n;
            from_chars(line_view.data(),line_view.data()+sz,n);
            curComm.push_back(n);
            ++sz;
        }
    }
}
void readCommFromFile(vector<vector<int>>& communities,string const& path){
    ifstream fin(path);
    if(!fin) throw FileOpenException();
    string line;
    while(getline(fin,line)){
        communities.emplace_back();
        auto & curComm=communities.back();
        stringstream ss(move(line));
        int n;
        while(ss>>n) {
            curComm.push_back(n);
        }
    }
}
void writeCommunityToFile(vector<Community> const& community, string const& path){
    ofstream fout(path);
    if(!fout) throw FileOpenException();
    for(auto const& c: community){
        string line("#");
        auto const& nodes=c.nodes;
        for(auto n: nodes){
            line+=to_string(n);
            line+=' ';
        }
        fout<<line<<endl;
        for(auto const& e: c.edges){
            fout<<e.first<<" "<<e.second<<endl;
        }
    }
}
pair<double,double> computeAccuracy(vector<vector<int>> const& commAlg,vector<vector<int>> const& commTruth, vector<int>& commGetId){
    vector<bool> maskAlg(commAlg.size(),false);
    for(int i=0;i<commTruth.size();++i){
        auto const& commT=commTruth[i];
        for(int j=0;j<commAlg.size();++j){
            if(maskAlg[j]) continue;
            auto const& commA=commAlg[j];
            if(commA.size()<commT.size())
                continue;
            if(isSubset(commA,commT)){
                maskAlg[j]=true;
                commGetId.push_back(i);
                break;
            }
        }
    }
    //{Precision, Recall}
    return {commGetId.size()/double(commAlg.size()),commGetId.size()/double(commTruth.size())};
}
int main()
{
#ifdef DEBUG
    vector<vector<int>> commAlg,commTruth;
    readCommFromFile(commAlg,"communitiesOnlyNode.txt");
    readCommFromFile(commTruth,"amazon.top5000.cmty.txt");
    vector<int> getId;
    auto acr=computeAccuracy(commAlg,commTruth,getId);
    cout<<acr<<endl;
    return 0;

    omp_set_num_threads(4);
#pragma omp parallel
    {
    int id=omp_get_thread_num();
    print("hello {}",id);
    print("world {}\n",id);
    }
    return 0;

    int vn=5;
    auto g=genTestGraph(vn);
    g->outputG();
    cout<<endl;
    vector<EgoNet> egoNets;
    g->getEgoNets(egoNets,true);
    vector<Community> allCommunities;
    for(const auto& egoNet: egoNets){
        print("----Ego-net for node {}:\n", egoNet.egoid);
        for(auto e: egoNet.edges){
            print("{}->{}\n",e.first,e.second);
        }
        Slpa slpa(Graph(egoNet.edges));
        slpa.SLPA();
        //slpa->outputLabelMem();
        vector<Community> communities;
        slpa.getCommunity(communities);
        using vvint_iter=decltype(communities.begin());
        allCommunities.insert(allCommunities.end(),move_iterator<vvint_iter>(communities.begin()),
                              move_iterator<vvint_iter>(communities.end()));
        /*slpa->getNodeCommunity(communities);
        for(const auto& c : communities){
            print("Community:\n");
            printContainer(c);
            print("\n");
        }
        using vvint_iter=decltype(communities.begin());
        allCommunities.insert(allCommunities.end(),move_iterator<vvint_iter>(communities.begin()),
                              move_iterator<vvint_iter>(communities.end()));
        vector<EdgeVec> EdgeCommunities;
        slpa->getEdgeCommunity(EdgeCommunities);
        for(const auto& c : EdgeCommunities){
            print("Edge Community:\n");
            for(const auto& p:c){
                print("{}->{}\n",p.first,p.second);
            }
            print("\n");
        }*/
    }
    mergeCommunities(allCommunities);
    print("all communities:\n");
    for(const auto& c : allCommunities){
            print("Community:\n");
            print("nodes:\n");
            printContainer(c.nodes);
            print("\n");
            print("edges:\n");
            for(const auto& p:c.edges){
                print("{}->{}\n",p.first,p.second);
            }
            print("\n");
    }
#else
    //auto g=genGraphFromFile("data/Amazon/amazon.ungraph.txt");
    //print("finish gen G\n");

    //getAndWriteEgoNets("egoNets.txt");
    constexpr auto egoNetFilePath="egoNets_oriId.txt";
    vector<EgoNet> egoNets;
    runningTime(readEgoNetsFromFile,egoNets,egoNetFilePath);
    print("finish reading egonets\n");

    omp_set_num_threads(PARALLEL_NUM);
    vector<Community> allCommunities[PARALLEL_NUM];
    using vvint_iter=decltype(allCommunities[0].begin());
    auto getAllcommunities=[&](){
        #pragma omp parallel for
        for(int i=0;i<egoNets.size();++i){
            int thid=omp_get_thread_num();
            auto const& net=egoNets[i];
            if(net.edges.empty()) continue;
            //auto slpa=Slpa::getInstance(Graph(net.edges),10,0.1);
            Slpa slpa(Graph(net.edges));
            slpa.SLPA();
            //slpa->outputLabelMem();
            vector<Community> communities;
            slpa.getCommunity(communities,false);
           // #pragma omp critical
            allCommunities[thid].insert(allCommunities[thid].end(),move_iterator<vvint_iter>(communities.begin()),
                                  move_iterator<vvint_iter>(communities.end()));
        }
    };
    runningTime(getAllcommunities);
    print("finish get all communities\n");

    vector<Community> finalCommunities;
    auto mergeAllCommunities=[&](){
        #pragma omp parallel for
        for(int i=0;i<PARALLEL_NUM;++i){
            //print("all community size: {}\n",allCommunities[i].size());
            fastMergeCommunities(allCommunities[i]);
            //print("all community size: {}\n",allCommunities[i].size());

        }
    };
    runningTime(mergeAllCommunities);
    print("finish merge all communities\n");
    for(int i=0;i<PARALLEL_NUM;++i){
        finalCommunities.insert(finalCommunities.end(),move_iterator<vvint_iter>(allCommunities[i].begin()),
                                  move_iterator<vvint_iter>(allCommunities[i].end()));

    }
    print("final communities size: {}\n",finalCommunities.size());

    runningTime(writeCommunityToFile,finalCommunities,"communities.txt");
    print("finish writing all communities\n");

#endif
    return 0;
}
