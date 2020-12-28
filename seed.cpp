#include <iostream>
#include <random>
#include <fstream>
#include <time.h>
#include <vector>
using namespace std;

const int MAX_DATA = 2250000;
// Used for testing
ofstream wrt;

int main() {
    vector<string> files;
    files.push_back("file1.txt");
    files.push_back("file2.txt");
    files.push_back("file3.txt");
    files.push_back("file4.txt");

    for(int j = 0; j<files.size(); j++) {
        ofstream wrt(files[j]);
        srand(time(NULL));
        for(int i = 0; i< MAX_DATA; i++) {
            int rando = rand() % 100000;
            wrt << rando << endl;
        }
        wrt.clear();
        wrt.close();
    }


    return 0;
}