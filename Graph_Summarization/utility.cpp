//
//  utility.cpp
//  Graph_Summarization
//
//  Created by Mehedi on 8/19/19.
//  Copyright © 2019 Hussain Md Mehedul Islam. All rights reserved.
//

#include "utility.hpp"


// Testing a filepath exist or not
bool file_exists_at_path( string fullPath) {
    ifstream f(fullPath.c_str());
    if (f.good()) {
        f.close();
        return true;
    }
    else {
        f.close();
        cout << fullPath << " not find " << endl;
        exit(1);
        return false;
    }
}

string __n_variable(string t, int n) {
    t = t + ',';
    int i = 0;
    if (n) for (; i < SIZE(t) && n; i++) if (t[i] == ',') n--;
    n = i;
    for (; t[i] != ','; i++);
    t = t.substr((unsigned long) n, (unsigned long) (i - n));
    trim(t);
    if (t[0] == '"') return "";
    return t + "=";
}


//Printing a set
void print_set(const set<long> &A){
    cout<<"Printing Set"<<endl;
    set<long>::iterator it;
    for (it=A.begin(); it!=A.end(); ++it)
        cout << ' ' << *it;
    cout << '\n';
}


//Union of Two Set
//A = A U B;
// Param: &A, const &B
//Union of superNodes
void union_set(set<long> &A, const set<long> &B){
    set<long>::iterator it;
    //Just adding B into A
    for (it=B.begin(); it!=B.end(); ++it)
        A.insert(*it);
    
}

//function overloading for set<pair<long,long>>
//A = A U B;
//Union of C
void union_set(set< pair<long, long>> &A, const set< pair<long, long>> &B){
    set< pair<long, long>>::iterator it;
    //Just adding B into A
    for (it=B.begin(); it!=B.end(); ++it)
        A.insert(*it);
}

////function overloading for set< pair<set<long>, set<long> >>
//A = A U B;
//Union of P
void union_set(set< pair<set<long>, set<long> >> &A, const set< pair<set<long>, set<long> >> &B){
    set< pair<set<long>, set<long> >>::iterator it;
    //Just adding B into A
    for (it=B.begin(); it!=B.end(); ++it)
        A.insert(*it);
}


//Intersection of Two Set
//Intersection of Two Set
//Return Res = A Intersection B
set<long> intersection_set(const set<long> &A, const set<long> &B){
    set<long>intersectionSet;
    set<long>::iterator it_A = A.begin();
    set<long>::iterator it_B = B.begin();
    
    //set container store sorted order
    while(1){
        if( it_A == A.end() || it_B == B.end())
            break;
        //Common item
        if(*it_A == *it_B){
            intersectionSet.insert(*it_A);
            ++it_A;
            ++it_B;
        }else if(*it_A < *it_B){
            ++it_A;
        }
        else if(*it_A > *it_B){
            ++it_B;
        }
    }
    // No need to add remaining items from both set:
    // They are not common
    return intersectionSet;
}
