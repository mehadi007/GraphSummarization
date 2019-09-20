//
//  utility.hpp
//  Graph_Summarization
//
//  Created by Mehedi on 8/19/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#ifndef utility_hpp
#define utility_hpp

#include <iostream>
#include <set>
#include <list>
#include <sstream>
#include <cmath>
#include <fstream>
#include <string>
#include <cstdio>
#include <functional>
#include <algorithm>
#include <climits>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <deque>
#include <queue>
//#include <chrono>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>

using namespace std;

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef TIMES_PER_SEC
#define TIMES_PER_SEC (1.0e9)
#endif


//MACRO define some variable
#define Pi acos(-1.0)
#define ull unsigned long long
#define inf   INT_MAX
#define mn_inf  INT_MIN
#define EPS 1e-9

//MACRO define some function
#define abs(x) (x<0?(-x):x)
#define MAX(a,b)    (a)>(b)?(a):(b)
#define MIN(a,b)    (a)<(b)?(a):(b)
#define RESET(a,s)    memset(a, s, sizeof(a))


#define SIZE(t) (int)(t.size())
#define ALL(t) (t).begin(), (t).end()
#define FOR(i, n) for (int(i) = 0; (i) < ((int)(n)); (i)++)
//--------------------------//


#pragma mark -  Globally accessible functions
bool file_exists_at_path(string);

void print_set(const set<long> &);

//Union
void union_set(set<long> &, const set<long> &);  //A = A U B; Param: &A, const &B,//Union of SuperNodes
void union_set(set< pair<long, long>> &A, const set< pair<long, long>> &B); //Union of C
void union_set(set< pair<set<long>, set<long> >> &A, const set< pair<set<long>, set<long> >> &B); //Union of P

//InterSection
set<long> intersection_set(const set<long> &, const set<long> &); //Return Res = A Intersection B

//Double to String Conversion
static string to_str(double t)
{
    stringstream ss;
    ss << t;
    return ss.str();
}

static string get_current_time_instring()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    std::string str(buffer);
    
    return str;
}


//shell coloring
const string Green = "\033[0;32m";
const string Reset = "\033[0m";
const string Red = "\033[0;31m";
const string Blue= "\033[0;34m";
//Black="\[\033[0;30m\]"        # Black
//Red="\[\033[0;31m\]"          # Red
//Green="\[\033[0;32m\]"        # Green
//Yellow="\[\033[0;33m\]"       # Yellow

static void program_start(int argc, char **argv)
{
    
    cout << Green << "--------------start------------" << get_current_time_instring() << Reset << endl;
    string combine = "";
    for (int i = 1; i < argc; i++)
    {
        combine += argv[i];
        combine += " ";
    }
    cout << Green << "args:" << combine << Reset << endl;
}

static void program_stop()
{
    cout << Red << "--------------stop------------" << get_current_time_instring() << Reset << endl;
    cout << endl;
    cout << endl;
    cout << endl;
}
inline void printSplitLine(){
    cout<<"\n================================\n";
}

static vector<string> combine_args(int argc, char **argv) {
    vector<string> args;
    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return args;
}

//--------------------------//
#pragma mark - template classes
template <typename T>
class UPair
{
private:
    pair<T, T> p;

public:
    // making a pair where min value if the first element
    UPair(T a, T b) : p(min(a, b), max(a, b))
    {
    }
    UPair(pair<T, T> pair) : p(min(pair.first, pair.second), max(pair.first, pair.second))
    {
    }
    friend bool operator==(UPair const &a, UPair const &b)
    {
        return a.p == b.p;
    }
    operator pair<T, T>() const
    {
        return p;
    }
};

namespace std
{
template <typename T>
struct hash<UPair<T>>
{
    size_t operator()(UPair<T> const &up) const
    {
        return hash<size_t>()(
                   hash<T>()(pair<T, T>(up).first)) ^ hash<T>()(pair<T, T>(up).second);
        // the double hash is there to avoid the likely scenario of having the same value in .first and .second, resulinting in always 0
        // that would be a problem for the unordered_map's performance
    }
};
} // namespace std

//Use-Case of UPair
//unordered_map<UPair<int>,float> um;
//um[UPair<int>(3,7)] = 3.14;
//um[UPair<int>(8,7)] = 2.71;
//return 10*um[::std::make_pair(7,3)]; // correctly returns 31
//https://stackoverflow.com/questions/29180271/c-storing-a-value-in-an-unordered-pair



