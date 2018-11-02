#include "matrixMultiplication.h"
#include "mergeSort.h"


using namespace std;


int main() {
    int n = 0;
    cout << "Choose 1 for matrix multiplication or 2 for mergesort"<< endl;
    cin >> n;
    switch (n){
        case 1: {
            matrixMultiplication m;
            m.begin("input.txt");
            break;
        }
        case 2: {
            mergeSort m;
            m.begin("input.txt");
            break;
        }
        default: cout << "Invalid Choice";
        exit(EXIT_FAILURE);
    }

    return 0;
}