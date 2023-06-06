#ifndef STRINGHELPER_H
#define STRINGHELPER_H
#include "Header.h"

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

void pass1Helper (vector<string> &line, string &label, string &opcode, string &operand) {
    int n = line.size();
    switch (n) {
        case 1:
            label = "";
            opcode = line[0];
            operand = "";
            break;
        case 2:
            label = "";
            opcode = line[0];
            operand = line[1];
            break;
        case 3:
            label = line[0];
            opcode = line[1];
            operand = line[2];
            break;
        default:
            cout << "Error: too many argument in line...\n";
            break;
    }
}

void pass2Helper (vector<string> &line, string &locctr, string &label, string &opcode, string &operand) {
    int n = line.size();
    switch (n) {
        case 1:
            locctr = "";
            label = "";
            opcode = line[0];
            operand = "";
            break;
        case 2:
            if (line[0] != "BASE") {
                locctr = line[0];
                label = "";
                opcode = line[1];
                operand = "";
            } else {
                locctr = "";
                label = "";
                opcode = line[0];
                operand = line[1];
            }
            break;
        case 3:
            locctr = line[0];
            label = "";
            opcode = line[1];
            operand = line[2];
            break;
        case 4:
            locctr = line[0];
            label = line[1];
            opcode = line[2];
            operand = line[3];
            break;
        default:
            cout << "Error: too many argument in line...\n";
            break;
    }
}

string toString(vector<string> strs) {
    string res = "";
    for (int i=0; i<strs.size(); i++) {
        res += strs[i];
        if (i != strs.size()-1) res += "\t";
    }
    return res;
}

string format(string format, string str) {
    const char *f = format.c_str();
    const char *s = str.c_str();
    char* buf;

    sprintf(buf, f, s);

    string res = buf;   
    return res;
}

#endif