//
//  graph.cpp
//  Graph_Summarization
//
//  Created by Mehedi on 9/23/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#include "graph.hpp"


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
    initialize_Node_Vector(); // initialize Node Vection with just id upto 0 to n-1
    g = GraphType(n, vector<vertex*>());
    string graphFile = graph_data_folder + DirectorySeparator + "graph.txt";
    if(file_exists_at_path(graphFile)){
        FILE *Fin = fopen(graphFile.c_str(), "r");
        int t1, t2;
        while (~fscanf(Fin, "%d%d", &t1, &t2)) {
            assert(t1 < n);
            assert(t2 < n);
            if(t1 != t2){
                NodeVector[t1]->setNodeID(t1); // t1 is the node id, NodeVector[t1] is the pointer to that node
                NodeVector[t2]->setNodeID(t2);
                //For other Node Attributes can set here;
                
                g[t1].push_back(NodeVector[t1]);
                g[t2].push_back(NodeVector[t2]); // undirected graph
                
            }
        }
    }
}
void Graph::initialize_Node_Vector(){
    //Set Node Vector With vertex ID
    NodeVector = vector<vertex*>(n);
    for (long i=0; i<n; i++) {
        NodeVector[i] = new vertex(i);
    }
}
