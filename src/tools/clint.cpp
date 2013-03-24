#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

using namespace std;

typedef vector<string> Command;

Command getCommand(){
    Command cmd;
    string line;
    getline(cin, line);
    string arg;
    bool quotes=false, escaping=false;
    for(string::const_iterator i=line.begin(); i!=line.end(); i++){
        if(escaping){
            arg += *i;
            escaping = false;
        }
        else if(*i == '\\')
            escaping = true;
        else if(*i == '"')
            quotes = !quotes;
        else if(quotes)
            arg += *i;
        else if(*i == ' '){
            if(!arg.empty()){
                cmd.push_back(arg);
                arg.clear();
            }
        }
        else arg += *i;
    }
    if(!arg.empty()) cmd.push_back(arg);
    return cmd;
}

bool number(const std::string &str, unsigned &n){
    if(str.empty()) return false;
    for(std::string::const_iterator i=str.begin(); i!=str.end(); i++){
        if(*i < '0' || *i > '9') return false;
    }
    n = atoi(str.c_str());
    return true;
}

int main(int argc, char **argv){
    vector<unsigned> data;
    Command cmd;
    while(1){
        Command cmd = getCommand();
        // exit / quit / ^D
        if(cmd.empty() && cin.eof()){
            cout << "exit" << endl;
            break;
        }
        else if(cmd.empty());
        else if((cmd[0] == "exit" || cmd[0] == "quit") && cmd.size() == 1)
            break;
        // size
        else if(cmd[0] == "size" && cmd.size() == 1)
            cout << data.size();
        // get INDEX
        else if(cmd[0] == "get" && cmd.size() == 2){
            unsigned idx;
            if(!number(cmd[1], idx))
                cerr << "get: expecting an index" << endl;
            else if(idx >= data.size()){
                cerr << "get: index " << idx << " out of range" << endl;
                cout << 0;
            }
            else
                cout << data[idx];
        }
        // set INDEX VALUE
        else if(cmd[0] == "set" && cmd.size() == 3){
            unsigned idx, val;
            if(!number(cmd[1], idx) || !number(cmd[2], val))
                cerr << "get: expecting numbers" << endl;
            else{
                if(idx >= data.size()) data.resize(idx+1, 0);
                cout << (data[idx] = val);
            }
        }
        // increment INDEX
        else if(cmd[0] == "increment" && cmd.size() == 2){
            unsigned idx;
            if(!number(cmd[1], idx))
                cerr << "increment: expecting an index" << endl;
            else{
                if(idx >= data.size()) data.resize(idx+1, 0);
                cout << ++data[idx];
            }
        }
        // load FILE
        else if(cmd[0] == "load" && cmd.size() == 2){
            ifstream in(cmd[1].c_str(), ios::binary);
            if(!in) cerr << "couldn't open " << cmd[1] << " for reading" << endl;
            else{
                in.seekg(0, ios::end);
                int length = in.tellg()/sizeof(unsigned);
                in.seekg(0, ios::beg);
                data.resize(length);
                in.read((char*)&data[0], sizeof(unsigned)*length);
                in.close();
            }
        }
        // save FILE
        else if(cmd[0] == "save" && cmd.size() == 2){
            ofstream out(cmd[1].c_str(), ios::binary);
            if(!out) cerr << "couldn't open " << cmd[1] << " for writing" << endl;
            else{
                out.write((char*)&data[0], sizeof(unsigned)*data.size());
                out.close();
            }
        }
        else
            cerr << "unknown command: " << cmd[0] << endl;
        cout << endl;
    }
    return 0;
}
