/* 	ELEC 377,  Lab 3
 *
 *  consumer.c 
 */

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>

#include "common.h"

#define FALSE 0
#define TRUE 1

int main (int argc, char *argv[]){
	// initialize the shared memory, load in the initial array's, spawn the worker
	// processes.
	key_t   key;
	struct shared    *memptr;
	int shmid;
    
    if (argc != 2) {
        fprintf(stderr,"Usage: %s processid\n",argv[0]);
        exit(1);
    }
    // no error checking...
    int pid = atoi(argv[1]);
    if (pid < 0 || pid >= NUMPROCS){
        fprintf(stderr,"pid between 0 and %d\n",NUMPROCS-1);
        exit(1);
    }

	/*	 Shared memory init 	*/
	key = ftok(".", 'S');
	if((shmid = shmget(key, MEMSIZE, IPC_CREAT|0666)) == -1 ){
		if( (shmid = shmget(key, MEMSIZE, 0)) == -1){
			printf("Error allocating shared memory. \n");
			exit(1);
		}
	}

	// now map the region..
	if((int)(memptr = (struct shared *) shmat(shmid, 0, 0)) == -1){
	    printf("Couldn't map the memory into our process space.\n");
	    exit(1);
	}
	
	// intialize mutex with memptr struct
	mutexInit(memptr);
	
    while (1) {
		// gets mutex with current pid
		getMutex(pid);
		// checks if mutex should release pid and breaks if true
		if ((memptr->count == 0) && (memptr->numProducers == 0)) {
			releaseMutex(pid);
			break;
		}
		releaseMutex(pid);
		// initialize variables for checking buffer status
		int retrChar = FALSE;
		char currCharacter;
		while (retrChar == FALSE) {
			getMutex(pid);
			// checks count for current objects in buffer and updates currCharacter
			if (memptr->count != 0) {
				currCharacter = memptr->buffer[memptr->out];
				memptr->count--;
				memptr->out = (memptr->out + 1) % BUFFSIZE;
				retrChar = TRUE;
			}
			// check if theres no  producers
			if (memptr->numProducers == 0) {
				releaseMutex(pid);
				break;
			}
			releaseMutex(pid);
		}
		putchar(currCharacter);
	}
	return 0;
}


