#include <bits/stdc++.h>
#include "FileMgr.h"
// #include "StringMgr.h"
using namespace std;

vector<string> split(string line) {
    vector<string> res;
    string tmp = "";
    for (char c : line) {
        if (c == ' ' || c == '\t') {
            if (tmp.size() != 0) {
                res.push_back(tmp);
                tmp = "";
            }
        } else {
            tmp += c;
        }
        if (c == '\n' && tmp.size() != 0) {
            res.push_back(tmp);
            tmp = "";
        }
    }
    if (tmp.size() != 0) res.push_back(tmp);
    return res;
}

int main () {
    FileMgr file;
    vector<string> fsrc = file.read("source.txt");
    for (string s : fsrc) {
        vector<string> line = split(s);
        for (int i=0; i<line.size(); i++) {
            cout << i << ":" << line[i] << " ";
        }
        cout << endl;
    }
}