//
// Created by sohayla on 01/11/18.
//


#include "mergeSort.h"
vector<int> arrayTmp;

/**
 * read the array of integers from an input file
 * @param filename the name of the input file
 */
void mergeSort::readFileMerge(string filename) {
    ifstream mergeF;
    //open the file
    mergeF.open(filename);
    if(mergeF.is_open()){     //check if the file is opened
        string line;
        stringstream ss;
        int temp;
        //read first line which contains the size of the array
        getline(mergeF, line);
        ss << line;
        ss >> size;
        // cout << size << " " << endl;

        //read the array elements to be sorted
        ss.clear();
        ss.str(string());
        getline(mergeF, line);
        ss << line;
        while(ss >> temp) {
            array.push_back(temp);
            //   cout << temp << " ";
        }
        //   cout << endl;
    } else {
        //   cout << "File was not found" << endl;
        return;
    }
}

/**
 * merging two sorted arrays into one
 * @param a pointer to array to hold the final sorted array
 * @param l left-sorted array
 * @param r right-sorted array
 */
 void mergeSort::merge(vector<int> *a,vector<int> l, vector<int> r) {
     //total number of elements in the final array
    int totElem = l.size() + r.size();
    //clear the result array to put new values in it
    a->clear();
    //i is index for a
    //li is index for l
    //ri is index for r
    int i,li,ri;
    i = li = ri = 0; // inetialise all indices
    //loop over all elements
    while ( i < totElem) {
        if ((li < l.size()) && (ri<r.size())) {   //if li, ri are still in bounds for the arrays
            if (l[li] < r[ri]) { //if element in left array is smaller than the one in the right
                a->push_back(l[li]);
                i++;
                li++;
            } else {//if the element in right is bigger or equal the one in left
                a->push_back(r[ri]);
                i++;
                ri++;
            }
        } else { // one of li or ri is out of bound
            //check which array has not finished yet and add all their elements to the result
            if (li >= l.size()) {
                while (ri < r.size()) {
                    a->push_back(r[ri]);
                    i++;
                    ri++;
                }
            }
            if (ri >= r.size()) {
                while (li < l.size()) {
                    a->push_back(l[li]);
                    li++;
                    i++;
                }
            }
        }
    }


}

/**
 * used as a bridge between threads, which is passed
 * to as a parameter, and the code to merge sort
 * @param args holds the parameters passed to the computing function
 * which is sub-array of the original array
 * @return thread results
 */
void* mergeSort::mergesortHandler(void *arg) {
    return ((mergeSort*) arg) -> mergeSortF(arg);
}

/**
 * gets a subarray of the original as a parameter
 * creates two threads for each division done for the array
 * until the two arrays are each holding one element
 * then it starts merging from down up
 * @param arg sub-array
 * @return
 */
void* mergeSort::mergeSortF(void * arg) {
    vector<int> *temp = (vector<int> *)arg;
    //get the mid point in the sub-array
    int midP = static_cast<int>(0 + (temp->size() - 1) / 2);
    pthread_t t1,t2;
    vector<int> arr1;
    vector<int> arr2;
    //divides it into two arrays
    for (int j = 0; j < temp -> size(); ++j) {
        if(j <= midP) {
            arr1.push_back(temp->at(j));
        } else{
            arr2.push_back(temp->at(j));
        }
    }
    //if its size is bigger than 1 then it creates a thread to merge sort each array
    if(temp->size() > 1) {
        pthread_create(&t1, NULL, &mergesortHandler, &arr1);
        nThreads++;
        pthread_join(t1, NULL);
        pthread_create(&t2, NULL, &mergesortHandler, &arr2);
        nThreads++;
        pthread_join(t2, NULL);

    }
    //check if size of array is bigger than one to merge the two arrays
    if(temp->size() > 1) {
        merge(temp, arr1, arr2);
        // cout << "Array after merge " << mergeN;
        mergeN++;
       /* for (int i = 0; i < temp->size(); ++i) {
            //   cout << temp -> at(i) << " ";
        }*/
         //   cout<<endl;
    }
}
/**
 * main thread creates a thread holding the original array
 */
void mergeSort::mainThread() {
    pthread_t tid;
    pthread_create(&tid, NULL, &mergesortHandler, &arrayTmp);
    nThreads++;
    pthread_join(tid, NULL);
    for (int i = 0; i < arrayTmp.size(); i++)
        printf ("%d ", arrayTmp[i]);

    printf ("\n");
}
/**
 * for beginning the sorting of the array
 * the only public function in the class
 * @param filename the name of the input file
 */
void mergeSort::begin(string filename) {
    readFileMerge(filename);
  //  cout << array.size() << endl;
    arrayTmp = array;
    mainThread();
 //   cout  << nThreads<< endl;


}