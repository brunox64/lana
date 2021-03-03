#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <map>

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


vector<regex>* compilarRegexList(list<string>& regexList);
void addCrossRefLines(vector<int>& indexes, map<string,vector<int> >& groupMap);
void addToGroup(int index, string& line, map<string,vector<int> >& groupMap, regex& regexGroup, int groupIndex);
vector<string>* processLines(vector<string>& lines);

int printLines;
vector<regex> *regexMatches;
vector<regex> *regexNotMatches;
vector<regex> *regexGroups;
vector<int> groupsNumber;
vector<string> searchText;

int main(int argc, char** argv) {

    list<string> strMatches;
    list<string> strNotMatches;
    list<string> strGroups;
    int completeGaps = 0;
    bool findReq = false;
    string findReqText;
    string fileName;
    ifstream input;

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
        } else if (name->compare("-find-req") == 0) {
            findReq = true;
            findReqText = argv[index+1];
        } else if (name->compare("-f") == 0) {
            fileName = argv[index+1];
            input.open(fileName, fstream::in);
        }
    }

    vector<string> lines;
    for (string line; getline(input, line); ) {
        lines.push_back(line);
    }

    if (input.is_open()) input.close();

    try {
        vector<string>* outLines = nullptr;

        regexMatches = compilarRegexList(strMatches);
        regexNotMatches = compilarRegexList(strNotMatches);
        regexGroups = compilarRegexList(strGroups);
            
        if (!findReq) {
            outLines = processLines(lines);
        } else {
            searchText.push_back(findReqText);

            strGroups.push_back("/\\[scheduler-tag-sync-jobs_Worker-(\\d+)\\]/g");
            groupsNumber.push_back(1);

            regexGroups = compilarRegexList(strGroups);

            outLines = processLines(lines);

            strGroups.clear();
            strGroups.push_back("/request_id=([a-zA-Z0-9\\-]+)/g");
            groupsNumber.clear();
            groupsNumber.push_back(1);

            regexGroups = compilarRegexList(strGroups);

            outLines = processLines(*outLines);
        }

        string eol("\n");
        ofstream output(fileName + ".filtered.log", fstream::out);
        for (auto line : *outLines) {
            output.write(line.c_str(), line.length());
            output.write(eol.c_str(), eol.length());
        }
        output.flush();
        output.close();
    } catch (Error& error) {
        cout << error.what() << endl;
        return 1;
    }

    return 0;
}

vector<string>* processLines(vector<string>& lines) {
    vector<string> *outLines = new vector<string>();

    if (regexGroups->size() <= 0) {
        throw Error("é necessáiro informar 1 ou mais grupos!");
    }

    vector<map<string,vector<int> > > listOfIndexesByGroupId;

    for (int index = 0; index < regexGroups->size(); index++) {
        listOfIndexesByGroupId.push_back( * new map<string,vector<int> >() );
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
                outLines->push_back(to_string(index) + ": " + lines[index]);
            } else {
                outLines->push_back(lines[index]);
            }
        }

        outLines->push_back(divider);
    }

    return outLines;
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

vector<regex>* compilarRegexList(list<string>& regexList) {
    vector<regex> *regexListComp = new vector<regex>();
    
    regex regexStart("^/");
    regex regexEnd("/([igm]+)$", regex_constants::icase);
    
    string from("I");
    string to("i");
    string barra("/");

    for (string& reg : regexList) {

        smatch sm, sm1;
        if (regex_search(reg, sm1, regexStart) && regex_search(reg, sm, regexEnd)) {
            string regstr = reg;
            int indexStart = regstr.find_first_of(barra)+1;
            int indexEnd = regstr.find_last_of(barra);
            
            regstr = regstr.substr(indexStart, indexEnd - indexStart);

            string flags = sm[1].str();

            for (int i = 0; i < flags.length(); i++) {
                if (flags[i] == from[0]) {
                    flags[i] = to[0];
                }
            }
            
            regexListComp->push_back( * new regex( regstr ) );
            // if (flags.find(to) != string::npos) {
            //     regexListComp->push_back( * new regex(regstr, regex_constants::icase ) );
            // } else {
            //     regexListComp->push_back( * new regex( regstr ) );
            // }
        } else {
            throw Error(string("Regex: " + reg + " é inválida").c_str());
        }
    }

    return regexListComp;
}
