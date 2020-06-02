#include "batMonitor.h"

struct threadID { //structure to hold the attributes of bat
    char dir;
    int id;
};

void joinThreads(int bats, pthread_t pthread[]);

batMonitor monitor;
/**
 * thread function calls for arrive in monitor
 * @param args  attributes of BAT
 * @return
 */
void* start(void* args) {
    threadID* identification = (threadID*) args;
    monitor.arrive(identification);

}
/**
 * periodically check for deadlocks
 * periods of 1.5 seconds
 * @param bound bound of number of checks which is propotional to number of BATs
 */
void checkForDeadlocks(int bound) {
    for (int k = 0; k < bound ; ++k) {
        monitor.check();
        sleep(1.5);
    }
}
/**
 * create a thread for each BAT
 * @param numOfBats number of entered bats
 * @param input  the input
 */
void createThreads(int numOfBats, string input) {
    int err;
    pthread_t threads[numOfBats];
    threadID ids[numOfBats];
    for (int i = 0; i <numOfBats ; ++i) {
        ids[i] = {static_cast<char>(tolower(input.at(i))), i + 1};
        err = pthread_create(&threads[i], NULL, start, &ids[i]);
        if (err != 0) {
            cout << "Thread can't be created";
            exit(EXIT_FAILURE);
        }
    }
    checkForDeadlocks(numOfBats); // check for deadlocks
    joinThreads(numOfBats, threads); // join threads
}
/**
 * join all the threads
 * @param bats  number of bats
 * @param threads threads to be joined
 */
void joinThreads(int bats, pthread_t threads[]) {
    for (int j = 0; j < bats ; ++j) {
        pthread_join(threads[j], NULL);
    }
}

int main() {
    //read input
    string v;
    cin >> v;
    createThreads(v.length(), v);
    //destory cv and mutex
    monitor.destruct();
    return 0;
}