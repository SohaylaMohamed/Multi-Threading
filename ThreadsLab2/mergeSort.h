//
// Created by sohayla on 01/11/18.
//

#ifndef THREADSLAB2_MERGESORT_H
#define THREADSLAB2_MERGESORT_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
class mergeSort {
private :
    vector<int> array;
    vector<int> result;
    vector<pthread_t> threads;
    int nThreads = 0;
    int mergeN = 1;
    int size;
    void readFileMerge(string filename);
    void merge(vector<int>* a,vector<int> l, vector<int> r);
    static void * mergesortHandler(void *arg);

    void * mergeSortF(void* arg);
    void mainThread();

public:
    void begin(string filename);

};


#endif //THREADSLAB2_MERGESORT_H
