/*
 *  common.h - Common definiton of the shared
 *     memory segment and prototypes for semaphore functions
 * 
 *  Created by Thomas Dean
 *  Copyright 2005 Queen's University.
 *
 */

#define MEMSIZE 200
#define BUFFSIZE 5
#define NUMPROCS 5

struct shared {
    // initialize global variables
    int waiting[NUMPROCS];
    int lock;
    int numProducers;
    char buffer[BUFFSIZE];
    int in;
    int out;
    int count;
};

// initialize header definitions
void mutexInit(struct shared *memptr);
void getMutex(short pid);
void releaseMutex(short pid);
