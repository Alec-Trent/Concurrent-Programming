/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 03/16/2022
* PROGRAM ASSIGNMENT 3
* FILE NAME: thread.h
* PROGRAM PURPOSE:
*	Utilized multithreading to implement the calcThread algorithm specified in the assignment
* DAYS LATE: 0
* LATE DAYS REMAINING: 4
*/

#include "ThreadClass.h"

extern int valArray[500][500];  // sourced this off the internet because I couldnt pass it directly to the constructor
extern char output[10000];

//---------------------------------------------------------------//
//  PROCESS THREAD CLASS: calcThread
//  	each thread applies the calcThread algorithm to calculate
//	the array values 
// PARAMETERS: int i, int j
// FUNCTIONS CALLED: ThreadFunc(), calcThread()
//---------------------------------------------------------------//
class calcThread : public Thread{
	public:
		calcThread(int i, int j);	// constructor
	
	private:
		void ThreadFunc();	// thread body
		int i;			// rows
		int j;			// cols
		//int **a;		// array ptr, idk if i need this, not using it for now
};

