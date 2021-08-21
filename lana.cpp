#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <map>

using namespace std;

vector<string*>* addToGroup(regex &reg, map<string,vector<string*>*> &groups, string *line) {
    smatch sm;
    if (regex_search(*line, sm, reg)) {
        string key = sm[0].str();
        if (groups.find(key) == groups.end()) {
            groups[key] = new vector<string*>();
        }
        groups[key]->push_back(line);
        return groups[key];
    }
    return nullptr;
}

void addToQueue(string *line, smatch &smenqtt, regex &regenqtt, map<string,vector<string*>*> &queueptt, map<string,vector<string*>*> &enqtt) {
    if (regex_search(*line, smenqtt, regenqtt)) {
        string key = smenqtt[0].str();
        if (queueptt.find(key) == queueptt.end()) {
            queueptt[key] = new vector<string*>();
        }
        vector<string*>* vec = queueptt[key];
        for (auto it : *enqtt[key]) {
            vec->push_back(it);
        }
    }
}

int main(int argc, char** argv) {
    bool debug = false;
    for (int index = 1; index < argc; index++) {
        if (strcmp(argv[index], "debug") == 0) {
            debug = true;
            break;
        }
    }

    if (debug) cout << "starting input reading..." << endl;

    vector<string*> lines;

    for (string line; getline(cin, line); ) {
        lines.push_back(new string(line));
    }

    if (debug) cout << "starting process..." << endl;

    map<string,vector<string*>*> enqToTag, tagMsgProc;
    map<string,vector<string*>*> enqToPms, pmsMsgProc;

    regex regEnqToTag("tag_enqueuing_id=([a-zA-Z0-9-_]+)"), regTagMsgProc("tag_message_processing_id=([a-zA-Z0-9-_]+)");
    regex regEnqToPms("pms_enqueuing_id=([a-zA-Z0-9-_]+)"), regPmsMsgProc("pms_message_processing_id=([a-zA-Z0-9-_]+)");

    int count = 0;
    bool anyMatch;
    vector<string*> *last = nullptr, *cur = nullptr;
    for (string *line : lines) {
        anyMatch = false;

        cur = addToGroup(regEnqToTag, enqToTag, line);
        if (cur != nullptr) {
            last = cur;
            anyMatch = true;
        }

        cur = addToGroup(regTagMsgProc, tagMsgProc, line);
        if (cur != nullptr) {
            last = cur;
            anyMatch = true;
        }
        
        cur = addToGroup(regEnqToPms, enqToPms, line);
        if (cur != nullptr) {
            last = cur;
            anyMatch = true;
        }
        
        cur = addToGroup(regPmsMsgProc, pmsMsgProc, line);
        if (cur != nullptr) {
            last = cur;
            anyMatch = true;
        }
        
        if (!anyMatch && last != nullptr) {
            last->push_back(line);
        }

        if (debug) {
            count++;
            cout << "processing line...:" << count << ":" << lines.size() << " : " << (count/((float)lines.size())*100) << "%" << endl;
        }
    }

    if (debug) cout << "starting relation of message with queues..." << endl;

    map<string,vector<string*>*> queueTagToPms, queuePmsToTag;

    regex regMsgHash("queue_message_hash=([a-zA-Z0-9-_]+)");

    smatch smMsgHash, smEnqToTag, smTagMsgProc, smEnqToPms, smPmsMsgProc;
    for (auto line : lines) {
        if (regex_search(*line, smMsgHash, regMsgHash)) {
            addToQueue(line, smEnqToTag, regEnqToTag, queuePmsToTag, enqToTag);
            addToQueue(line, smTagMsgProc, regTagMsgProc, queuePmsToTag, tagMsgProc);
            addToQueue(line, smEnqToPms, regEnqToPms, queueTagToPms, enqToPms);
            addToQueue(line, smPmsMsgProc, regPmsMsgProc, queueTagToPms, pmsMsgProc);
        }
    }

    if (debug) cout << "starting write output..." << endl;

    for (auto it : queuePmsToTag) {
        for (auto ln : *it.second) {
            cout << *ln << endl;
        }
    }
    for (auto it : queueTagToPms) {
        for (auto ln : *it.second) {
            cout << *ln << endl;
        }
    }

    if (debug) cout << "finished..." << endl;

    return 0;
}
