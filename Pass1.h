#ifndef PASS1_H
#define PASS1_H
#include "Header.h"
#include "FileMgr.h"
#include "StringHelper.h"
#include "Tool.h"
#include "Optab.h"
#define SOURCE_FILE "source.txt"
#define INTER_FILE "intermediate.txt"
#define SYMTAB_FILE "symble_table.txt"
#define OBJ_FILE "object.txt"

map<string, int> symtab;
map<string, int> REGISTER = {
    {"A", 0}, {"X", 1}, {"L", 2},
    {"B", 3}, {"S", 4}, {"T", 5},
    {"F", 6}, {"PC", 8}, {"SW", 9}
};

vector<string> fsrc;
FileMgr file;
int objLen = 0;

stringstream ss;
int pass1 () {
    string LABEL, OPCODE, OPERAND;
    int LOCCTR = 0, startAddr = 0, line = 0;
    vector<string> LINE;
    
    // read first line from source file
    LINE = split(fsrc[line]);
    pass1Helper(LINE, LABEL, OPCODE, OPERAND);
    if (OPCODE == "START") {
        startAddr = hexTodec(OPERAND);
        LOCCTR = startAddr;

        ss.str("");
        ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "\t" << fsrc[0];
        // file.write(INTER_FILE, LOChelper(LOCCTR) + "\t" + fsrc[0]);
        file.write(INTER_FILE, ss.str());
    } else {
        cout << "Error: No START at first line, in line " << line << endl;
        return -1;
    }

    // read next line
    line += 1;
    while (true) {
        int e = 0;
        LINE = split(fsrc[line]);
        pass1Helper(LINE, LABEL, OPCODE, OPERAND);
        // cout << "LABEL:" << LABEL << " OPCODE:" << OPCODE << " OPERAND:" << OPERAND << endl;

        // format 4
        if (OPCODE[0] == '+') {
            e = 1;
            OPCODE = OPCODE.substr(1);
        }

        // skip comment 
        if (LABEL[0] == '#') {
            line += 1;
            continue;
        }
        if (LABEL[0] == '.' || OPCODE[0] == '.') {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "\t" << fsrc[line];
            file.write(INTER_FILE, ss.str());
            line += 1;
            continue;
        }
        
        // base opcode
        if (OPCODE == "BASE") {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "\t" << fsrc[line];
            file.write(INTER_FILE, ss.str());
            line += 1;
            continue;
        }

        // last line
        if (OPCODE == "END") {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "\t" << fsrc[line];
            file.write(INTER_FILE, ss.str());
            objLen = LOCCTR - startAddr;
            break;
        }

        // write intermediate file
        ss.str("");
        ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "\t" << fsrc[line];
        // file.write(INTER_FILE, LOChelper(LOCCTR) + "\t" + fsrc[line]);
        file.write(INTER_FILE, ss.str());

        // add label and addr into symbol table
        if (LABEL != "") {
            // check duplicate label
            if (symtab.count(LABEL)) {
                cout << "Error: Duplicate label '" << LABEL << "', in line " << line << endl;
                return -1;
            }
            symtab[LABEL] = LOCCTR;
        }

        // calculate next LOCCTR
        // search opcode in optab
        if (optab.count(OPCODE)) { // instruction
            size_t found = OPERAND.find(",");
            if (found != string::npos) {
                OPERAND = OPERAND.substr(0, (int)found);
            }
            if (REGISTER.count(OPERAND)) LOCCTR += 2;
            else if (e == 0) LOCCTR += 3;
            else LOCCTR += 4;
        } else if (OPCODE == "BYTE") {
            // length of operand
            int len = OPERAND.size() - 3;
            string SEC_OPERAND = OPERAND.substr(2, len);
            if (OPERAND[0] == 'C') LOCCTR += len;
            else if (OPERAND[0] == 'X') LOCCTR += (len + 1) / 2;
            else {
                cout << "Error: Invalid byte type '" << OPERAND[0] << "', in line " << line << endl;
                return -1;
            }

        } else if (OPCODE == "WORD") LOCCTR += 3;
        else if (OPCODE == "RESB") LOCCTR += stoi(OPERAND);
        else if (OPCODE == "RESW") LOCCTR += 3 * stoi(OPERAND);
        else {
            cout << "Error: Invalid opcode '" << OPCODE << "', in line " << line << endl;
            return -1;
        }
        line += 1;
    }
    cout << INTER_FILE << " complete" << endl;

    // write symtab file 
    file.clear(SYMTAB_FILE);
    for (auto sym : symtab) {
        file.write(SYMTAB_FILE, sym.first + "\t" + LOChelper(sym.second));
    }
    for (auto regi : REGISTER) {
        file.write(SYMTAB_FILE, regi.first + "\t" + to_string(regi.second));
    }
    cout << SYMTAB_FILE << " complete\n";
    return 0;
}

#endif