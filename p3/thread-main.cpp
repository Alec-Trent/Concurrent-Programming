/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 03/16/2022
* PROGRAM ASSIGNMENT 3
* FILE NAME: thread-main.cpp
* PROGRAM PURPOSE:
*	This program takes an input array, and uses Thread Mentor to calculate new arrays by utilizing the prefix sum
*		algorithm implemented in the calcThread class.
* DAYS LATE: 0
* LATE DAYS REMAINING: 4
*
* COMPLIATION: make noVisual
* INVOCATION: ./prog3 < input
*/

/*
 * Thread Mentor Syntax
 * Begin()
 * Exit()
 * Delay()
 * ThreadFunc()
 * Join()
*/

/*
 * Psuedocode
 * Get the number of elements from input
 * Get the input array, save into 2d array 
 * Create a thread/setup thread class
 * Begin the threads
 * Join the threads, need previous values 
 * Implement class to handle algorithm
 * Format results
 */

#include <iostream>	// input/output
#include <string>	
#include <math.h>	// log2()
#include "thread.h"	// threadMentor
#include <cstring>	// strlen()

using namespace std;	// control over scope

int valArray[500][500];	// global array so that the threads can access the values, cant pass 2d array 
char output[10000]; 	// store the output to print in the correct order

//---------------------------------------------------------------//
// FUNCTION: main()
// 	takes input, using threads we calculate the prefix sum
// 	and format the output of each threads calculation 
// 	untill we reach the final result
// PARAMETERS:
// FUNCTIONS CALLED: low2(), Begin(), Join(), calcThread()
//---------------------------------------------------------------//
int main(){
	//cout << "Concurrent Prefix Sum Computation" << endl << endl;
	sprintf(output + strlen(output), "Concurrent Prefix Sum Computation\n\n");
	int numOfElems = 0;
	cin >> numOfElems;	// read the first number, number of elements to be expected
	//cout << "Number of input data = " << numOfElems << endl; 	// prints the number of elements
	sprintf(output + strlen(output),"Number of input data = %d\n", numOfElems);
	int runs = log2(numOfElems);		// number of runs
	int count = 0;
	//int valArray[runs + 1][numOfElems];	// rows and columns
	int arrayElem = 0;
	//cout << "Input array: " << endl;
	sprintf(output + strlen(output),"Input array: \n");
	while(count < numOfElems){		// while count is less than the number of elems
		cin >> arrayElem;		// input saved to variable
		valArray[0][count] = arrayElem;	// variable saved to array
		count = count + 1;		// increment count
		//cout << "   " << arrayElem;	// print out element
		sprintf(output + strlen(output), "   %d", arrayElem);
	}

	calcThread * vArr[runs + 1][numOfElems];	// from slides/quicksort example

	for(int i = 1; i <= runs; i = i + 1){
		//cout << endl << endl << "Run " << i << ": " << endl;
		sprintf(output + strlen(output), "\n\nRun %d: \n", i);
		for(int j = 0; j < numOfElems; j = j + 1){

			vArr[i][j] = new calcThread(i, j); 	// creates a thread, array global dont pass
			vArr[i][j] -> Begin();      		// runs a thread
			//vArr[i][j] -> Join();			// join to make threads wait for previous to get value
		}

		for(int j = 0; j < numOfElems; j = j + 1){
			vArr[i][j] -> Join();			// join to make threads wait for previous to get value
		}

		//cout << "Result after run " << i << ": " << endl;
		sprintf(output + strlen(output), "Result after run %d: \n", i);
		count = 0;
		while(count < numOfElems){
			//cout << "   " << valArray[i][count];	// print j values after each i iteration
			sprintf(output + strlen(output), "   %d", valArray[i][count]);
			count = count + 1;			// increment count
		}
	}

	//cout << endl << endl << "Final result after run " << runs << ": " << endl;
	sprintf(output + strlen(output), "\n\nFinal result after run %d: \n", runs);
	count = 0;
	while(count < numOfElems){			// while count is less than the number of elems
		//cout << "   " << valArray[runs][count];	// print out element
		sprintf(output + strlen(output), "   %d", valArray[runs][count]);
		count = count + 1;			// increment count
	}
	//cout << endl;
	sprintf(output + strlen(output), "\n");

	printf("%s", output);
}

