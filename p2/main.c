/*
 * NAME: Alec Trent				USER ID: ajtrent
 * DUE DATE: 02/16/2022
 * PROGRAM ASSIGNMENT 2
 * FILE NAME: main.c
 * PROGRAM PURPOSE:
 * 	Create shared memory segments, read input, create two child processes to sort the input.
 * DAYS LATE: 1
 * LATE DAYS REMAINING: 4
 *
 * COMPLIATION: gcc -o main main.c
 * INVOCATION: ./main < input
*/ 


/*
 * Psuedo Code
 *
 * Generate shared mem segments
 * Reads three input arrays a[], x[], y[], be k, m, and n
 * print out the arrays							// finish this step, parsing is bad
 * create child to run qsort.c using execvp()
 * 	pass in Left and Right arguments, plus anything extra
 *	initially Left = 0 and Right = k-1
 * create a second child to run merge.c using execvp()
 * 	pass needed arguments
 * wait for both children to finish, print results
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

extern int errno ;

int main(int argc, char *argv[]) {

	// Init Variables
	int k, m, n;		// number of elements in the input arrays
	int wStatus = 0;	// used to determine wait status
	int g = 0;

	// qsort shared mem vars
	key_t qKey;
	int qShmID;
	int * qShmPTR;
	int qSize = 0;

	// merge sort shared mem vars
	key_t mKey;
	int mShmID;
	int * mShmPTR;
	int mSize = 0;

	// merge sort output shared mem vars
	key_t oKey;
	int oShmID;
	int * oShmPTR;

	int hold = 0;

	// read in the k array
	scanf("%d", &k);
	int q[k];
	int count1 = 0;
	while(count1 < k){
		scanf("%d", &hold);
		q[count1] = hold;
		count1 = count1 + 1;
	}
	
	// read in m array
	scanf("%d", &m);
	int m1[m];
	int count2 = 0;
	while(count2 < m){
		scanf("%d", &hold);
		m1[count2] = hold;
		count2 = count2 + 1;
	}

	// read in n array
	scanf("%d", &n);
	int m2[n];
	int count3 = 0;
	while(count3 < n){
		scanf("%d", &hold);
		m2[count3] = hold;
		count3 = count3 + 1;
	}

	// update sizes for shared mem
	qSize = k;
	mSize = m + n;

	// qsort shared memory segment
	qKey = ftok("./", 'q'); // generate the key
	printf("*** MAIN: Qsort shared memory key = %d \n", qKey);

	qShmID = shmget(qKey, qSize, IPC_CREAT | 0666);	// create shared memory segment	
	if(qShmID == -1){	// error handling
		printf("Error: the qsort shared memory creation failed\n");
		exit(1);
	}
	printf("*** MAIN: Qsort shared memory created\n");

	qShmPTR = shmat(qShmID, NULL, 0);
	if(qShmPTR == (int *)-1){	// error handling (char *)
		printf("Error: the qsort shared memory attatch failed\n");
		exit(1);
	}
	printf("*** MAIN: Qsort shared memory attatched and is ready to use\n\n");


	// merge shared memory segment
	mKey = ftok("./", 'm');   // generate the key
	printf("*** MAIN: Merge shared memory key = %d \n", mKey);

	mShmID = shmget(mKey, mSize, IPC_CREAT | 0666);	// create shared memory segment	
	if(mShmID == -1){	// error handling
		printf("Error: the merge shared memory creation failed\n");	// SPORADICALLY FAILING****
		exit(1);
	}
	printf("*** MAIN: Merge shared memory created\n");

	mShmPTR = shmat(mShmID, NULL, 0);
	if(mShmPTR == (int *)-1){	// error handling (char *)
		printf("Error: the merge shared memory attatch failed\n");
		exit(1);
	}
	printf("*** MAIN: Merge shared memory attatched and is ready to use\n\n");


	// merge sort output shared memory segment
	oKey = ftok("./", 'o'); // generate the key
	printf("*** MAIN: Merge sort output shared memory key = %d \n", oKey);

	oShmID = shmget(oKey, mSize, IPC_CREAT | 0666);	// create shared memory segment	
	if(oShmID == -1){	// error handling
		printf("Error: the merge sort output shared memory creation failed\n");
		exit(1);
	}
	printf("*** MAIN: Merge sort output shared memory created\n");

	oShmPTR = shmat(oShmID, NULL, 0);
	if(oShmPTR == (int *)-1){	// error handling (char *)
		printf("Error: the merge output shared memory attatch failed\n");
		exit(1);
	}
	printf("*** MAIN: Merge output shared memory attatched and is ready to use\n\n");	

	// set dummy arrays to shared memory
	int counter = 0;
	while(counter < qSize){
		qShmPTR[counter] = q[counter];
		counter = counter + 1;
	}

	int counter1 = 0;
	int co = 0;
	while(counter1 < mSize){
		while(counter1 < m){
			mShmPTR[counter1] = m1[counter1];
			counter1 = counter1 + 1;
		}

		mShmPTR[counter1] = m2[co];
		counter1 = counter1 + 1;
		co = co + 1;
	}


	// print formatting
	printf("Input array a[] for qsort has %d elements: \n", k);
	printf("        ");
	for(int j = 0; j < k; j++){
		printf("%d ", qShmPTR[j]);
	}
	printf("\n\n");

	// print formatting
	printf("Input array x[] for merge has %d elements: \n", m);
	printf("        ");
	for(; g < m; g++){
		printf("%d ", mShmPTR[g]);
	}
	printf("\n\n");

	// print formatting
	printf("Input array y[] for merge has %d elements: \n", n);
	printf("        ");
	for(; g < (n+m); g++){
		printf("%d ", mShmPTR[g]);
	}
	printf("\n\n");


	printf("*** MAIN: Spawning qsort process\n");
	// create a child process, qsort
	if(fork() == 0){
		// handle arguments
		int Left = 0;
		int Right = k - 1;
		char l[11];
		char r[11];
		char id[11];
		sprintf(l, "%d", Left);
		sprintf(r, "%d", Right);
		sprintf(id, "%d", qShmID);

		// arg list
		char * arg[] = {"/home/campus19/ajtrent/cs3331/p2/qsort", l, r, id, '\0'};

		// exec the child
		if(execvp(arg[0], arg) == -1){ //exec qsort, check for error
			perror ("The following error occurred");
    			printf( "Value of errno: %d\n", errno );
			printf("Fork Fail - qsort\n");
			exit(1);
		}
	}


 	printf("*** MAIN: Spawning merge process\n");
	int proc = n + m;
	// create a child process, merge
	if(fork() == 0){
		// handle arguments
		char mm[11];
		char nn[11];
		char p[11];
		char id[11];
		char od[11];
		sprintf(mm, "%d", m);
		sprintf(nn, "%d", n);
		sprintf(p, "%d", proc);
		sprintf(id, "%d", mShmID);
		sprintf(od, "%d", oShmID);

		// arg list
		char * arg[] = {"/home/campus19/ajtrent/cs3331/p2/merge", mm, nn, p, id, od, '\0'};

		// exec the child
		if(execvp(arg[0], arg) == -1){ //exec merge, check for error
			perror ("The following error occurred");
    			printf( "Value of errno: %d\n", errno );
			printf("Fork Fail - merge\n");
			exit(1);
		}
	}

	// wait for both children to finish
	for(int i = 0; i < 2; i++) wait(&wStatus);


	// print out results
	printf("*** MAIN: Array sorted by qsort:\n");
	printf("        ");
	for(int j = 0; j < qSize; j++){
		printf("%d ", qShmPTR[j]);
	}
	printf("\n\n");

	// print out results
	printf("*** MAIN: Merged array:\n");
	printf("        ");
	for(int j = 0; j < mSize; j++){
		printf("%d ", mShmPTR[j]);
	}
	printf("\n\n");

	
	shmdt(qShmPTR);	// detatch
	printf("*** MAIN: Qsort shared memory successfully detatched\n");
	shmctl(qShmID, IPC_RMID, NULL); // removed
	printf("*** MAIN: Qsort shared memory successfully removed\n\n");

	shmdt(mShmPTR);	// detatch
	printf("*** MAIN: Merge shared memory successfully detatched\n");	
	shmctl(mShmID, IPC_RMID, NULL); // removed
	printf("*** MAIN: Merge shared memory successfully removed\n\n");

	shmdt(oShmPTR);	// detatch
	printf("*** MAIN: Merge output shared memory successfully detatched\n");	
	shmctl(oShmID, IPC_RMID, NULL); // removed
	printf("*** MAIN: Merge output shared memory successfully removed\n");

	// terminate
	return 0;
}




/*	// shared memory segment
	key_t key = ftok("./input", 'a');	// generate the key
	printf("Shared memory key = %d \n", key);

	int shmID = shmget(key, size, IPC_CREAT | 0666);	// create shared memory segment UPDATE SIZE****
	if(shmID == -1){	// error handling
		printf("Error: the shared memory creation failed\n");	// SPORADICALLY FAILING****
		exit(1);
	}
	printf("Shared memory created\n");

	void * shmPTR = shmat(shmID, NULL, 0);
	if(shmPTR == (char *)-1){	// error handling
		printf("Error: the shared memory attatch failed\n");
		exit(1);
	}
	printf("Shared memory attatched and is ready to use\n");*/



		/*
		// get length of first array
		if(count == 0){
			k = atoi(string);
			qSize = k;
			//printf("k: %d\n", k);
			count = count + 1;

			// shared memory segment
			qKey = ftok("./input", 'a');	// generate the key
			printf("Shared memory key = %d \n", qKey);

			qShmID = shmget(qKey, qSize, IPC_CREAT | 0666);	// create shared memory segment	
			if(qShmID == -1){	// error handling
				printf("Error: the shared memory creation failed\n");	// SPORADICALLY FAILING****
				exit(1);
			}
			printf("Shared memory created\n");

			qShmPTR = shmat(qShmID, NULL, 0);
			if(qShmPTR == (int *)-1){	// error handling (char *)
				printf("Error: the shared memory attatch failed\n");
				exit(1);
			}
			printf("Shared memory attatched and is ready to use\n");
			count = count + 1;


		// get values of first array
		}else if(count == 1){
			//char * a[k];			// declare array here so i have k value
			//printf("a[]: %s", string);	// check input
			char * tok = strtok(string, " a[]");	// parse string using delimiters
			int i = 0;			// iinit incrementer
			while(tok != NULL){		// run until nothing left to parse	
				//printf("%s", tok);	// display elemenets after parsing
				qShmPTR[i] = atoi(tok);		// add element to an array for later use
				tok = strtok(NULL, " a[]");	// continue to parse string until end
				i = i + 1;		// increment array position
			}
			// print formatting
			printf("Input array a[] for qsort has %d elements: \n", k);
			for(int j = 0; j < k; j++){
				printf("%d ", qShmPTR[j]);
			}
			printf("\n\n");

			count = count + 1; // increment counter

		
		// get length of second array
		}else if(count == 2){
			m = atoi(string);
			//printf("m: %d\n", m);
			count = count + 1;

		// get values of second array
		}else if(count == 3){
			char * x[m];
			//printf("x[]: %s", string);	// check input
			char * tok = strtok(string, " x[]");	// parse string using delimiters
			int i = 0;
			while(tok != NULL){		// run until nothing left to parse	
				//printf("%s", tok);	// display elemenets after parsing
				x[i] = tok;		// add element to an array for later use
				tok = strtok(NULL, " x[]");	// continue to parse string until end
				i = i + 1;		// increment array position
			}
			// print formatting 
			printf("Input array x[] for merge has %d elements: \n", m);
			for(int j = 0; j < m; j++){
				printf("%s ", x[j]);
			}
			printf("\n\n");

			count = count + 1;


		// get length of third array
		}else if(count == 4){
			n = atoi(string);
			//printf("n: %d", n);
			count = count + 1;

		// get values of third array
		}else if(count == 5){
			char * y[n];
			//printf("y[]: %s", string);	// check input
			char * tok = strtok(string, " y[]");	// parse string using delimiters
			int i = 0;
			while(tok != NULL){		// run until nothing left to parse	
				//printf("%s", tok);	// display elemenets after parsing
				y[i] = tok;		// add element to an array for later use
				tok = strtok(NULL, " y[]");	// continue to parse string until end
				i = i + 1;		// increment array position
			}
			// print formatting
			printf("Input array y[] for merge has %d elements: \n", n);
			for(int j = 0; j < n; j++){
				printf("%s ", y[j]);
			}
			printf("\n\n");

			count = count + 1;

			break;

		}else{
			printf("Error: incorrect count\n");
		}
	}*/

