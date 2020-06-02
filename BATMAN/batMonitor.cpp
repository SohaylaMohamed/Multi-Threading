//
// Created by sohayla on 18/11/18.
//

#include "batMonitor.h"
/**
 * initializing the mutex and the condition variables
 */

void batMonitor::construct()  {
    pthread_cond_init(&nFirst, NULL); // for east to wait on north
    pthread_cond_init(&sFirst, NULL); // for west to wait on south
    pthread_cond_init(&eFirst, NULL); // for south to wait on east
    pthread_cond_init(&wFirst, NULL); // for north to wait on west

    pthread_cond_init(&nQueue, NULL); //for north queue
    pthread_cond_init(&sQueue, NULL); // for south queue
    pthread_cond_init(&wQueue, NULL); //for west queue
    pthread_cond_init(&eQueue, NULL);// for east queue

    pthread_cond_init(&crossCV, NULL); // for crossing

    pthread_mutex_init(&monitorLock, NULL); //locks the monitor
}
/**
 * constructor for the monitor
 * inetializes all variables needed
 */

batMonitor::batMonitor() {
    sLine =0; // number of bats in south
    nLine = 0; // number of bats in north
    wLine =0; // number of bats in west
    eLine = 0; //number of bats in east
    //for waiting on right first
    sNe = 0; //south waits on east
    nNw = 0; //north waits on west
    wNs = 0; // west waits on south
    eNn = 0; // east waits on north
    //for waiting on queue
    south = 0;
    north = 0;
    east  = 0;
    west = 0;
    //for waiting on crossing
    crossing = 0;
    waitCross = 0;
    //for deadlock check
    onTrack = 0;
    construct();
}
/**
 * arriving at the crossing
 * @param d  attributes of the bat (id, direction)
 */
void batMonitor::arrive(void* d) {
    pthread_mutex_lock(&monitorLock); //lock monitor
    threadID* bat = (threadID*) d; //attributes of the bat
    printf("BAT %d from %c arrives at crossing\n", bat->id, bat->dir); // print that bat has arrived
    switch (bat->dir) { // check the direction
        case 'n': {
            nLine++; // a new north has arrived
    //        cout<< "north is waiting" << endl; //was for debugging
            if(nLine > 1) { //if there's more than one bat at north
                north++; // number of bats waiting on north queue
                pthread_cond_wait(&nQueue, &monitorLock); // wait and release the lock
                north--;
            }
            break;
        }
        case 's': { // same as north
            sLine++;
    //        cout<< "south is waiting" << endl;
            if(sLine > 1) {
                south++;
                pthread_cond_wait(&sQueue, &monitorLock);
                south--;
            }
            break;
        }
        case 'w': { // same as north
            wLine++;
  //          cout<< "west is waiting" << endl;
            if(wLine > 1) {
                west++;
                pthread_cond_wait(&wQueue, &monitorLock);
                west--;
            }
            break;
        }
        case 'e': { // same as north
            eLine++;
    //        cout<< "east is waiting" << endl;
            if(eLine > 1) {
                east++;
                pthread_cond_wait(&eQueue, &monitorLock);
                east--;
            }
            break;
        }
        default: cout<< "Invalid input!" ; //not n,w,e or s
        exit(EXIT_FAILURE);
    }

    pthread_mutex_unlock(&monitorLock); //release the lock
  //  sleep(.005); // was to produce a deadlock
    cross(bat); // start crossing
}
/**
 * starts to cross
 * waits on right and on crossing
 * @param d  attributes of BAT
 */
