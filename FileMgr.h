#ifndef FILEMGR_H
#define FILEMGR_H
#include "Header.h"

class FileMgr {
private:
    fstream file;
public:
    vector<string> read(string path) {
        vector<string> datas;
        this->file.open(path, ios::in);

        if (this->file.fail()) {
            cout << "Error: " << path << " not found...\n";
            exit(-1);
        }

        string line;
        while (getline(file, line)) {
            datas.push_back(line);
            // cout << line << endl;
        }
        this->file.close();
        return datas;
    }   

    void write(string path, string line) {
        this->file.open(path, ios::app);

        if (this->file.fail()) {
            cout << "Error: " << path << " not found...\n";
            exit(-1);
        }

        this->file << line << "\n";
        this->file.close();
    }

    void clear(string path) {
        this->file.open(path, ios::out);

        if (this->file.fail()) {
            cout << "Error: " << path << " not found...\n";
            exit(-1);
        }

        this->file << "";
        this->file.close();
    }
};

#endif