#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <map>
#include "lana-2.h"

using namespace std;

extern vector<regex>* compilarRegexList(list<string>& regexList) {
    vector<regex> *regexListComp = new vector<regex>();

    for (string& r : regexList) {
        regexListComp->push_back( * new regex( r ) );
    }

    return regexListComp;
}

