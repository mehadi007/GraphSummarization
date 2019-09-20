//
//  algo.cpp
//  Graph_Summarization
//
//  Created by Mehedi on 8/29/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#include "algo.hpp"


#pragma mark - Function Declarations

// Unordered Map Hash
typedef std::pair<long,long> Upair;
struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &Upair) const
    {
        return std::hash<T1>()(Upair.first) ^ std::hash<T2>()(Upair.second);
    }
};

//Global
void print_SuperNodeG(); //Printing the Disjoint group
void print_Bijective_Hash(const vector<long>&);


//------ Algo 1 Helpers----//
void initializeSupernodes(long);
void initilize_Hash_using_Vector(vector<long>&, long);

//------ Algo 2 Helpers----//
long Shingle_of_NodeV(const Graph& graph, long v, const vector<long>&H);
void shuffleHash(vector<long>&hash);
long myrandom (long i);
bool compareF_Score(pair< set<long>, long> i1, pair< set<long>, long> i2);
set<long> get_Neighbor_Set_of_SuperNode(set<long>&A, const Graph& graph);

//------Merger--algo3 Helpers--//
void merge_Based_on_Q(set<set<long>>&q,const Graph& graph, int t);
set<long> pickRandom_Item_From_Q(set<set<long>>&q);
double SuperJaccard(set<long>&A, set<long>&B, const Graph& graph);
double Saving(const Graph& graph, const set<long>&A,const set<long>&B, set<set<long>>SS);
void encoding_for_Saving_only(const Graph &graph, const set<set<long>> &SS, const set<long> &SuperA, set< pair<set<long>, set<long> >> &P, set< pair<long, long>> &C_plus, set< pair<long, long>> &C_minus);
long cost(const Graph& graph, const set<long>&A , set<set<long>>& SS);
void HashMap_Edges_of_SuperA(const Graph &graph,const set<long>&A, unordered_map<Upair, bool, pair_hash> &MP);
double mergingThreshold(int t);

//------Encoding--algo4 Helpers--//
void calculateEdges_in_AB(const Graph &graph, const set<long>&A, const set<long>&B, set<pair<long, long>>&E_AB, set<pair<long, long>>&Pi_AB );

//------Dropping--algo5 Helpers--//
void updateChangingLimit(const Graph &graph, long numberofNodes_inG, double epsilion, vector<double>&C_V);
void countSort_SuperEdges(long max, vector<long> &Size_AB, vector< pair< set<long>, set<long>>> &res);
void testCountSortByPrint(vector<long> &Size_AB);
void sortSuperEdges(const set< pair< set<long>, set<long> > > &P, vector< pair< set<long>, set<long>>> &res);
bool superNode_Limit_Checking(const Graph &graph, set<long> &SuperA, long limit);
void drop_Changing_Limit_SuperNode(vector<double>&C_V, set<long> &SuperA, long limit);
//============================================================

//Algorithm-1
#pragma mark - Algorithms
void OverviewSummarization(Graph &graph){
    initializeSupernodes(graph.getN());
    
    vector<long>H; //Hash
    initilize_Hash_using_Vector(H, graph.getN());
    
    for (int t = 1; t<=config.max_iteration_T; t++) {
        cout<< Green <<"\n============== Start T = "<< t <<" ==================\n"<<Reset;
        //cout<<"\nIteration t="<<t<<endl;
        Create_Disjoint_Groups(graph, H); // algo 2
        Merge_Super_Nodes(graph, t); // algo 3
        encoding_Summary(graph); //Algo 4
        
        
#ifdef DEBUG
        cout<<Blue<<"Before Dropping Stage"<<endl<<Reset;
        result.printResult_Data();
#endif
        if(config.epsilon > 0.0){
            dropping_Step_SWeG(graph); //Algo 5
        }
#ifdef DEBUG
        cout<<Blue<<"After Dropping Stage"<<endl<<Reset;
        result.printResult_Data();
#endif
        cout<< Green <<"\n============== End T = "<< t <<" ==================\n"<<Reset;
    }
}

