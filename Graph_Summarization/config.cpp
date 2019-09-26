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

