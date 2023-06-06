#ifndef PASS2_H
#define PASS2_H
#include "Pass1.h"
#define ANS_FILE "answer.txt"

vector<string> finter; 
stringstream textss;
int pass2() {
    ss.str("");
    ss << setfill(' ') << left << setw(12) << "Loc" << setw(24) << "Source Statement" << setw(11) << "Object code";
    file.write(ANS_FILE, ss.str());

    string locctr, LABEL, OPCODE, OPERAND;
    int LOCCTR, PC, BASE;
    int line = 0;
    vector<string> LINE;

    // read first input line from intermediate file
    LINE = split(finter[line]);
    string startAddr;
    pass2Helper(LINE, locctr, LABEL, OPCODE, OPERAND);
    if (OPCODE == "START") {
        PC = stoi(OPERAND);
        startAddr = OPERAND;

        ss.str("");
        ss << setfill('0') << setw(4) << hex << uppercase << PC << "    ";
        ss << left << setfill(' ') << setw(8) << LABEL;
        ss << left << setfill(' ') << setw(8) << OPCODE;
        ss << left << setfill(' ') << setw(8) << startAddr;
        file.write(ANS_FILE, ss.str());

        ss.str("");
        ss << "H^" << left << setfill(' ') << setw(6) << LABEL;
        ss << "^" << right << setfill('0') << setw(6) << hex << uppercase << startAddr;
        ss << "^" << right << setfill('0') << setw(6) << hex << uppercase << objLen;
        file.write(OBJ_FILE, ss.str());
    } else {
        cout << "Error: No START at first line, in line " << line << endl;
        return -1;
    }

    // read next line
    string textRecord, T;
    vector<string> modifyRecord;
    int objNum = 0, objLen = 0;
    line += 1;  
    while (true) {
        LOCCTR = PC;
        // default format 3
        int n = 1, i = 1, x = 0, b = 0, p = 0, e = 0, format = 0;
        LINE = split(finter[line]);
        pass2Helper(LINE, locctr, LABEL, OPCODE, OPERAND);

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
            ss << "           ";
            for (int k=1; k<LINE.size(); k++) ss << " " << LINE[k];
            file.write(ANS_FILE, ss.str());
            line += 1;
            continue;
        }
        
        // immediate addressing
        if (OPERAND[0] == '#') {
            n = 0;
            i = 1;
            OPERAND = OPERAND.substr(1);
        }

        // indirect addressing
        if (OPERAND[0] == '@') {
            n = 1;
            i = 0;
            OPERAND = OPERAND.substr(1);
        }        

        // base opcode 
        if (OPCODE == "BASE") {
            if (symtab.count(OPERAND)) {
                BASE = symtab[OPERAND];
                ss.str("");
                ss << setw(16) << setfill(' ') << ' ';
                ss << left << setfill(' ') << setw(8) << OPCODE;
                ss << left << setfill(' ') << setw(8) << OPERAND;
                // ss.str("");
                // ss << "\t\t\t" << OPCODE << "\t" << OPERAND;
                file.write(ANS_FILE, ss.str());
                line += 1;
                continue;
            } else {
                cout << "Error: Undefined base operand '" << OPCODE << "', in line " << line << endl;
                return -1;
            }
        }

        // last line
        if (OPCODE == "END") {
            ss.str("");
            ss << setw(16) << setfill(' ') << ' ';
            ss << left << setfill(' ') << setw(8) << OPCODE;
            ss << left << setfill(' ') << setw(8) << OPERAND;
            // ss.str("");
            // ss << "\t\t\t" << OPCODE << "\t" << OPERAND;
            file.write(ANS_FILE, ss.str());
            
            ss.str("");
            ss << "^" << setfill('0') << setw(2) << right << hex << uppercase << objLen;
            textRecord = textRecord + ss.str() + textss.str();
            file.write(OBJ_FILE, textRecord);
            // ss.str("");
            // ss << "T" << left << setfill('0') << setw(2) << hex << uppercase << objLen;
            // ss << textRecord;
            // file.write(OBJ_FILE, ss.str());
            
            for (string m : modifyRecord) {
                file.write(OBJ_FILE, m);
            }
            
            ss.str("");
            ss << "E^" << right << setfill('0') << setw(6) << hex << uppercase << startAddr;
            file.write(OBJ_FILE, ss.str());
            break;
        }

        // read next PC
        string nextLoc = split(finter[line+1])[0];
        PC = hexTodec(nextLoc);
        
        // search OPCODE in optab
        int TA = 0, disp = 0, regi1 = 0, regi2 = 0, objCode = 0;
        string SEC_OPERAND;
        if (optab.count(OPCODE)) { // instruction
            if (OPERAND != "") {
                size_t found = OPERAND.find(",");
                if (found != string::npos) {
                    SEC_OPERAND = OPERAND.substr((int)found + 1);
                    OPERAND = OPERAND.substr(0, (int)found);
                }
                if (REGISTER.count(OPERAND)) {
                    regi1 = REGISTER[OPERAND];
                    if (SEC_OPERAND != "") {
                        if (REGISTER.count(SEC_OPERAND)) {
                            regi2 = REGISTER[SEC_OPERAND];
                        } else {
                            cout << "Error: Invalid second register name '" << SEC_OPERAND << "', in line " << line << endl;
                            return -1;
                        }
                    }
                    format = 2;
                } else {
                    // search symtab for operand
                    if (symtab.count(OPERAND)) {
                        if (SEC_OPERAND == "X") {
                            x = 1;
                        }
                        TA = symtab[OPERAND];
                        disp = TA - PC;
                        if (-2048 <= PC && disp <= 2047) { // PC-relative mode
                            p = 1;
                            format = 3;
                        } else {
                            disp = TA - BASE;
                            if (0 <= disp && disp <= 4095) { // Base-relative mode
                                b = 1;
                                format = 3;
                            } else {
                                if (e == 1) { // direct address: format 4
                                    disp = TA;
                                    format = 4;
                                    // append modify record
                                    ss.str("");
                                    ss << "M^" << right << setfill('0') << setw(6) << hex << uppercase << LOCCTR + 1 << "^05";
                                    modifyRecord.push_back(ss.str());
                                } else {
                                    // for SIC
                                    n = 0, i = 0;
                                    format = 3;
                                }
                            }
                        }
                    } else {
                        if (n == 0 && i == 1) { // immediate address (#)
                            disp = stoi(OPERAND);
                            if (disp >= 0x1000) format = 4;
                            else format = 3;
                        } else {
                            cout << "Error: Undefined symbol '" << OPERAND << "', in line " << line << endl;
                            return -1;
                        }
                    }
                }
            } else { // no operand
                TA = 0;
                format = 3;
            }

            // create object code
            if (format == 2) {
                objCode = objectCodeFormat2(optab[OPCODE], regi1, regi2);
            } else if (format == 3) {
                objCode = objectCodeFormat3(optab[OPCODE], n, i, x, b, p, e, disp);
            } else {
                objCode = objectCodeFormat4(optab[OPCODE], n, i, x, b, p, e, disp);
            }
        } else if (OPCODE == "BYTE") {
            int len = OPERAND.size() - 3;
            string SEC_OPERAND = OPERAND.substr(2, len);
            if (OPERAND[0] == 'C') { // character
                for (char c : SEC_OPERAND) objCode = objCode * 0x100 + c;
                format = len; // one character one byte
            } else if (OPERAND[0] == 'X') { // hex
                objCode = hexTodec(SEC_OPERAND);
                format = (len + 1) / 2; // two hex one byte
            } else {
                cout << "Error: Invalid byte type '" << OPERAND << "', in line " << line << endl;
                return -1;
            }
        } else if (OPCODE == "WORD") {
            objCode = stoi(OPERAND);
            format = 3;
        } else if (OPCODE == "RESW" || OPCODE == "RESB") {
            // do nothing
        } else {
            cout << "Error: Undefined opcode '" << OPCODE << "', in line " << line << endl;
            return -1;
        }

        if (objLen + format > 30) {
            ss.str("");
            ss << "^" << setfill('0') << setw(2) << right << hex << uppercase << objLen;
            textRecord = textRecord + ss.str() + textss.str();
            file.write(OBJ_FILE, textRecord);
            textss.str(""); // clear string stream
            objLen = 0;
            objNum = 0;
            textRecord = "";
        }

        objLen += format;

        string tmp;
        if (SEC_OPERAND != "") OPERAND = OPERAND + "," + SEC_OPERAND;
        if (format == 0) {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "    ";
            ss << left << setfill(' ') << setw(8) << LABEL;
            ss << left << setfill(' ') << setw(8) << OPCODE;
            ss << left << setfill(' ') << setw(8) << OPERAND;
            file.write(ANS_FILE, ss.str());
            // file.write(ANS_FILE, finter[line]);
        } else if (format == 1) {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "    ";
            ss << left << setfill(' ') << setw(8) << LABEL;
            ss << left << setfill(' ') << setw(8) << OPCODE;
            ss << left << setfill(' ') << setw(12) << OPERAND;
            // file.write(ANS_FILE, ss.str());
            ss << right << setw(2) << setfill('0') << hex << uppercase << objCode;
            file.write(ANS_FILE, ss.str());
            // ss.str("");
            // ss << right << setw(2) << setfill('0') << hex << uppercase << objCode;
            // tmp = ss.str();
            textss << "^" << right << setw(2) << setfill('0') << hex << uppercase << objCode;
        } else if (format == 2) {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "    ";
            ss << left << setfill(' ') << setw(8) << LABEL;
            ss << left << setfill(' ') << setw(8) << OPCODE;
            ss << left << setfill(' ') << setw(12) << OPERAND;
            ss << right << setw(4) << setfill('0') << hex << uppercase << objCode;
            // ss.str("");
            // ss << finter[line] << "\t" << right << setw(4) << setfill('0') << hex << uppercase << objCode;
            file.write(ANS_FILE, ss.str());
            // ss.str("");
            // ss << right << setw(4) << setfill('0') << hex << uppercase << objCode;
            // tmp = ss.str();
            textss << "^" << right << setw(4) << setfill('0') << hex << uppercase << objCode;
        } else if (format == 3) {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "    ";
            ss << left << setfill(' ') << setw(8) << LABEL;
            if (n == 0 && i == 1) {
                ss << left << setfill(' ') << setw(7) << OPCODE;
                ss << left << setfill(' ') << setw(13) << "#" + OPERAND;
            } else if (n == 1 && i == 0) {
                ss << left << setfill(' ') << setw(7) << OPCODE;
                ss << left << setfill(' ') << setw(13) << "@" + OPERAND;
            }else {
                ss << left << setfill(' ') << setw(8) << OPCODE;
                ss << left << setfill(' ') << setw(12) << OPERAND;
            }
            ss << right << setw(6) << setfill('0') << hex << uppercase << objCode;
            // ss.str("");
            // ss << finter[line] << "\t" << right << setw(6) << setfill('0') << hex << uppercase << objCode;
            file.write(ANS_FILE, ss.str());
            // ss.str("");
            // ss << right << setw(6) << setfill('0') << hex << uppercase << objCode;
            // tmp = ss.str();
            textss << "^" << right << setw(6) << setfill('0') << hex << uppercase << objCode;
        } else if (format == 4) {
            ss.str("");
            ss << setfill('0') << setw(4) << hex << uppercase << LOCCTR << "    ";
            ss << left << setfill(' ') << setw(7) << LABEL;
            if (n == 0 && i == 1) {
                ss << left << setfill(' ') << setw(7) << "+" + OPCODE;
                ss << left << setfill(' ') << setw(13) << "#" + OPERAND;
            } else if (n == 1 && i == 0) {
                ss << left << setfill(' ') << setw(7) << "+" + OPCODE;
                ss << left << setfill(' ') << setw(13) << "@" + OPERAND;
            } else {
                ss << left << setfill(' ') << setw(9) << "+" + OPCODE;
                ss << left << setfill(' ') << setw(12) << OPERAND;
            }
            ss << right << setw(8) << setfill('0') << hex << uppercase << objCode;
            // ss.str("");
            // ss << finter[line] << "\t" << right << setw(8) << setfill('0') << hex << uppercase << objCode;
            file.write(ANS_FILE, ss.str());
            // ss.str("");
            // ss << right << setw(8) << setfill('0') << hex << uppercase << objCode;
            // tmp = ss.str();
            textss << "^" << right << setw(8) << setfill('0') << hex << uppercase << objCode;
        }

        // textRecord += tmp;

        // cnt number of object code
        if (objNum == 0) {
            ss.str("");
            ss << "T^" << setfill('0') << setw(6) << right << hex << uppercase << LOCCTR;
            textRecord = ss.str();
        }
        objNum += 1;

        // if over 10 object code, change to next line
        // if (objNum == 9) {
        //     ss.str("");
        //     ss << setfill('0') << setw(2) << right << hex << uppercase << objLen;
        //     textRecord = T + ss.str() + textRecord;
        //     file.write(OBJ_FILE, textRecord);
        //     objNum = 0;
        //     objLen = 0;
        //     textRecord = "";
        // }

        line += 1;
    }
    cout << ANS_FILE << " complete\n";
    cout << OBJ_FILE << " complete\n";
    return 0;
}

#endif