//Algorithm-2
void Create_Disjoint_Groups(const Graph& graph, vector<long>&H){
    shuffleHash(H);
    //print_Bijective_Hash(H); //Just for testing purpose
    set<long>::iterator it;
    result.S_Group.clear();//need to clear before creating disjoint group
    for (auto A: result.S) {
        long F_A = inf; // shingle for SuperNode A;
        for (it = A.begin(); it != A.end(); ++it) {
            long F_v = Shingle_of_NodeV(graph, *it, H);
            if (F_v < F_A ) {
                F_A = F_v; // update the shingle of superNode A
            }
        }
        result.S_Group.push_back(make_pair(A, F_A));
    }
    // Sort S_Group to make group
    // Same F(.) super nodes will be adjacent to each other after sorting
    sort(result.S_Group.begin(),result.S_Group.end() ,compareF_Score);
   // print_SuperNodeG(); // For testing
}

//Algorithm-3
void Merge_Super_Nodes(const Graph& graph, int t){
    // iterate over the sorted S_Group & push into Q where F score is same;
    list<pair< set<long>, long>>::iterator it;
    set<set<long>> Q;
    for (int j=0; j < result.S_Group.size(); j++) {
        
        Q.insert(result.S_Group[j].first);
        
        if ((j+1) < result.S_Group.size() && result.S_Group[j].second!= result.S_Group[j+1].second) {
            //this is the last element of this group.
            
            //Call merger functions
            merge_Based_on_Q(Q, graph, t);
            Q.clear(); // Clear Q for next group
            
        }else if((j+1) >= result.S_Group.size()){
            //last element also belongs to the same group;
            //Call merger functions
            merge_Based_on_Q(Q, graph, t);
            Q.clear(); // Clear Q for next group
        }
    }
    //INFO("After Merging SuperNodes S Size: ", result.S.size());
    //result.printSuperNodes();
}

