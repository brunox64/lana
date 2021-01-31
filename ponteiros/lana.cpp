#include <iostream>
#include <string>
#include <regex>
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
        return msg;
    }
};

list<regex> compilarRegexList(list<string>& regexList);

int main(int argc, char** argv) {

    list<string> searchText;
    list<string> strMatches;
    list<string> strNotMatches;
    list<string> strGroups;
    list<int> groupsNumber;
    int printLines = 0;
    int completeGaps = 0;

    regex regex_group("^(-g(\\d))$");

    for (int index = 1; index < argc; index++) {
        string name = argv[index];

        if (name.compare("-pl") == 0) {
            printLines = 1;
        } else if (name.compare("-cg") == 0) {
            completeGaps = 1;
        } else if (name.compare("-t") == 0) {
            searchText.push_back(argv[index+1]);
        } else if (name.compare("-m") == 0) {
            strMatches.push_back(argv[index+1]);
        } else if (name.compare("-M") == 0) {
            strNotMatches.push_back(argv[index+1]);
        } else if (regex_match(name, regex_group)) {
            smatch smatch;
            regex_match(name, smatch, regex_group);
            if (smatch.size() == 3) {
                strGroups.push_back(argv[index+1]);
                groupsNumber.push_back(stoi(smatch[2]));
            }
        }
    }

    list<regex> regexMatches = compilarRegexList(strMatches);
    list<regex> regexNotMatches = compilarRegexList(strNotMatches);
    list<regex> regexGroups = compilarRegexList(strGroups);

    try {
        if (regexGroups.size() <= 0) {
            throw new Error("é necessáiro informar 1 ou mais grupos!");
        }

        list<string> lines;
        list<map<string,list<int> > > listOfIndexesByGroupId;

        for (int index = 0; index < regexGroups.size(); index++) {
            listOfIndexesByGroupId.push_back( map<string,list<int> >() );
        }

        for (string line; getline(cin, line); ) {
            lines.push_back(line);
        }
        
        for (string line : lines) {
            for (int indexGroup = 0; indexGroup < regexGroups.size(); index++) {
                
            }
        }
    } catch (Error& error) {
        cerr << error.what() << endl;
        return 1;
    }

    return 0;
}

list<regex> compilarRegexList(list<string>& regexList) {
    list<regex> regexListComp;

    for (auto iter = regexList.begin(); iter != regexList.end(); iter++) {
        regexListComp.push_back( regex( *iter ) );
    }

    return regexListComp;
}
