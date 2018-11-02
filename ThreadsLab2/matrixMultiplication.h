//
// Created by sohayla on 01/11/18.
//
#include <iostream>
#include <string>
#include <sstream>
#include<fstream>
#include <vector>
#include <pthread.h>
#include <mutex>
#ifndef THREADSLAB2_MATRIXMULTIPLICATION_H
#define THREADSLAB2_MATRIXMULTIPLICATION_H

using  namespace std;
class matrixMultiplication {

private:
    vector<int > sizeA;
    vector<int > sizeB;
    vector<vector<int>> a;
    vector<vector<int>> b;
    vector<vector<int>> result1;
    vector<vector<int>> result2;
    double t1, t2;
    mutex c;
    int m1_row = 0;
    int m1_col = 0;
    void readFileMult(string filename);
    static void *computeARowHelper(void *context);
    static void *computeAnElementHelper(void *context);
    void* computeAnElement(void);
    void* computeARow(void);
    void matrixMult1();
    void matrixMult2();
    void writeOutputFile();
public:
    void begin(string filename);


};


#endif //THREADSLAB2_MATRIXMULTIPLICATION_H
