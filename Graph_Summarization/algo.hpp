//
//  algo.hpp
//  Graph_Summarization
//
//  Created by Mehedi on 8/29/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#ifndef algo_hpp
#define algo_hpp

#include <stdio.h>
#include "utility.hpp"
#include "config.hpp"

using namespace std;

//Algorithms
void OverviewSummarization(Graph &graph); //Algo 1, return object "result" is an extern
void Create_Disjoint_Groups(const Graph& graph, vector<long>&H); //Algo 2
void Merge_Super_Nodes(const Graph& graph, int t); // Algo 3
void encoding_Summary(const Graph &graph); //Algo 4
void dropping_Step_SWeG(Graph &graph); //Algo 5

// Functions
void print_SuperNodes_S(); // Printing SuperNodes S



#endif /* algo_hpp */