//Algorithm 4
void encoding_Summary(const Graph &graph){
    //const set<long> &SuperA, set< pair<set<long>, set<long> >> &P, set< pair<long, long>> &C_plus, set< pair<long, long>> &C_minus

    set<set<long>>::iterator it_SuperA;
    set<set<long>>::iterator it_SuperB;
    
    //Reset P, C+, C-
    result.P.clear();
    result.C_plus.clear();
    result.C_minus.clear();

    //just the upper triangle to avoid duplication in superEdge pair P
    for (it_SuperA = result.S.begin() ; it_SuperA != result.S.end(); ++it_SuperA) {
        for (it_SuperB= it_SuperA, ++it_SuperB; it_SuperB !=result.S.end();  ++it_SuperB) {
            if(*it_SuperA != *it_SuperB){
                set<long> SuperA = *it_SuperA;
                set<long> SuperB = *it_SuperB;
                set<pair<long, long>>E_AB;
                set<pair<long, long>>Pi_AB;
                calculateEdges_in_AB(graph, SuperA, SuperB, E_AB, Pi_AB);// Calculate E_AB and Pi_AB
                if(!E_AB.empty()){
                    if (E_AB.size() <= ((SuperA.size())*(SuperB.size()))/2) {
                        if (!E_AB.empty()) {
                            union_set(result.C_plus, E_AB); // C+ = C+ U E_AB;
                        }
                    }else{
                        //Fixed: Problem of pair(B,A) and pair(A,B) in P solved by upper triangle for loop, previously both item was stored in P
                        result.P.insert(make_pair(SuperA, SuperB)); // P = P U {{A,B}}
                        
                        set< pair<long, long>> tempDiff; //hold Pi_AB - E_AB;
                        set_difference(Pi_AB.begin(), Pi_AB.end(), E_AB.begin(), E_AB.end(), inserter(tempDiff, tempDiff.begin()) );
                        if (!tempDiff.empty()) {
                            union_set(result.C_minus, tempDiff); // C- = C- U (Pi_AB - E_AB);
                        }
                    }

                }
            }
        }
        //Self Loop Edges.
        set<pair<long, long>>E_AA;
        set<pair<long, long>>Pi_AA;
        set<long> SuperA = *it_SuperA;
        calculateEdges_in_AB(graph, SuperA, SuperA, E_AA, Pi_AA);
        
        if (E_AA.size() <= ((SuperA.size())*(SuperA.size()-1))/4) {
            if (!E_AA.empty()) {
                //E_AA not Empty
                union_set(result.C_plus, E_AA); // C+ = C+ U E_AA;
            }
            
        }else{
            //Fixed: Problem of pair(B,A) and pair(A,B) in P solved by upper triangle for loop, previously both item was stored in P
            result.P.insert(make_pair(SuperA, SuperA)); // P = P U {{A,A}}
            
            set< pair<long, long>> tempDiff; //hold Pi_AA - E_AA;
            set_difference(Pi_AA.begin(), Pi_AA.end(), E_AA.begin(), E_AA.end(), inserter(tempDiff, tempDiff.begin()) );
            if(!tempDiff.empty()){
                //tempDiff not Empty
                union_set(result.C_minus, tempDiff); // C- = C- U (Pi_AA - E_AA);
            }
        }

        

    }

}
//Algo 5
void dropping_Step_SWeG(Graph &graph){
    //changing limit of all node set to zero.
    vector<double>C_V (graph.getN(), 0.0);
    updateChangingLimit(graph, graph.getN(), config.epsilon, C_V);
    
    set< pair<long, long>>::iterator it;
    bool erase_successful = false;
    //--------------------- Dropp from C+ ---------------------
    for (it = result.C_plus.begin(); it != result.C_plus.end();) {
        long u = (*it).first;
        long v = (*it).second;
        erase_successful = false;
        if(C_V[u] >=1 && C_V[v] >=1){
            C_V[u] -= 1;
            C_V[v] -= 1;
            if(!result.C_plus.empty()){
                it= result.C_plus.erase(it); // C++ 11 fix
                erase_successful = true;
            }
        }
        if (!erase_successful) {
            ++it;
        }
    }
    //--------------------- Dropp from C-  ---------------------
    for (it = result.C_minus.begin(); it != result.C_minus.end();) {
        long u = (*it).first;
        long v = (*it).second;
        erase_successful = false;
        if(C_V[u] >=1 && C_V[v] >=1){
            C_V[u] -= 1;
            C_V[v] -= 1;            
            if(!result.C_minus.empty()){
                it = result.C_minus.erase(it); // C++ 11
                erase_successful = true;
            }
        }
        if (!erase_successful) {
            ++it;
        }
    }
    
    //---------------------Drop SuperEdges---------------------
    //Counting Sort SuperEdge based on the |Size of A times Size of B|
    vector< pair< set<long>, set<long>>>sortedSuperEdge_P;
    sortSuperEdges(result.P, sortedSuperEdge_P); //using counting sort, complexity O(n)
    
    // increasing order of |A|.|B| in P
    for (int j=0; j<sortedSuperEdge_P.size(); j++) {
        set<long>A = sortedSuperEdge_P[j].first;
        set<long>B = sortedSuperEdge_P[j].second;
        if (A!=B) {
            if(superNode_Limit_Checking(graph, A, B.size()) && superNode_Limit_Checking(graph, B, A.size())){
                if(!result.P.empty()){ //If P isn't empty, remove pair<A,B> from Super Edges P
                    result.P.erase(make_pair(A, B));
                }
                drop_Changing_Limit_SuperNode(C_V, A, B.size());
                drop_Changing_Limit_SuperNode(C_V, B, A.size());
            }
        }
    }
}


#pragma mark - Functions
void drop_Changing_Limit_SuperNode(vector<double>&C_V, set<long> &SuperA, long limit){
    set< long>::iterator it;
    for (it = SuperA.begin(); it != SuperA.end(); ++it) {
        C_V[(*it)] -= limit;
    }
}

bool superNode_Limit_Checking(const Graph &graph, set<long> &SuperA, long limit){
    bool flag = true;
    
    set< long>::iterator it;
    for (it = SuperA.begin(); it != SuperA.end(); ++it) {
        if (graph.g[(*it)].size()<limit) {
            flag = false;
            break;
        }
    }
    return flag;
}

void sortSuperEdges(const set< pair< set<long>, set<long> > > &P, vector< pair< set<long>, set<long>>> &res){
    
    vector<long>Size_AB;
    res.clear();
    
    //take the superEdge Pair into vector res;
    // take corresponding |A|.|B| into Size_AB// will sort this vector and track corresponding index of res.
    set< pair< set<long>, set<long> > >:: iterator it;
    long maxSize = 0;
    for (it=P.begin(); it!=P.end(); ++it) {
        long temp = (*it).first.size()*(*it).second.size();
        Size_AB.push_back(temp); //|A|*|B|
        if(temp>maxSize){
            maxSize = temp;
        }
        res.push_back((*it));
    }
//    cout<<"Before Sorting"<<endl;
//    testCountSortByPrint(Size_AB);
    
    countSort_SuperEdges(maxSize, Size_AB, res);
    
//    cout<<"After Sorting"<<endl;
//    testCountSortByPrint(Size_AB);
    
}
void testCountSortByPrint(vector<long> &Size_AB){
    for(int i = 0; i < Size_AB.size(); i++) {
        cout<<" "<<Size_AB[i];
    }
    cout<<endl;
}