/*

	// create a child process
	if(fork() == 0){
		// handle arguments
		int Left = 0;
		int Right = k - 1;

		// arg list
		char * arg[] = {"./qsort.c", Left, Right, '\0'};

		// exec the child
		if(execvp(arg[0], arg) == -1){ //exec qsort, check for error
			printf("Fork Fail - qsort\n");
			exit(1);
		}
	}
	
	// create a child process
	if(fork() == 0){
		// handle arguments

		// arg list
		char * arg[] = {"./merge.c", '\0'};

		// exec the child
		if(execvp(arg[0], arg) == -1){ //exec merge, check for error
			printf("Fork Fail - merge\n");
			exit(1);
		}
	}


	// wait for both children to finish
	for(int i = 0; i < 2; i++) wait(wStatus);
*/




/*
	// shared memory segment
	key_t key = ftok("./input", 'a');	// idk about 'a' as the id
	int shmID = shmget(key, size, "IPC_CREAT" | 0666);	// create shared memory segment
	if(shmID == -1){	// error handling
		printf("Error: the shared memory creation failed");
	}

	// parse input file to get array sizes, and values
	int fd = open(argv[1], O_RDONLY); 	// open the file with the input
	int seekRtn = lseek(fd, 0, SEEK_SET); 	// set ptr to start of file

	while(read(fd, &ch, 1) != 0){	// loop unitl EOF, reading 1 char at a time
		if(atoi(ch) != 0){

		}

		arrayLength = ch;	// find array length to parse each array out of input file
		while(arrayLength > 0){
			read(fd, &ch, 1)
			arrayLength = arrayLength - 1;
		}
	}
*/
	

	// open the file, get fileDesc
	//FILE * fd = fopen("input1", "r");
	//FILE * fd = fopen(file, "r");

