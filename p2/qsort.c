/*
 * NAME: Alec Trent                             USER ID: ajtrent
 * DUE DATE: 02/16/2022
 * PROGRAM ASSIGNMENT 2
 * FILE NAME: qsort.c
 * PROGRAM PURPOSE:
 * 	To use qsort on the shared memory, recursion happens through creating child processes
 * DAYS LATE: 1
 * LATE DAYS REMAINING: 4
 *
 * COMPLIATION: gcc -o qsort qsort.c
 * INVOCATION: ./qsort LeftIndicie RightIndice shmID
*/ 

/*
 * Psuedo Code
 *
 * receives left and right from cmd line args
 * partitions array section a[Left..Right] into two at a[M]
 * create two child processes of qsort.c with execvp()
 * 	the first gets Left and M-1, second gets M+1 and Right
 * 	parent waits on both children to finish 
 *
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
#include <errno.h>


int main(int argc, char *argv[]){

	// func declaration
	int partition(int *, int, int);
	
	// init vars
	int * qShmPTR;
	int wStatus = 0;

	// parse command line args
	int left = atoi(argv[1]);
	int right = atoi(argv[2]);
	int qShmID = atoi(argv[3]);
	
	//get PID for print
	int pid = getpid();

	// attatch memory here
	qShmPTR = shmat(qShmID, NULL, 0);
        if(qShmPTR == (int *)-1){       // error handling (char *)
                printf("Error: the qsort shared memory attatch failed\n");
                exit(1);
        }
        printf("   ### Q-PROC(%d): Qsort shared memory attatched and is ready to use\n\n", pid);
	
	printf("   ### Q-PROC(%d): entering with a[%d..%d]\n", pid, left, right);
	// print array
	int size = (right - left) + 1;
	printf("        ");
	for(int j = 0; j < size; j++){
		printf("%d ", qShmPTR[j]);
	}
	printf("\n\n");


	int temp = 0;
	int low = left; // to keep orig values
	int high = right;
	int M = qShmPTR[left]; // starting at left end
	printf("   ### Q-PROC(%d): pivot element is a[%d]\n", pid, M);

	//printf("low: %d, high %d, val %d, val2 %d, M %d\n", low, high, qShmPTR[low], qShmPTR[high], M);
	while(low <= high){
		// check values to mid point
		while(qShmPTR[low] < M){
			low = low + 1;
		}

		// check values to mid point
		while(qShmPTR[high] > M){
			high = high - 1;
		}

		// swap values
		if(low <= high){
			//printf("val %d, val2 %d, temp %d\n", qShmPTR[low], qShmPTR[high], temp);
			temp = qShmPTR[low];
			qShmPTR[low] = qShmPTR[high];
			qShmPTR[high] = temp;	
			low = low + 1;
			high = high - 1;
			//printf("val %d, val2 %d, temp %d\n", qShmPTR[low], qShmPTR[high], temp);
		}
	}

	// create a child process
	if(left < high){
        	if(fork() == 0){	
               	 	// handle arguments
               		int Right = M - 1;
			char l[11];
			char r[11];
			char id[11];
			sprintf(l, "%d", left);
			sprintf(r, "%d", high); //Right
			sprintf(id, "%d", qShmID);

			//get PID for print
			int pid = getpid();
			printf("   ### Q-PROC(%d): entering with a[%d..%d]\n", pid, left, Right);

			// print array
			int size = (Right - left) + 1;
			printf("        ");
			for(int j = 0; j < size; j++){
				printf("%d ", qShmPTR[j]);
			}
			printf("\n\n");

                	// arg list
                	char * arg[] = {"/home/campus19/ajtrent/cs3331/p2/qsort", l, r, id, '\0'};
		
                	// exec the child
                	if(execvp(arg[0], arg) == -1){ //exec qsort, check for error
                		perror ("The following error occurred");
				printf( "Value of errno: %d\n", errno );
				printf("Fork Fail - qsort\n");
                 	       	exit(1);
               	 	}
			printf("   ### Q-PROC(%d): exits\n", pid);
		}
	}

	// create a child process
	if(low < right){
        	if(fork() == 0){
                	// handle arguments
                	int Left = M + 1;
			char l[11];
			char r[11];
			char id[11];
			sprintf(l, "%d", low); // Left
			sprintf(r, "%d", right);
			sprintf(id, "%d", qShmID);

			//get PID for print
			int pid = getpid();
			printf("   ### Q-PROC(%d): entering with a[%d..%d]\n", pid, Left, right);

			// print array
			int size = (right - Left) + 1;
			printf("        ");
			for(int j = 0; j < size; j++){
				printf("%d ", qShmPTR[j]);
			}
			printf("\n\n");

                	// arg list
                	char * arg[] = {"/home/campus19/ajtrent/cs3331/p2/qsort", l , r, id, '\0'};
		
                	// exec the child
                	if(execvp(arg[0], arg) == -1){ //exec qsort, check for error
				perror ("The following error occurred");
				printf( "Value of errno: %d\n", errno );
                		printf("Fork Fail - qsort\n");
                        	exit(1);
                	}
			printf("   ### Q-PROC(%d): exits\n", pid);
		}
	}

	// wait for both children to finish
	for(int q = 0; q < 2; q++) wait(&wStatus);

	shmdt(qShmPTR); // detatch
        printf("   ### Q-PROC(%d): Qsort shared memory successfully detatched\n");

	// terminate
	return 0;
}

/*
// based on Lomutos Partition Scheme
int partition(int * qShmPTR, int low, int high){
	int pivot = qShmPTR[high];
	int i = (low - 1);
	int temp = 0;
 
	for (int j = low; j <= high - 1; j = j + 1){

		if (qShmPTR[j] <= pivot){
        		i = i + 1;
			// swap the elements
			temp = qShmPTR[i];
			qShmPTR[i] = qShmPTR[j];
			qShmPTR[j] = temp;
        	}
    	}
	
	// swap the elements
	temp = qShmPTR[i + 1];
	qShmPTR[i + 1] = qShmPTR[high];
	qShmPTR[high] = temp;

	return (i + 1);
}
*/