void countSort_SuperEdges(long max, vector<long> &Size_AB, vector< pair< set<long>, set<long>>> &res){
    
    //g = vector<vector<int>>(n, vector<int>());
    vector<long> number_index(max+2); //vector start from zero but we need upto max storage, i.e, max+1
    
    //High Storage requirement
    vector<vector< pair< set<long>, set<long>>>> twoD_Index_of_SuperEdgePair =  vector<vector< pair< set<long>, set<long>>>> (max+2, vector< pair< set<long>, set<long>>>()); // will index the SuperEdge Pair according to our number_index
    
    
    for(int i = 0; i < Size_AB.size(); i++) {
        number_index[Size_AB[i]]++;
        twoD_Index_of_SuperEdgePair[Size_AB[i]].push_back(res[i]);
    }
    
    Size_AB.clear();
    res.clear();
    for(int i = 0; i < number_index.size(); i++) {
        for(int j = 0; j < number_index[i]; j++) {
            Size_AB.push_back(i);
        }
        for(int j = 0; j < twoD_Index_of_SuperEdgePair[i].size(); j++) {
            res.push_back(twoD_Index_of_SuperEdgePair[i][j]);
        }
    }
}
void updateChangingLimit(const Graph &graph, long numberofNodes_inG, double epsilion, vector<double>&C_V){
    for (long node=0 ; node<numberofNodes_inG; node++) {
        C_V[node] = epsilion * graph.g[node].size();
    }
}


void encoding_for_Saving_only(const Graph &graph, const set<set<long>> &SS, const set<long> &SuperA, set< pair<set<long>, set<long> >> &P, set< pair<long, long>> &C_plus, set< pair<long, long>> &C_minus){
    set<set<long>>::iterator it_SuperB;

    for (it_SuperB = SS.begin(); it_SuperB != SS.end(); ++it_SuperB) {
        if(SuperA != *it_SuperB){
            set<long> SuperB = *it_SuperB;
            set<pair<long, long>>E_AB;
            set<pair<long, long>>Pi_AB;
            calculateEdges_in_AB(graph, SuperA, SuperB, E_AB, Pi_AB);
            if(!E_AB.empty()){
                if (E_AB.size() <= ((SuperA.size())*(SuperB.size()))/2) {
                    if (!E_AB.empty()) {
                        union_set(C_plus, E_AB); // C+ = C+ U E_AB;
                    }
                    
                }else{
                    P.insert(make_pair(SuperA, SuperB));// P = P U {{A,B}}
                    
                    set< pair<long, long>> tempDiff; //hold Pi_AB - E_AB;
                    set_difference(Pi_AB.begin(), Pi_AB.end(), E_AB.begin(), E_AB.end(), inserter(tempDiff, tempDiff.begin()) );
                    if (!tempDiff.empty()) {
                        union_set(C_minus, tempDiff); // C- = C- U (Pi_AB - E_AB);
                    }
                }
            }
        }
    }
    // for Saving calculation We don't need self Loop edges to consider
    
}


//E_AB is the edges between supernode A & B. each edges exist in original graph.
//Pi_AB is the all pair of nodes from A,B.
void calculateEdges_in_AB(const Graph &graph, const set<long>&A, const set<long>&B, set<pair<long, long>>&E_AB, set<pair<long, long>>&Pi_AB ){
    // E_AB, & Pi_AB is the result we need from this function.
    
    // Create Hash Table of Actual edges between SuperNode A and B.
    unordered_map<Upair, bool, pair_hash> Hash_MP;
    //unordered_map<pair<long, long>, bool> :: iterator it_map;
    
    HashMap_Edges_of_SuperA(graph, A, Hash_MP);
    HashMap_Edges_of_SuperA(graph, B, Hash_MP);
    // At this step MP has all the edges of A & B.
    
    set<long>::iterator it_A;
    set<long>::iterator it_B;
    for (it_A = A.begin(); it_A != A.end(); ++it_A) {
        for (it_B = B.begin(); it_B != B.end(); ++it_B) {
            //if both are not same node.Condition specially for Pi_AA, u!=v
            if(*it_A != *it_B){
                Pi_AB.insert(make_pair(MIN(*it_A, *it_B),MAX(*it_A, *it_B))); //insert All pair, use MIN,MAX to avoid symmetric duplicate entry
                
                if(Hash_MP.find(make_pair(MIN(*it_A, *it_B), MAX(*it_A, *it_B))) != Hash_MP.end()){
                    // Edge exist in original Graph
                    E_AB.insert(make_pair(MIN(*it_A, *it_B),MAX(*it_A, *it_B)));
                }
            }
        }
    }
}

