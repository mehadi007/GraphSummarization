//
//  graph.hpp
//  Graph_Summarization
//
//  Created by Mehedi on 9/23/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include "utility.hpp"


//Forward Declarations
class vertex;
class SuperNode;
class Graph;

//typeDefs
typedef vector<vector<vertex*>> GraphType;
typedef vector<vector<vertex*>>::iterator GraphType_It;
typedef vector<vertex*> NodeVec;
typedef vector<vertex*>::iterator NodeVec_It;
typedef set <vertex*> Set_Nodes;
typedef set <vertex*>::iterator Set_Nodes_It;

//-----------------Edge Struct-----------------//
//Declare the Weighted Edge
struct wEdge
{
    int a;
    int b;
    int weight;
    wEdge(int a_, int b_, int weight_):a(a_), b(b_), weight(weight_){}
    
    bool operator<(const wEdge &b) const
    {
        return weight < b.weight;
        
    }
};

struct Edge
{
    int a;
    int b;
    Edge(int a_, int b_):a(a_), b(b_){}
};
//-----------------Edge Struct-----------------//


//-----------------Custom Vertex---------------//
class vertex{
private:
    long nodeID;
    //string nodeTitle;
    //Add Other Node Attributes
public:
    vertex(long id=-1){
        nodeID = id;
    }
    long getNodeID() const{
        return nodeID;
    }
    void setNodeID(long temp){
        nodeID = temp;
    }
};

//-----------------Custom SuperNode---------------//
class SuperNode{
   
public:
    int S_id; //SuperNode ID
    Set_Nodes S_Node; //Set of Vertex pointer is A SuperNode
};

//set<SuperNode*>S; //superNodes
//S.insert = New
//set<pair< SuperNode*, SuperNode* > > superEdge;

//Remove
//1. superNode Object using pointer
//2. from S remove pointer

class Graph {
private:
    long n;  // Number of Nodes  valid nodeID:(0 to n-1)
    long long m;   //Number of Edges
    void getNM();
    void init_graph();
    void initialize_Node_Vector();
public:
    long getN();
    long long getM();
    void setN(long);
    void setM(long long);
    NodeVec NodeVector; // Store pointer to the Vertex | for all in G
    GraphType g; //Adjacency List representation
    string graph_data_folder;
    static bool cmp(const pair<int, double> &t1, const pair<int, double> &t2) {
        return t1.second > t2.second;
    }
    Graph(string data_folder) {
        cout<<"Graph Constructor"<<endl;
        this->graph_data_folder = data_folder;
        init_graph();
        cout<<"N = "<<n<< "   M = "<< m <<endl;
    }
    ~Graph(){
        cout<<"Graph Destructor"<<endl;
        
        for (int i=0; i<NodeVector.size(); i++) {
            delete NodeVector[i]; //delete the actual vertex object
        }
        for (long i=0;i<n;i++){
            for(long j=0;j<g[i].size();j++){
                delete g[i][j];
            }
        }
    }
    
};





#endif /* graph_hpp */
