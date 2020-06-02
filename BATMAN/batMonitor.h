//
// Created by sohayla on 18/11/18.
//

#ifndef UNTITLED_BATMONITOR_H
#define UNTITLED_BATMONITOR_H

#include <iostream>
#include <condition_variable>
#include <unistd.h>

using namespace std;
class batMonitor {
private:
    struct threadID {
        char dir;
        int id;
    };
    pthread_cond_t nQueue, sQueue, eQueue, wQueue;
    pthread_cond_t nFirst, sFirst, wFirst, eFirst;
    pthread_cond_t crossCV;
    pthread_mutex_t monitorLock;
    int onTrack;
    int north, east, west, south;
    int sNe, nNw, eNn, wNs;
    int crossing, waitCross;
    int nLine, wLine, sLine, eLine;
   void construct();
    void cross(void* d);
    void leave(void* d);
public:
    void destruct();
    void arrive(void* d);
    batMonitor();
    void check();

};




#endif //UNTITLED_BATMONITOR_H
