/* 
 * ajtrent
 * Days Late: 0
 * Late Days Remaining: 5
 *
 * COMPLIATION: gcc -lm -o simplecrack simplecrack.c     //Requires math lib for ceil()
 * MD5SUM: echo -n "string" | md5sum
 * INVOCATION: ./simplecrack md5sum min max numprocesses
*/

/*
 * psuedo code
 *
 * generate md5sum: echo -n "password" | md5sum
 * provide md5sum to simplecrack, along with min max pass lengths and the # of processes to be used
 * divide up keyspace by keylength, range (max-min+1) / numproccesses, evenly distribute R starting at process 1
 * fork #of proccess of pwfind
 * check passwords
 *
 * close out
*/

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	//check for correct number of arguments
	if(argc != 5){
		printf("You supplied the incorrect number of arguments. Given: %d, Expected: 5\n", argc);
		exit(1);
	}

	//checks argument 1's type
	if(isalnum(*argv[1]) == 0){
		printf("You supplied the incorrect type of argument. Argument 1 was not an alphanumeric character.\n");
		exit(1);
	}

	//checks argument 2's type
	if(isdigit(*argv[2]) == 0){
		printf("You supplied the incorrect type of argument. Argument 2 was not a digit, cannot be negative.\n");
		exit(1);
	}

	//checks argument 3's type
	if(isdigit(*argv[3]) == 0){
		printf("You supplied the incorrect type of argument. Argument 3 was not a digit, cannot be negative.\n");
		exit(1);
	}

	//checks argument 4's type
	if(isdigit(*argv[4]) == 0){
		printf("You supplied the incorrect type of argument. Argument 4 was not a digit.\n");
		exit(1);
	}


	//assignments
	double min = atoi(argv[2]);
	double max = atoi(argv[3]); 
	double numprocesses = atoi(argv[4]);
	//printf("arg min: %f\n", min);
	//printf("arg max: %f\n", max);
	//printf("arg numproc: %f\n", numprocesses);

	//check for valid min/max values
	if(min > max){
		printf("You supplied a min value > than the max value. Min: %d, Max: %d\n", (int) min, (int) max);
		exit(1);
	}

	if(min < 1){
		printf("Passwords must be at least 1 character long. You supplied a min value < 1. Min: %d", (int) min);
		exit(1);
	}
	
	int range = max - min;
	if((range + 1) < numprocesses){ //prevents duplicates of processes
		numprocesses = range + 1;
	}

	//divide up keyspace by keylength, range (max-min+1) / numprocesses, evenly distribute R starting at process 1
	double value = (max - (min + 1)) / numprocesses; //find min number of processes
	if(value <= 0){ //corrects tasks min/max if value falls below 0
		value = 1;
	}
	//printf("value: %f\n", value);

	double ceilingV = ceil(value); //take the ceiling, need whole number of processes
	int cValue = ceilingV;
	//printf("cValue: %d\n", cValue);

	int procCounter = numprocesses; //reassignment for loop condition
	int remainder = ((max - min) + 1) - (cValue * numprocesses); //equiv to a mod
	if(remainder < 0){ //accounts for if there are more numprocesses than tasks
		remainder = 0;
	}
	int pMax = 0;
	int wstatus = 0;

	if(procCounter == 1){ //override if only one iteration so that max val is correct
		remainder = 0;
		pMax = max;

		//generate a child process
		if(fork() == 0){
			char aMin[11];
			char aMax[11];
			sprintf(aMin, "%d", (int) min);
			sprintf(aMax, "%d", pMax);
			//printf("Min: %d Max: %d\n", (int) min, pMax);
			//printf("aMin: %c aMax: %c\n", aMin, aMax);
			char * arg[]={"/home/campus13/jmayo/public/cs3331/projects/project1/pwfind", aMin, aMax, argv[1], '\0'				}; //create an argument list for execvp

			if(execvp(arg[0], arg) == -1){ //exec pwfind, check for error
				printf("Fork Fail\n");
				exit(1);
			}
		}
		while(wait(&wstatus) > 0); //wait for all children to exit, then parent
	}else{
		while(procCounter > 0){
			//printf("rem: %d\n\n", remainder);
			//if a remainder exists update range
			if(remainder != 0){
				pMax = (min + cValue + 1) - 1; //subtract 1 for min length
				remainder = remainder - 1;
			} else {
				pMax = (min + cValue) - 1;
			}

			//generate a child process
			if(fork() == 0){
				char aMin[11];
				char aMax[11];
				sprintf(aMin, "%d", (int) min);
				sprintf(aMax, "%d", pMax);
				//printf("Min: %d Max: %d\n", (int) min, pMax);
				//printf("aMin: %c aMax: %c\n", aMin, aMax);
				char * arg[]={"/home/campus13/jmayo/public/cs3331/projects/project1/pwfind", aMin, aMax, argv[					1], '\0'}; //create an argument list for execvp

				if(execvp(arg[0], arg) == -1){ //exec pwfind, check for error
					printf("Fork Fail\n");
					exit(1);
				}
			}

			//update counters
			procCounter = procCounter - 1;
			if(min != max){//in case of input 5 5 it does not become  6 6 7 7 8 8, etc
				min = pMax + 1;
			}
		}
		while(wait(&wstatus) > 0); //wait for all children to exit, then parent
	}
	return 0;
}
