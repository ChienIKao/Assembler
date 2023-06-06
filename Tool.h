#ifndef TOOL_H
#define TOOL_H
#include "Header.h"

int hexTodec(string hex) {
    int dec = 0;
    for (char c : hex) {
        int val = 0;
        if ('0' <= c && c <= '9') val = c - '0';
        else if ('A' <= c && c <= 'F') val = c - 'A' + 10;
        else if ('a' <= c && c <= 'f') val = c - 'a' + 10;
        else {
            cout << "Error: Wrong hexadecimal format\n";
            exit(-1);
        }
        dec = dec*16 + val;
    }
    return dec;
}

string decTohex(int dec) {
    stringstream ss;
    ss << hex << dec;
    string res (ss.str());
    for (int i=0; i<res.size(); i++) res[i] = toupper(res[i]);
    return res;
}

string LOChelper(int LOCCTR) {
    string res = decTohex(LOCCTR);
    int n = 4 - res.size();
    while (n--) res = "0" + res;
    return res;
}

int objectCodeFormat2(int opcode, short regi1, short regi2) {
	return opcode*0x100 + regi1*0x10 + regi2;
}

int objectCodeFormat3(int opcode, short N, short I, short X, short B, short P, short E, int disp) {
	return opcode*0x10000 + N*0x20000 + I*0x10000 + X*0x8000 + B*0x4000 + P*0x2000 + E*0x1000 + (disp&0x00FFF);
}

int objectCodeFormat4(int opcode, short N, short I, short X, short B, short P, short E, int addr) {
	return opcode*0x1000000 + N*0x2000000 + I*0x1000000 + X*0x800000 + B*0x400000 + P*0x200000 + E*0x100000 + addr;
}

#endif