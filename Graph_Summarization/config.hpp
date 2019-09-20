//
//  config.hpp
//  Graph_Summarization
//
//  Created by Mehedi on 8/28/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#ifndef config_hpp
#define config_hpp

#include <boost/property_tree/ptree.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <unordered_map>
#include <list>

#include "utility.hpp"


using namespace boost;
using namespace std;
using namespace boost::property_tree;

//-----------Macros-------------//
#pragma mark - Macros
#ifdef WIN32
#define DirectorySeparator "\\"
#else
#define DirectorySeparator "/"
#endif

#ifdef WIN32
const string parent_folder = "../../";
#else
const string parent_folder = string("./") + DirectorySeparator;
#endif
//-----------------------//


//static void initGraphparams(Config &config, const Graph &graph);


#pragma mark - Classes
//Forward Declaration
class Graph;
class Config;
class Result;


//Extern Configuration Class object to use multiple places
extern Config config;
extern Result result;

class Config {
public:
    
    double epsilon = 0; //Error Bound-Dropping Stage
    unsigned int max_iteration_T = 100;
    
    string graphData; // Graph Data Name. Example DBLP / IMDB
    string graphDataLocation;
    
    string prefix = "/Users/Mehedi/BitBucket/GraphDatabase-Research/Snap-4.1/Graph_Summarization/Graph_Summarization/data/";
    
    string executed_result_directory = parent_folder;
    string action = ""; // Summarization/ retrieve Summarized graph from Disk/query/ generate index, etc.. :: For Future Extention
    
    
    string getGraphDataFolder() {
        return prefix + graphData + DirectorySeparator;
    }
    void printConfig(){
        printSplitLine();
        cout<<"Current Configuration: "
        <<"\nGraph Data: "<< graphData
        <<"\nAction: " << action
        << "\nEpsilon: "<<epsilon
        <<"\nresult_Dir: "<<executed_result_directory
        <<"\nMax_Iteration: "<<max_iteration_T
        <<endl;
        printSplitLine();
    }
    ptree getData() {
        ptree config_data;
        config_data.put("graph_alias", graphData);
        config_data.put("action", action);
        config_data.put("epsilon", epsilon);
        config_data.put("result-dir", executed_result_directory);
        config_data.put("Max_Iteration", max_iteration_T);
        return config_data;
    }
};

class Graph {
private:
    long n;  // Number of Nodes  (0 to n-1)
    long long m;   //Number of Edges
    void getNM();
    void init_graph();
public:
    long getN();
    long long getM();
    void setN(long);
    void setM(long long);
    vector<vector<int>> g; //Adjacency List representation
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
};



class Result{
private:
 
public:
    set<set<long>>S; //superNodes
    set< pair< set<long>, set<long> > >P; //SuperEdges
    set< pair<long, long>> C_plus; //correction+
    set< pair<long, long>> C_minus; //correction+
    vector< pair< set<long>, long> > S_Group;//pair<superNode, F>
    
    void printSuperNode_A(const set<long>&); //Print Super Node A
    void printSuperNodes(); //All Super Nodes S
    void printSuperEdges();
    void printCorrections();
    void printResult_Data();
    
    ~Result(){
        S.clear();
        P.clear();
        S_Group.clear();
        C_plus.clear();
        C_minus.clear();
    }
    
    ptree getData() {
        ptree data;
        data.put("Size of SuperNodes: ", S.size());
        data.put("Size of SuperEdges", P.size());
        data.put("Size of C+", C_plus.size());
        data.put("Size of C-", C_minus.size());
        
        //TODO://data.put("Timer Information will update Later");
        
        return data;
    }
};



#pragma mark - NameSpace
namespace SaveContainer {
    static ptree combineOutput;
    static void init() {
        combineOutput.put("start_time", get_current_time_instring());
    }
    
    static string get_time_path() {
        if(!boost::algorithm::ends_with(config.executed_result_directory, DirectorySeparator))
            config.executed_result_directory += DirectorySeparator;
        config.executed_result_directory += "Output/";
        if(!boost::filesystem::exists(config.executed_result_directory)){
            boost::filesystem::path dir(config.executed_result_directory);
            boost::filesystem::create_directories(dir);
        }
        std::string filename = config.graphData+"."+config.action + ".t=" + std::to_string(config.max_iteration_T) + "epsilion-" + std::to_string(config.epsilon) ;

        return config.executed_result_directory + filename;
        //return the fileName to save the result.
    }
    static void saveResultAsjson(Config &config, Result &result, vector<string> args) {       
        ofstream fout(get_time_path() + ".json");
        string command_line = "";
        for (int i = 1; i < args.size(); i++) {
            command_line += " " + args[i];
        }
        combineOutput.put("end_time", get_current_time_instring());
        combineOutput.put("command_line", command_line);
        combineOutput.put_child("Config", config.getData());
        combineOutput.put_child("Result", result.getData());

        //TODO :: Have to add the timer
//        ptree timer;
//        for (int i = 0; i < (int) Timer::timeUsed.size(); i++) {
//            if (Timer::timeUsed[i] > 0) {
//                stringstream ss;
//                ss << i;
//                timer.put(ss.str(), Timer::timeUsed[i] / TIMES_PER_SEC);
//            }
//        }
//        combineOutput.put_child("Timer", timer);
        
        write_json(fout, combineOutput, true);
    }
};



#endif /* config_hpp */
