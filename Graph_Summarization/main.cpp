//
//  main.cpp
//  Graph_Summarization
//
//  Created by Mehedi on 8/19/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <chrono>
#include <fstream>
#include <map>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

//// Boost lib can help to implement multiple thread/ FileSystem / Serialization
#include <boost/progress.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



//--- Add Own headers below ---/

#include <iostream>
#include "utility.hpp"
#include "config.hpp"
#include "algo.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    
    program_start(argc, argv);
    SaveContainer::init();
    config.graphData = "test"; // Default
    for (int i = 0; i < argc; i++) {
        string help = ""
        "Actions: \n"
        "summ:  build exact supernodes S, superEdges P and Corrections C\n"
        "summA: Droppoing some edges while maintaining the error bound\n\n"

        "options: \n"
        "  --prefix <prefix>\n"
        "  --epsilon <epsilon>\n"
        "  --dataset <dataset>\n"
        "  --result_dir <directory to place results>\n"
        "  --iteration <Num of Iteration Count T>\n";

        if (string(argv[i]) == "--help") {
            cout << help << endl;
            exit(0);
        }
    }
    
    //Read argv values
    
    config.action = argv[1];
    //cout << "Action: " << config.action << endl;

    int i=0;
    while(i < argc) {
        if (string(argv[i]) == "--prefix") {
            //Data taken from path--- prefix
            config.prefix = argv[i + 1];
            //cout << "Prefix: " << config.prefix << endl;
        }
        if (string(argv[i]) == "--dataset") {
            config.graphData = argv[i + 1];
           // cout << "DataSet: " << config.graphData << endl;
        }
        i++;
    }
    for (int i = 0; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--epsilon") {
            config.epsilon = atof(argv[i + 1]);
        }
        else if(arg == "--result_dir"){
            config.executed_result_directory = string(argv[i + 1]);
        }
        else if(arg == "--iteration"){
            config.max_iteration_T = atoi(argv[i + 1]);
        }
        else if (arg == "--prefix" || arg == "--dataset") {
            //
        }
        else if (arg.substr(0, 2) == "--") {
            cout << "Unknown : " << arg << endl;
            exit(1);
        }
    }
#ifdef DEBUG
    config.printConfig();
#endif
    
    
    //Start All the Operations from Here.
    
    srand (time(NULL));
    if (config.action == "summ"){
        config.graphDataLocation = config.getGraphDataFolder();
        Graph graph(config.graphDataLocation);
        
        OverviewSummarization(graph); //Calling algo 1
        
        auto args = combine_args(argc, argv);
        SaveContainer::saveResultAsjson(config, result, args);
                
        program_stop();
    }
    return 0;
}


