#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <map>

#include "lana-2.h"

using namespace std;


class Error : public exception {
private:
    const char* msg;
public:

    Error(const char* msg) {
        this->msg = msg;
    }

    const char* what() const throw() {
        return this->msg;
    }
};

void addCrossRefLines(vector<int>& indexes, map<string,vector<int> >& groupMap);
void addToGroup(int index, string& line, map<string,vector<int> >& groupMap, regex& regexGroup, int groupIndex);

int main(int argc, char** argv) {

    vector<string> searchText;
    list<string> strMatches;
    list<string> strNotMatches;
    list<string> strGroups;
    vector<int> groupsNumber;
    int printLines = 0;
    int completeGaps = 0;

    regex regex_group("^(-g(\\d))$");
    smatch smatch_group;

    for (int index = 1; index < argc; index++) {
        string *name = new string(argv[index]);

        if (name->compare("-pl") == 0) {
            printLines = 1;
        } else if (name->compare("-cg") == 0) {
            completeGaps = 1;
        } else if (name->compare("-t") == 0) {
            searchText.push_back(argv[index+1]);
        } else if (name->compare("-m") == 0) {
            strMatches.push_back(argv[index+1]);
        } else if (name->compare("-M") == 0) {
            strNotMatches.push_back(argv[index+1]);
        } else if (regex_match(*name, smatch_group, regex_group) && smatch_group.size() == 3) {
            strGroups.push_back(argv[index+1]);
            groupsNumber.push_back(stoi(smatch_group[2]));
        }

        delete name;
    }

    vector<regex> *regexMatches = compilarRegexList(strMatches);
    vector<regex> *regexNotMatches = compilarRegexList(strNotMatches);
    vector<regex> *regexGroups = compilarRegexList(strGroups);

    try {
        if (regexGroups->size() <= 0) {
            throw new Error("é necessáiro informar 1 ou mais grupos!");
        }

        vector<string> lines;
        vector<map<string,vector<int> > > listOfIndexesByGroupId;

        for (int index = 0; index < regexGroups->size(); index++) {
            listOfIndexesByGroupId.push_back( * new map<string,vector<int> >() );
        }

        for (string line; getline(cin, line); ) {
            lines.push_back(line);
        }
        
        for (int index = 0; index < lines.size(); index++) {
            for (int indexGroup = 0; indexGroup < regexGroups->size(); indexGroup++) {
                addToGroup(index, lines[index], listOfIndexesByGroupId[indexGroup], regexGroups->at(indexGroup), groupsNumber[indexGroup]);
            }
        }

        for (auto iter = listOfIndexesByGroupId[0].begin(); iter != listOfIndexesByGroupId[0].end(); iter++) {
            const string& groupId = (*iter).first;
            vector<int>& indexes = (*iter).second;

            for (int index = 1; index < listOfIndexesByGroupId.size(); index++) {
                addCrossRefLines(indexes, listOfIndexesByGroupId[index]);
            }

            list<int> sorter;
            for (int index : indexes) {
                sorter.push_back(index);
            }
            sorter.sort();
            indexes.clear();
            for (int index : sorter) {
                indexes.push_back(index);
            }
        }

        vector<string> groupIds;
        for (auto iter : listOfIndexesByGroupId[0]) {
            const string& groupId = iter.first;
            vector<int>& indexes = iter.second;

            bool foundSearch = false;
            if (searchText.size() > 0 || regexMatches->size() > 0) {
                for (int index : indexes) {
                    string& line = lines[index];

                    bool foundText = false;
                    if (searchText.size() > 0) {
                        for (string& m : searchText) {
                            if (line.find(m) != string::npos) {
                                foundSearch = true;
                                foundText = true;
                                break;
                            }
                        }
                    }

                    if (!foundText) {
                        for (regex& r : *regexMatches) {
                            if (regex_search(line, r)) {
                                foundSearch = true;
                                break;
                            }
                        }
                    }

                    if (foundSearch) {
                        break;
                    }
                }
            } else {
                foundSearch = true;
            }


            if (foundSearch) {
                bool foundExclusion = false;

                for (int index : indexes) {
                    string& line = lines[index];

                    for (regex& r : *regexNotMatches) {
                        if (regex_search(line, r)) {
                            foundExclusion = true;
                            break;
                        }
                    }

                    if (foundExclusion) {
                        break;
                    }
                }

                if (!foundExclusion) {
                    groupIds.push_back(groupId);
                }
            }
        }


        string divider;
        for (int i = 0; i < 100; i++) {
            divider.push_back('-');
        }
        
        for (string groupId : groupIds) {
            for (int index : listOfIndexesByGroupId[0][groupId]) {
                if (printLines) {
                    cout << index << ": " << lines[index] << endl;
                } else {
                    cout << lines[index] << endl;
                }
            }

            cout << divider << endl;
        }
    } catch (Error& error) {
        cerr << error.what() << endl;
        return 1;
    }

    return 0;
}

void addCrossRefLines(vector<int>& indexes, map<string,vector<int> >& groupMap) {
    for (auto iter = groupMap.begin(); iter != groupMap.end(); iter++) {
        vector<int>& idxs = (*iter).second;
        
        bool found = false;
        for (int m : indexes) {
            for (int x : idxs) {
                if (m == x) {
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }

        for (int i : idxs) {
            found = false;
            for (int index : indexes) {
                if (i == index) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                indexes.push_back(i);
            }
        }
    }
}

void addToGroup(int index, string& line, map<string,vector<int> >& groupMap, regex& regexGroup, int groupIndex) {
    
    smatch smatch;
    if (regex_search(line, smatch, regexGroup) && smatch.size() > groupIndex) {
        const string& groupId = smatch[groupIndex].str();

        auto iter = groupMap.find(groupId);
        if (iter == groupMap.end()) {
            vector<int>& indexes = * new vector<int>();
            groupMap[groupId] = indexes;
            indexes.push_back(index);
        } else {
            vector<int>& indexes = (*iter).second;
            indexes.push_back(index);
        }
    }
}