/*
void quickSort(int * qShmPTR, int low, int high){
	if (low < high){
		int pi = partition(qShmPTR, low, high);
      		printf("piv %d\n", pi);
		printf("val %d\n", qShmPTR[pi]);

		quickSort(qShmPTR, low, pi - 1);
		quickSort(qShmPTR, pi + 1, high);
	}
}
*/
/*

void quickSort(qShmPTR, low, high, pivot){
	int temp = 0;
	int l = low; // storing orig values
	int h = high;

	while(low <= high){
		// check values to mid point
		while(qShmPTR[low] < pivot){
			low = low + 1;
		}

		// check values to mid point
		while(qShmPTR[high] > pivot){
			high = high - 1;
		}

		// swap values , what if unequal swaps???
		if(low <= high){
			temp = qShmPTR[low];
			qShmPTR[low] = qShmPTR[high];
			qShmPTR[high] = temp;	
		}
	}*/
	/*// recursively call function to sort arrays
	if(l < high){
		quickSort(qShmPTR, l, high, pivot);
	}
	if(h > low){
		quickSort(qShmPTR, low, h, pivot);
	}
}*/

/*
// partition
int partition(qShmPTR, left, right){
	int pivot = qShmPTR[left];
	int index = left;
	int temp = 0;
	
	for(int i = left; i <= right - 1; i = i + 1;){
		if(qShmPTR[i] > pivot){ //elem > pivot
			index = index + 1;
			temp = qShmPTR[index];
			qShmPTR[index] = qShmPTR[i];
			qShmPTR[i] = temp;
		}
	}
	temp = qShmPTR[index + 1];
	qShmPTR[index] = qShmPTR[right];
	qShmPTR[right] = temp;

	return index + 1;	
}
*/


	// partition
	//int Mi = partition(qShmPTR, left, right);

	/*int hoarePartition(qShmPTR[left, right]){
		int pivot = qShmPTR[left];
		int index = left;
		int temp = 0;
		
		for(int i = left; i <= right - 1; i = i + 1;){
			if(qShmPTR[i] > pivot){ //elem > pivot
				index = index + 1;
				temp = qShmPTR[index];
				qShmPTR[index] = qShmPTR[i];
				qShmPTR[i] = temp;
			}
		}
		temp = qShmPTR[index + 1];
		qShmPTR[index] = qShmPTR[right];
		qShmPTR[right] = temp;

		return index + 1;
	}*/


	//int i = low;
	//int j = high;
/*
	int pivot = qShmPTR[left];
    	int i = left - 1;
	int j = right + 1;
	int M = 0;
 	int temp = 0;

    	while (1) {
        	// Find leftmost element greater than
        	// or equal to pivot
        	do {
            		i++;
        	} while (qShmPTR[i] < pivot);
 
        	// Find rightmost element smaller than
        	// or equal to pivot
        	do {
         	   j--;
        	} while (qShmPTR[j] > pivot);
 
        	// If two pointers met.
        	if (i >= j){
          	  M = j;
		}
 
        	temp = qShmPTR[i];
		qShmPTR[i] = qShmPTR[j];
		qShmPTR[j] = temp;	
    	}
	 printf("   ### Q-PROC(%d): pivot element is a[%d]\n", pid, M);	

*/



	// terminate
	/*if(left >= right){
		shmdt(qShmPTR); // detatch
        	printf("   ### Q-PROC(%d): Qsort shared memory successfully detatched\n");
		printf("   ### Q-PROC(%d): exits\n", pid);
		exit(0);	
	}*/
	
	// find the center of the array
	//int M = partition(qShmPTR, left, right);		
	//printf("   ### Q-PROC(%d): pivot element is a[%d]\n", pid, M);
/*
	int low = left;
	int high = right;
	int pivot = qShmPTR[high];
	int i = (low - 1);
	int temp = 0;
	int p = 0;
 
	for (p = low; p <= high - 1; p = p + 1){

		if (qShmPTR[p] <= pivot){
        		i = i + 1;
			// swap the elements
			temp = qShmPTR[i];
			qShmPTR[i] = qShmPTR[p];
			qShmPTR[p] = temp;
        	}
    	}
	
	// swap the elements
	temp = qShmPTR[i + 1];
	qShmPTR[i + 1] = qShmPTR[high];
	qShmPTR[high] = temp;

	int M = (i + 1);
	printf("   ### Q-PROC(%d): pivot element is a[%d]\n", pid, M);

*/