void HashMap_Edges_of_SuperA(const Graph &graph,const set<long>&A, unordered_map<Upair, bool, pair_hash> &MP){
    set<long>::iterator it;
    //Insert each edge of SuperNode A into the HashTable, insertion cost O(1)
    for (it=A.begin(); it !=A.end(); ++it) {
        for (int j=0; j<graph.g[*it].size(); j++) {
            MP[make_pair(MIN(*it, graph.g[*it][j]), MAX(*it, graph.g[*it][j]))] = true;
        }
    }
}

void merge_Based_on_Q(set<set<long>>&q,const Graph& graph, int t){
    while(q.size()>1){
        set<long> A = pickRandom_Item_From_Q(q);
        q.erase(A);
        set<set<long>>::iterator it;
        set<long> B;
        double maxJValue = mn_inf;
        //get the Super Node B :: maxSimilar with A
        for (it = q.begin(); it != q.end(); ++it) {
            set<long>temp = (*it);
            double JSimilarity = SuperJaccard(A, temp, graph);
            if( JSimilarity > maxJValue){
                B.clear();
                B = temp;
                maxJValue = JSimilarity; //update maxJValue;
            }
        }
        //Calculate Saving(A,B,S);
        double saving = Saving(graph, A, B, result.S);
        double thetaT = mergingThreshold(t);
       // cout<< "Saving : " <<saving <<" Thera_T: "<<thetaT<<endl;
        
         if (saving >= thetaT) {
            result.S.erase(A);
            result.S.erase(B);
            set<long>A_Union_B;
            set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(A_Union_B, A_Union_B.begin()));
            result.S.insert(A_Union_B);
             q.erase(A);
             q.erase(B);
             q.insert(A_Union_B);
        }
        
    }
}
double Saving(const Graph& graph, const set<long>&A,const set<long>&B, set<set<long>>SS){
    double saving = 0.0;
    double previousCost = result.P.size() + result.C_plus.size() + result.C_minus.size();
    double costA = cost(graph, A, SS) - previousCost;
    double costB = cost(graph, B, SS) - previousCost;
    
    SS.erase(A);
    SS.erase(B);
    set<long>A_Union_B;
    set_union(A.begin(), A.end(), B.begin(), B.end(), inserter(A_Union_B, A_Union_B.begin()));
    SS.insert(A_Union_B);
    
    double cost_AUB = cost(graph, A_Union_B , SS) - previousCost;
    
    double Denom = costA+costB;
    if(Denom > 0.0){
        saving = 1 - (cost_AUB/Denom);
    }
    
    return saving;
}

//return the Cost of A = |P*| + |C+| + |C-|
long cost(const Graph& graph, const set<long>&A , set<set<long>>& SS){
    
    long res = 0;
    set< pair<set<long>, set<long> >> P_temp;
    set< pair<long, long>> C_plus_temp;
    set< pair<long, long>> C_minus_temp;
    
    encoding_for_Saving_only(graph, SS, A, P_temp, C_plus_temp, C_minus_temp);
    res = P_temp.size() + C_plus_temp.size() + C_minus_temp.size();
    return res;
}



