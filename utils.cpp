#include <fstream>
#include <sstream>
#include <fmt/core.h>
#include <set>
#include "utils.h"

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