//-------Templates for Boost Object Print -----------------//

static inline string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

static inline string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

static inline string &trim(string &s) { return ltrim(rtrim(s)); }

string __n_variable(string t, int n);

#define __expand_nv(x) __n_variable(t, x)<< t##x << " "

template<class T0>
void ___debug(string t, T0 t0, ostream &os) {
    //if(!__clang__){
        os << __expand_nv(0);             // Issue :: Error - Fixed after adding boost properly
    //}
}

template<class T0, class T1>
void ___debug(string t, T0 t0, T1 t1, ostream &os) {
    os << __expand_nv(0) << __expand_nv(1);
}

template<class T0, class T1, class T2>
void ___debug(string t, T0 t0, T1 t1, T2 t2, ostream &os) {
    os << __expand_nv(0) << __expand_nv(1) << __expand_nv(2);
}

template<class T0, class T1, class T2, class T3>
void ___debug(string t, T0 t0, T1 t1, T2 t2, T3 t3, ostream &os) {
    os << __expand_nv(0) << __expand_nv(1) << __expand_nv(2) << __expand_nv(3);
}

template<class T0, class T1, class T2, class T3, class T4>
void ___debug(string t, T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, ostream &os) {
    os << __expand_nv(0) << __expand_nv(1) << __expand_nv(2) << __expand_nv(3) << __expand_nv(4);
}

template<class T0>
void ___debug(string t, deque<T0> t0, ostream &os) {
    os << __n_variable(t, 0);
    FOR(i, SIZE(t0))os << t0[i] << " ";
}

template<class T0>
void ___debug(string t, vector<T0> t0, ostream &os) {
    os << __n_variable(t, 0);
    FOR(i, SIZE(t0))os << t0[i] << " ";
}

template<class T0, class T1>
void ___debug(string t, vector<pair<T0, T1> > t0, ostream &os) {
    os << __n_variable(t, 0);
    FOR(i, SIZE(t0))os << t0[i].F << "," << t0[i].S << " ";
}

#define RUN_TIME(...) { int64 t=rdtsc();  __VA_ARGS__; t=rdtsc()-t; cout<<  #__VA_ARGS__ << " : " << t/TIMES_PER_SEC <<"s"<<endl;  }

#ifdef HEAD_TRACE
#define TRACE(...) {{ ___debug( #__VA_ARGS__,  __VA_ARGS__,cerr); cerr<<endl;  } }
#define IF_TRACE(args) args
#define TRACE_LINE(...) { ___debug( #__VA_ARGS__,  __VA_ARGS__,cerr); cerr<<"                    \033[100D";  }
#define TRACE_SKIP(a, ...) { static int c=-1; c++; if(c%a==0)TRACE( __VA_ARGS__); }
#define TRACE_LINE_SKIP(a, ...) { static int c=-1; c++; if(c%a==0) TRACE_LINE(__VA_ARGS__);  }
#define TRACE_LINE_END(...) {cerr<<endl; }
ofstream __HEAD_H__LOG("log.txt");
#define TRACE_LOG(...) { __HEAD_H__LOG.close(); ofstream cerr("log.txt", ofstream::out|ofstream::app); ___debug( #__VA_ARGS__,  __VA_ARGS__, cerr); cerr<<endl;  }
#else
#define TRACE(...) ;
#define IF_TRACE(args) ;
#define TRACE_LINE(...) ;
#define TRACE_SKIP(a, ...) ;
#define TRACE_LINE_SKIP(a, ...) ;
#define TRACE_LINE_END(...) ;
#define TRACE_LOG(...) ;
#endif //HEAD_TRACE


//void setInfoFile(string s) { __HEAD_H_FOUT.open(s.c_str()); }

#define ASSERT(v) {if (!(v)) {cerr<<"ASSERT FAIL @ "<<__FILE__<<":"<<__LINE__<<endl; exit(1);}}
#define INFO(...) do {\
___debug( #__VA_ARGS__,  __VA_ARGS__,cout); cout<<endl; \
} while(0)
#define ASSERTT(v, ...) {if (!(v)) {cerr<<"ASSERT FAIL @ "<<__FILE__<<":"<<__LINE__<<endl; INFO(__VA_ARGS__); exit(1);}}





#endif /* utility_hpp */
