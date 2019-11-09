/* 	ELEC 377, Lab 3 
 * Nicholas Alderman, Daniyal Maniar
 * common.c contains routines to be used from both the  producer, and the  consumer
 *   Mutual Exclusion routines will be here
 */

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <asm/system.h>

#include "common.h"

#define FALSE 0
#define TRUE 1

static struct shared *sharedptr;

int test_and_set(int * lock){
    return __cmpxchg(lock,0,1,4);
}

void mutexInit(struct shared *memptr){
        // initialize the only mutex once, from the producer... 
    sharedptr = memptr;
}

void getMutex(short pid){
	// this should not return until it has mutual exclusion. Note that many versions of 
	// this will probobly be running at the same time.
    sharedptr->waiting[pid] = TRUE;
    // initialize test and set key
    int key = TRUE;
    // ensure key has access and pid is in queue
    while (sharedptr->waiting[pid] && key) {
        // set key based on test_and_set
        key = test_and_set(&sharedptr->lock);
    }  
    sharedptr->waiting[pid] = FALSE;
}

void releaseMutex(short pid){
	// set the mutex back to initial state so that somebody else can claim it
    // initialize counter to iterate through processes
    short j = (pid + 1) % NUMPROCS;
    // ensures counter exists in queue
    while ((j!=pid) && (!sharedptr->waiting[j])){ 
	    j=(j+1)%NUMPROCS;
	}
    if (pid == j) {
        sharedptr->lock = FALSE;
    }
    else {
        sharedptr->waiting[j] = FALSE;
    }
}

