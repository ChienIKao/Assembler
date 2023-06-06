#include "Pass2.h"

int main () {
    cout << "=== SIC/XE Assembler ===\n\n";
    cout << "==== Pass 1 Start ====\n\n";   

    fsrc = file.read(SOURCE_FILE);
    file.clear(INTER_FILE);
    if (pass1() != 0) {
        cout << "\n\nError: Something went wrong in pass 1\n";
        exit(-1);
    }

    cout << "\n==== Pass 1 End ====\n\n";
    cout << "==== Pass 2 Start ====\n\n";

    finter = file.read(INTER_FILE);
    file.clear(ANS_FILE);
    file.clear(OBJ_FILE);
    if (pass2() != 0) {
        cout << "\n\nError: Something went wrong in pass 2\n";
        exit(-1);
    }

    cout << "\n==== Pass 2 End ====\n\n";
    
    system("PAUSE");
    return 0;
}