/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 03/16/2022
* PROGRAM ASSIGNMENT 3
* FILE NAME: thread.cpp
* PROGRAM PURPOSE:
*	This is where the class calcThread is implemented
* DAYS LATE: 0
* LATE DAYS REMAINING: 4
*/

#include <math.h>       // pow()
#include "thread.h"     // threadMentor
#include <cstring>	// strlen()

//---------------------------------------------------------------//
// CLASS: calcThread
// 	class constructor
// PARAMETERS: int i, int j
// FUNCTIONS CALLED:
//---------------------------------------------------------------//
calcThread::calcThread(int i, int j) : i(i), j(j){

	// might need to set seek pos like quicksort example
	//cout << "     Thread " << j << " Created" << endl;
	sprintf(output + strlen(output), "     Thread %d Created\n", j);
}

//---------------------------------------------------------------//
// CLASS BODY: calcThread
// 	A comparison made determines the action of copying the
// 	previous rows value or calculating the new array vals
// PARAMETERS:
// FUNCTIONS CALLED: ThreadFunc(), pow(), Exit()
//---------------------------------------------------------------//
void calcThread::ThreadFunc(){	// following example from slides/quicksort example
	Thread::ThreadFunc();   // following example from slides/quicksort example
        //cout << "Thread " << j << " Created" << endl;
	//cout << (j-((int)(pow(2,i-1)))) << endl;

        if((j-((int)(pow(2,i-1)))) < 0){
		//cout << "     Thread " << j << " copies valArray[" << j << "]" << endl;
		sprintf(output + strlen(output), "     Thread %d copies valArray[%d]\n", j, j);
		//cout << "     Thread " << j << " copies " << valArray[i-1][j] << endl;
		//sprintf(output + strlen(output), "     Thread %d copies %d \n", j, valArray[i-1][j]);
		valArray[i][j] = valArray[i-1][j];
        } else {
                //cout << "     Thread " << j << " computes valArray[" << i-1 << "][" << j << "] + valArray[" << i-1 << "][" << 
		//	(j-((int)(pow(2,i-1)))) << "] " << endl;
		 sprintf(output + strlen(output), "     Thread %d computes valArray[%d][%d] + valArray[%d][%d]\n", j, i-1, j, i-1, (j-((int)(pow(2,i-1)))));

		//cout << "     Thread " << j << " computes " << valArray[i-1][j] << " + " << valArray[i-1][(j-((int)(pow(2,i-1))))] << endl;

 		valArray[i][j] = valArray[i-1][j] + valArray[i-1][(j-((int)(pow(2,i-1))))];
        }

        //cout << "     Thread " << j << " Exits" << endl;
	sprintf(output + strlen(output), "     Thread %d Exits\n", j);
        Exit(); // terminates the thread
}

