#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <map>

using namespace std;

vector<regex>* compilarRegexList(list<string>& regexList);
void addCrossRefLines(vector<int>& indexes, map<string,vector<int> >& groupMap);
void addToGroup(int index, string& line, map<string,vector<int> >& groupMap, regex& regexGroup, int groupIndex);