/*
	// read through the file to get sizes for shared memory allocation
	while(fgets(string, sizeof(string), fd) != NULL){
		if(count0 == 0){
			k = atoi(string);
			//printf("k: %d\n", k);
			count0 = count0 + 1;
		} else if(count0 == 2){
			m = atoi(string);
			//printf("m: %d\n", m);
			count0 = count0 + 1;
		} else if(count0 == 4){
			n = atoi(string);
			//printf("n: %d\n", n);
			count0 = count0 + 1;
			break;
		} else{
			count0 = count0 + 1;
		}
	}
	//printf("k %d m %d n %d\n", k, m, n);
*/


/*
	// open the file, get fileDesc
	//FILE * fd1 = fopen("input1", "r");
	FILE * fd1 = fopen(file, "r");

	// while input availible, loop getting array data
	while(fgets(string, sizeof(string), fd1) != NULL){

		if(count == 1){
			char * tok = strtok(string, " a[]");	// parse string using delimiters
			int i = 0;			// iinit incrementer
			while(tok != NULL){		// run until nothing left to parse	
				//printf("%s", tok);	// display elemenets after parsing
				qShmPTR[i] = atoi(tok);		// add element to an array for later use
				tok = strtok(NULL, " a[]");	// continue to parse string until end
				i = i + 1;		// increment array position
			}
			// print formatting
			printf("Input array a[] for qsort has %d elements: \n", k);
			printf("        ");
			for(int j = 0; j < k; j++){
				printf("%d ", qShmPTR[j]);
			}
			printf("\n\n");

			count = count + 1; // increment counter
		} else if(count == 3){
			char * tok = strtok(string, " x[]");	// parse string using delimiters
			//int i = 0;
			while(tok != NULL){		// run until nothing left to parse	
				//printf("%s", tok);	// display elemenets after parsing
				mShmPTR[p] = atoi(tok);		// add element to an array for later use
				tok = strtok(NULL, " x[]");	// continue to parse string until end
				p = p + 1;		// increment array position
				//printf("p %d\n", p);
			}
			p = p - 1; // corrected, counted null terminator
			// print formatting 
			printf("Input array x[] for merge has %d elements: \n", m);
			printf("        ");
			for(; g < m; g++){
				printf("%d ", mShmPTR[g]);
			}
			printf("\n\n");

			count = count + 1;
		} else if(count == 3){
			char * tok = strtok(string, " x[]");	// parse string using delimiters
			//int i = 0;
			while(tok != NULL){		// run until nothing left to parse	
				//printf("%s", tok);	// display elemenets after parsing
				mShmPTR[p] = atoi(tok);		// add element to an array for later use
				tok = strtok(NULL, " x[]");	// continue to parse string until end
				p = p + 1;		// increment array position
				//printf("p %d\n", p);
			}
			p = p - 1; // corrected, counted null terminator
			// print formatting 
			printf("Input array x[] for merge has %d elements: \n", m);
			printf("        ");
			for(; g < m; g++){
				printf("%d ", mShmPTR[g]);
			}
			printf("\n\n");

			count = count + 1;

		} else if(count == 5){
			char * tok = strtok(string, " y[]");	// parse string using delimiters
			//int i = 0;
			while(tok != NULL){		// run until nothing left to parse	
				//printf("%s", tok);	// display elemenets after parsing
				mShmPTR[p] = atoi(tok);		// add element to an array for later use
				tok = strtok(NULL, " y[]");	// continue to parse string until end
				p = p + 1;		// increment array position
			}
			// print formatting
			printf("Input array y[] for merge has %d elements: \n", n);
			printf("        ");
			for(; g < (n+m); g++){
				printf("%d ", mShmPTR[g]);
			}
			printf("\n\n");

			count = count + 1;
			break;

		} else{
			count = count + 1;
		}
	}
*/

	// tidy up
	//fclose(fd);
	//fclose(fd1);	// close fd