void batMonitor::cross(void *d) {
    pthread_mutex_lock(&monitorLock); // lock monitor
    threadID* bat = (threadID*) d;
    switch (bat->dir) {
        case 'n': {
            if(wLine != 0) { //check if any bats on it's right
             //   printf("   north %d is waiting on west.\n", bat->id);
         //         cout << "north is waiting on west" << endl;
                  nNw++; //number of north waiting on west
                  pthread_cond_wait(&wFirst, &monitorLock); // wait on south
                  nNw--;
               // printf("   north %d is not waiting on west.\n", bat->id);

                //      cout << "north is not waiting on west" << endl;
            }
            break;
        }
        case 's': {
            if(eLine != 0) { // same as north
                sNe++;
              //  printf("   south %d is waiting on east.\n", bat->id);

                //   cout << "south is waiting on east" << endl;
                pthread_cond_wait(&eFirst, &monitorLock);
              //  printf("   south %d is not waiting on east.\n", bat->id);

                sNe--;
            //    cout << "south is not waiting on east" << endl;
            }
            break;
        }
        case 'w': { //same as north
            if(sLine != 0) {
               // printf("   west %d is waiting on south.\n", bat->id);

                wNs++;
            //    cout << "west is waiting on south" << endl;
                pthread_cond_wait(&sFirst, &monitorLock);
                wNs--;
          //      printf("   west %d is not waiting on south.\n", bat->id);

                //     cout << "west is not waiting on south" << endl;
            }
            break;
        }
        case 'e': { // same as north
            if(nLine != 0) {
                eNn++;
           //     printf("   east %d is waiting on north.\n", bat->id);

                //    cout << "east is waiting on north" << endl;
                pthread_cond_wait(&nFirst, &monitorLock);
                eNn--;
            //    printf("   east %d is not waiting on north.\n", bat->id);

                //    cout << "east is not waiting on north" << endl;
            }
            break;
        }
        default: cout<< "Invalid input!" ;
            exit(EXIT_FAILURE);
    }
    onTrack++; // a bat is crossing

    while (crossing != 0) { // if no bat is sleeping while crossing
        waitCross++;
        pthread_cond_wait(&crossCV, &monitorLock);// waits on crossing
        waitCross--;
    }

    printf("BAT %d from %c is crossing\n", bat->id, bat->dir); // starts crossing
    pthread_mutex_unlock(&monitorLock); // unlock monitor as it'll enter a second sleep while crossing

    crossing ++; // to indicate a BAT is currently crossing
    sleep(1); // cross in one second
    if(waitCross > 0)
      pthread_cond_signal(&crossCV);
    crossing--;
    leave(bat);

}
/**
 * leaves the crossing and signals all the waiting bats accordingly
 * @param d  attributes of bat
 */
void batMonitor::leave(void *d) {
    pthread_mutex_lock(&monitorLock);
    threadID* bat = (threadID*) d;
    printf("BAT %d from %c is leaving crossing\n", bat->id, bat->dir);
    switch (bat->dir) {
        case 'n': {
          if(eNn > 0 ) //checks if any east is waiting on north
             pthread_cond_signal(&nFirst); // signal left
          if(north > 0) // check if any north is waiting on queue
            pthread_cond_signal(&nQueue); //signal north
          nLine--; //a north has left the crossing

            break;
        }
        case 's': { //same as north
           if(wNs > 0)
            pthread_cond_signal(&sFirst);
           if(south > 0)
            pthread_cond_signal(&sQueue);

           sLine--;
            break;
        }
        case 'w': { //same as north
         if(nNw > 0)
            pthread_cond_signal(&wFirst);
         if(west > 0)
            pthread_cond_signal(&wQueue);

         wLine--;
            break;
        }
        case 'e': { //same as north
            if(sNe > 0)
             pthread_cond_signal(&eFirst);
            if(east > 0)
                pthread_cond_signal(&eQueue);

            eLine--;
            break;
        }
        default: cout<< "Invalid input!" ;
            exit(EXIT_FAILURE);
    }
    onTrack--; //a BAT is not crossing
    pthread_mutex_unlock(&monitorLock);

}
/**
 * check for deadlocks
 */
void batMonitor::check() {
   // pthread_mutex_lock(&monitorLock);
    if(nNw > 0 && wNs > 0 && eNn > 0 && sNe >0) { //if all are waiting on right
        if(onTrack == 0) { // no BAT currently on track crossing
            int num = rand() % 4; // choose a random number
            cout << "deadlock occured" << endl;
            switch (num){  // allow one of the BATs to cross
                case 0:  pthread_cond_signal(&wFirst); break; //from north
                case 1:  pthread_cond_signal(&sFirst); break; //from west
                case 2:  pthread_cond_signal(&eFirst); break; //from south
                case 3 : pthread_cond_signal(&nFirst); break; //from east
                default: check();
            }
        }
    }
   // pthread_mutex_unlock(&monitorLock);

}
/**
 * destroy the mutex and all condition variables
 */
void batMonitor::destruct() {
    pthread_cond_destroy(&nFirst);
    pthread_cond_destroy(&sFirst);
    pthread_cond_destroy(&eFirst);
    pthread_cond_destroy(&wFirst);

    pthread_cond_destroy(&nQueue);
    pthread_cond_destroy(&sQueue);
    pthread_cond_destroy(&eQueue);
    pthread_cond_destroy(&wQueue);

    pthread_cond_destroy(&crossCV);

    pthread_mutex_destroy(&monitorLock);


}
