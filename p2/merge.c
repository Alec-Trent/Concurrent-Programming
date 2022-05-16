/*
 * NAME: Alec Trent                             USER ID: ajtrent
 * DUE DATE: 02/16/2022
 * PROGRAM ASSIGNMENT 2
 * FILE NAME: merge.c
 * PROJECT PURPOSE:
 * 	To use merge on the shared memory, n+m processes sort the array
 * DAYS LATE: 1
 * LATE DAYS REMAINING: 4
 *
 * COMPLIATION: gcc -o merge merge.c
 * INVOCATION: ./merge
*/ 


/*
 * Psuedo Code
 * gets whatever arguments given
 * create m+n children each assigned to an element of x[] or y[]
 * 	each child does modified binary search to find final location of element in sorted array
 * 	when found write its value into that location of the output array
 * wait until all children exit
 * terminate
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	
	// init vars
	int * mShmPTR;
	int * oShmPTR;
	int count = 0;
	int val = 0;
	int flag = 0;
	int pos = 0;
	int wStatus = 0;
	

	// parse command line args
	int m = atoi(argv[1]);
	int n = atoi(argv[2]);
	int proc = atoi(argv[3]);
	int mShmID = atoi(argv[4]);
	int oShmID = atoi(argv[5]);

	int pid = getpid();

	// attatch memory here
        mShmPTR = shmat(mShmID, NULL, 0);
        if(mShmPTR == (int *)-1){       // error handling (char *)
        	printf("Error: the merge sort shared memory attatch failed\n");
                exit(1);
        }
        printf("      $$$ M-PROC(%d): Merge sort shared memory attatched and is ready to use\n", pid);

	// attatch memory here
        oShmPTR = shmat(oShmID, NULL, 0);
        if(oShmPTR == (int *)-1){       // error handling (char *)
        	printf("Error: the merge sort output shared memory attatch failed\n");
                exit(1);
        }
        printf("      $$$ M-PROC(%d): Merge sort output shared memory attatched and is ready to use\n", pid);

/*
	// create m+n child processes
	while(count < proc){
		if(fork() == 0){
			int pid = getpid();
			// each assigned to an element of x[] or y[]
			val = mShmPTR[count];
			printf("count: %d, m %d\n", count, m);
			if(count < m){
				printf("      $$$ M-PROC(%d): handling x[%d] = %d\n", pid, count, val);
				flag = 0;
			} else {
				printf("      $$$ M-PROC(%d): handling y[%d] = %d\n", pid, count, val);
				flag = 1;
			}
		
			// perform a modified binary search to determine final location in sorted array
			printf("flag: %d\n", flag);
			if (flag == 0) { // x
				printf("count: %d\n", count);
  				//printf("x \n");
        			if (val < mShmPTR[m]){ // eqiv of y[0]
					printf("      $$$ M-PROC(%d): x[%d] = %d is found to be smaller than y[%d] = %d\n", pid, count, val, 0, mShmPTR[m]);
					printf("      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", pid, count, val, pos);
					oShmPTR[count] = val;
  				}
	
        			if (val > mShmPTR[proc - 1]){ // equiv of y[n-1]
					printf("      $$$ M-PROC(%d): x[%d] = %d is found to be greater than y[%d] = %d\n", pid, count, val, proc - 1, mShmPTR[proc - 1]);

					printf("      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", pid, count, val, pos);
					oShmPTR[count] = val;
				}
				
				int counter = 0;
				while(counter < proc){
					if(val > mShmPTR[m] && val < mShmPTR[proc - 1]){
						printf("      $$$ M-PROC(%d): x[%d] = %d is found between y[%d] = %d and y[%d] = %d\n", pid, count, val, counter - 1, mShmPTR[counter - 1], counter, mShmPTR[counter]);

						printf("      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", pid, count, val, pos);
						oShmPTR[count] = val;
						counter = counter + 1;
					}
				}


    			} else { // y
				if (val < mShmPTR[m]){ // eqiv of y[0]
					printf("      $$$ M-PROC(%d): y[%d] = %d is found to be smaller than x[%d] = %d\n", pid, count - m, val, 0, mShmPTR[0]);

					printf("      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n", pid, count, val, pos);
					oShmPTR[count] = val;
  				}
	
        			if (val > mShmPTR[proc - 1]){ // equiv of y[n-1]
					printf("      $$$ M-PROC(%d): y[%d] = %d is found to be greater than x[%d] = %d\n", pid, count - m, val, m - 1, mShmPTR[m - 1]);

					printf("      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n", pid, count, val, pos);
					oShmPTR[count] = val;
				}

				int counter = 0;
				while(counter < proc){
					if(val > mShmPTR[0] && val < mShmPTR[m]){
						printf("      $$$ M-PROC(%d): y[%d] = %d is found between x[%d] = %d and x[%d] = %d\n", pid, count - m, val, counter - 1, mShmPTR[counter - 1], counter, mShmPTR[counter]);

						printf("      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", pid, count, val, pos);
						oShmPTR[count] = val;
						counter = counter + 1;
					}
				}
			}
		}
		count = count + 1;
	}

	// wait for m+n children to finish
	for(int i = 0; i < proc; i++) wait(&wStatus);	
*/
	shmdt(mShmPTR); // detatch
        printf("      $$$ M-PROC(%d): Merge sort shared memory successfully detatched\n", pid);

	shmdt(oShmPTR); // detatch
        printf("      $$$ M-PROC(%d): Merge sort output shared memory successfully detatched\n", pid);

	// terminate
	return 0;

}