double SuperJaccard(set<long>&A, set<long>&B, const Graph& graph){
    double res=0.0;
    
    set<long>Neighbors = get_Neighbor_Set_of_SuperNode(A,graph);
    union_set(Neighbors, get_Neighbor_Set_of_SuperNode(B,graph)); // Union of Neighbor_A and Neighbor_B
    //print_set(Neighbors);
    
    
    set<long>::iterator it;
    set<long>::iterator it_lookup;
    int Jnumerator = 0;
    int Jdenominator = 0;
    for (it=Neighbors.begin(); it!=Neighbors.end(); ++it) {
        //INFO("N_v: ", *it);
        int w_A = 0;
        int w_B = 0;
        //For each neighbor node: look at each edge and see that connecting node belongs to A or B.
        for (int j=0; j<graph.g[*it].size(); j++) {
            //INFO("AB_v: ", graph.g[*it][j]);
            //If node = graph.g[*it][j] belongs to set A, it_lookup will not be A.end.
            it_lookup = A.find(graph.g[*it][j]);
            if (it_lookup != A.end()) {
                w_A++;
            }
            //Same condition
            it_lookup = B.find(graph.g[*it][j]);
            if (it_lookup != B.end()) {
                w_B++;
            }
        }
        Jnumerator += MIN(w_A, w_B);
        Jdenominator += MAX(w_A, w_B);
    }
    
    if(Jdenominator !=0){
        res = double(Jnumerator)/double(Jdenominator);
    }
    
    return res;
}

set<long> get_Neighbor_Set_of_SuperNode(set<long>&A, const Graph& graph){
    set<long> res;
    set<long>::iterator it;
    for (it = A.begin(); it != A.end(); ++it) {
        //For each node in SuperA get the neighbor
        for (int j=0; j<graph.g[*it].size(); j++) {
            res.insert(graph.g[*it][j]);
        }
    }
    return res;
}

set<long> pickRandom_Item_From_Q(set<set<long>>&q){
    int item = rand() % q.size(); // Random Number within the size
    int i = 0;
    set<set<long>>::iterator it = q.begin();
    for(it = q.begin(); it!=q.end(); ++it){
        if (i == item) {
            break; // pick this random item  & return *it
        }
        i++;
    }
    return *it;
}


bool compareF_Score(pair< set<long>, long> i1, pair< set<long>, long> i2)
{
    return (i1.second < i2.second);
}


// return min{h(u): u in N_v or u==v}
// Shingle of Node V
long Shingle_of_NodeV(const Graph& graph, long v, const vector<long>&H){
    long fv = H[v]; // cover u==v checking case
    for (long u = 0; u< graph.g[v].size(); u++) {
        if(H[graph.g[v][u]] < fv){
            fv = H[graph.g[v][u]]; // get the min
        }
    }
    return fv;
}

//This function is for testing purpose
void print_Bijective_Hash(const vector<long>&H){
    for (long i=0; i<H.size(); i++) {
        cout<<"Node ID:["<< i <<"] = "<< H[i]<<" "<<endl;
    }
}

// Print Super Nodes S
void print_SuperNodes_S(){
    set<long>::iterator it;
    cout<<"\n::printing SuperNodes S::\n";
    int i=1;
    for(auto A: result.S){
        cout<<"\nSuper Node ["<<i++<<"] : ";
        for (it = A.begin(); it!=A.end(); ++it) {
            cout<<*it<<" ";
        }
    }
    printSplitLine();
}

// Print Super Node Group after Sorting
void print_SuperNodeG(){
    set<long>::iterator it;
    cout<<"\n::printing Disjoint Group of SuperNodes::\n";
    int i=1;
    for(auto A: result.S_Group){
        cout<<"\nSuper Node ["<<i++<<"] : ";
        for (it = A.first.begin(); it!=A.first.end(); ++it) {
            cout<<*it<<" ";
        }
        cout<<"\nF(.) Score: "<<A.second<<endl;
    }
    printSplitLine();
}



// initialize vector::  nodes V->{1,,,|V|}
void initilize_Hash_using_Vector(vector<long>&hash, long n){
    for (long i=0; i<n; i++) {
        hash.push_back(i+1);
    }
}
// random generator function:
long myrandom (long i) {
    return std::rand()%i;
    
}

//generate randomize bijective hash
void shuffleHash(vector<long>&hash){
    //Rearranges the elements in the range [first,last) randomly.
    random_shuffle ( hash.begin(), hash.end(), myrandom);
    //Complexity: Linear in the distance between first and last minus one:
}

void initializeSupernodes(long numberOfNodes){
    //Set Each Node as a SuperNode;
    //NodeID start at 0.
    set<long>temp;
    for (long i=0; i<numberOfNodes; i++) {
        temp.clear();
        temp.insert(i);
        result.S.insert(temp);
    }
}

double mergingThreshold(int t){
    double res = 0.0;
    if (t < config.max_iteration_T) {
        res =  1.0/double(1.0+t);
    }
    return res;
}

