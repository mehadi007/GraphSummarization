//
//  config.cpp
//  Graph_Summarization
//
//  Created by Mehedi on 8/28/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#include "config.hpp"

//Configuration Class Extern
Config config;
Result result;


long Graph::getN(){
    return n;
}
long long Graph::getM(){
    return m;
}
void Graph::setN(long temp){
    n=temp;
}
void Graph::setM(long long temp){
    m = temp;
}

void Graph::getNM() {
    string attribute_file = graph_data_folder + "attribute.txt";
    if(file_exists_at_path( attribute_file)){
        ifstream attrStream(attribute_file);
        char ss;
        while (1) {
            attrStream >> ss;
            if (ss == '=')
                break;
        }
        attrStream >> n;
        while (1) {
            attrStream >> ss;
            if (ss == '=')
                break;
        }
        attrStream >> m;
    }else{
        cout<<"No Attribute File Exist"<<endl;
    }
}

//Initialize adjacency List 
void Graph::init_graph() {
    getNM(); //get n & m
    g = vector<vector<int>>(n, vector<int>());
    string graphFile = graph_data_folder + DirectorySeparator + "graph.txt";
    if(file_exists_at_path(graphFile)){
        FILE *Fin = fopen(graphFile.c_str(), "r");
        int t1, t2;
        while (~fscanf(Fin, "%d%d", &t1, &t2)) {
            assert(t1 < n);
            assert(t2 < n);
            if(t1 != t2){
                g[t1].push_back(t2);
                g[t2].push_back(t1); // undirected graph
            }
        }
    }
}

void Result::printSuperNode_A(const set<long>&A){
    set<long>::iterator it;
    for (it = A.begin(); it!=A.end(); ++it) {
        if (it == A.begin()) {
            cout<<*it;
        }else{
            cout<<", "<<*it;
        }
    }
}

// Print Super Nodes S
void Result::printSuperNodes(){
    printSplitLine();
    cout<<"## Size of SuperNodes S: "<<S.size();
    int i=1;
    for(auto A: S){
        cout<<"\nSuper Node ["<<i++<<"] : ";
        printSuperNode_A(A);
    }
    printSplitLine();
}

void Result::printSuperEdges(){
    printSplitLine();
    cout<<"## Size of SuperEdges P: "<<P.size();
    set< pair< set<long>, set<long> > >::iterator it;
    int i=1;
    for (it=P.begin(); it!=P.end(); ++it) {
        cout<<"\nSuper Edge ["<<i++<<"] : ";
        cout<<'{';printSuperNode_A((*it).first); cout<<'}';
        cout<<" - ";
        cout<<'{';printSuperNode_A((*it).second); cout<<'}';
    }
    printSplitLine();
}

void Result::printCorrections(){
    printSplitLine();
    cout<<"## Size of Correction C+: "<<C_plus.size();
    set< pair<long, long>>::iterator it;
    int i=1;
    for (it = C_plus.begin(); it != C_plus.end(); ++it) {
        cout<<"\n C+ ["<<i++<<"] : "<<(*it).first<<"-"<<(*it).second;
    }
    
    
    
    cout<<"\n## Size of Correction C-: "<<C_minus.size();
    i=1;
    for (it = C_minus.begin(); it != C_minus.end(); ++it) {
        cout<<"\n C- ["<<i++<<"] : "<<(*it).first<<"-"<<(*it).second;
    }
    printSplitLine();
}
void Result::printResult_Data(){
    cout<<Red<<"<Printing Result>"<<Reset;
    printSuperNodes();
    printSuperEdges();
    printCorrections();
}
