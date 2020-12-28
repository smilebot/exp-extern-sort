// Goal 1: Use 10 MB of memory to load the data and sort it.
// Goal 2: Merge all the data and produce a sorted file.

#include<iostream>
#include<fstream>
#include<vector>
#include<queue>


using namespace std;
int MAX_BUFF = 10000000;
const int NUM_ELEMENTS = MAX_BUFF/32;

ifstream inputStream;

void swap(vector<int> &vec, int left, int right) {
    int temp = vec[left];
    vec[left] = vec[right];
    vec[right] = temp;
}


int partition(vector<int> &chunkVector, int start, int end) {
    int pivot = chunkVector[start];
    int lp = start;

    for(int x = lp+1; x<=end; x++) {
        if(chunkVector[x] < pivot) {
            lp++;
            swap(chunkVector, x, lp);
        }
    }

    swap(chunkVector, start, lp);

    return lp;

}

void quickSort(vector<int> &chunkVector, int start, int end) {
    if(start < end) {
        int pivot = partition(chunkVector, start, end);
        quickSort(chunkVector, start, pivot-1);
        quickSort(chunkVector, pivot+1, end);
    }

}


void splitIntoChunks(string fileName, vector<string> &fileNames) {
    inputStream.clear();
    inputStream.open(fileName.c_str());
    vector<string> chunkFileNames;

    // TODO: Change to primitive int arr for performance 
    vector<int> chunkVector;

    if(!inputStream) {
        cout << "Error reading file " << fileName << endl;
    }

    int cur_buff = 0;
    int cur_chunk = 0;
    string chunk_prefix = fileName+"_chunk_";
    string cur_chunk_file_name = chunk_prefix + to_string(cur_chunk);

    // create an array which holds 32 bit int data

    int num;

    while(inputStream >> num) {
        chunkVector.push_back(num);
        if(chunkVector.size() == NUM_ELEMENTS) {
            
            // perform quicksort on chunkVector;
            quickSort(chunkVector, 0, chunkVector.size()-1);
            
            // write to cur_chunk_file_name
            ofstream output_stream(cur_chunk_file_name);
            output_stream.clear();
            
            for(int val : chunkVector) {
                output_stream << val << endl;
            }

            chunkVector.clear();

            // add the current file_name to chunkFileNames

            fileNames.push_back(cur_chunk_file_name);

            // update cur_chunk && cur_chunk_file_name;

            cur_chunk++;
            cur_chunk_file_name = chunk_prefix + to_string(cur_chunk);

            // update output_stream;
            output_stream.close();
        }
    }

    if(chunkVector.size()) {
        quickSort(chunkVector, 0, chunkVector.size()-1);
        ofstream output_stream(cur_chunk_file_name);
        output_stream.clear();
        for(int val : chunkVector) {
            output_stream << val << endl;
        }
        chunkVector.clear();
        output_stream.close();
        fileNames.push_back(cur_chunk_file_name);
    }

    inputStream.close();
}


// Here we learn about some of the limits of our operating system:
// There is a scenario where we have a relatively large file ex: 1TB
// and we use a buffer of 1MB. We will split the data into 1000000 chunks (files).
// When we want to combine them, we read all the files in a k-merge algorithm and fill our buffer.
// There is a limit to how many files we can have open on my computer it is 98304 files.
// This can be found using `sysctl kern.maxfiles` (mac) and `ulimit -a` for ubuntu
// If you are working with a small RAM you will be able to open fewer files.

// WIP: Doesn't consider memory implications

void combineChunks(vector<string> &fileNames) {
    // initializing;
    vector<ifstream*> chunkedFilePointers;
    
    priority_queue <int, vector<int>, greater<int> > pq;

    for(int i = 0; i<fileNames.size(); i++) {
        ifstream *n = new ifstream;
        n->open(fileNames[i]);
        chunkedFilePointers.push_back(n);
    }

    ofstream out("output.txt");

    for(ifstream *ptr: chunkedFilePointers) {
        int num;
        while(*ptr >> num) {
            pq.push(num);
        }
        delete ptr;
    }

    while(pq.size()) {
        out << pq.top() <<endl;
        pq.pop();
    }

    for(string fileName: fileNames) {
        remove(fileName.c_str());
    }
}

int main() {
    remove("output.txt");
    vector<string> inputFiles;
    cout << "Enter the file names that contain data : (type done)" << endl;
    string file;
    while(true) {
        cin >> file;
        if(file == "done") break;
        inputFiles.push_back(file);
    }
    cout << "Sorting through the following files :" << endl;
    for(string x: inputFiles) {
        cout << x << endl;
    }

    // 1. sort each file into a new sorted file 
    
    // 1.a Read MAX_BUFF chunks and store in new files
    

    // TODO: Change from vec to storing into a folder
    vector<string> fileNames;
    
    for(int i = 0; i < inputFiles.size(); i++){
        cout << "Breaking " << inputFiles[i] << " into chunks" << endl;
        splitIntoChunks(inputFiles[i], fileNames);
    }

    cout << "Combining chunks" << endl;

    combineChunks(fileNames);

    return 0;
}