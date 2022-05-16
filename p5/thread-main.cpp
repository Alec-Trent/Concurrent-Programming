/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 04/15/2022
* PROGRAM ASSIGNMENT 5
* FILE NAME: thread-main.cpp
* PROGRAM PURPOSE:
*	This is where:
*		we accept input.
*		1 boat thread, c and m cannibal and missionary
*		threads  are created and initialized.
*		also initialize our monitor here 
* DAYS LATE: 1
* LATE DAYS REMAINING: 2
*/

#include "ThreadClass.h"
#include <cstring>
#include "thread.h"     // threadMentor

using namespace std;    // control over scope

//---------------------------------------------------------------//
// Function: main()
//      accept input, initialize, and setup threads
// PARAMETERS: 
// FUNCTIONS CALLED: strlen(), Begin(), Join(), atoi(), sprintf(), write()
//---------------------------------------------------------------//
int main(int argc, char *argv[]){

	// take input from user args, if 0 use defaults
	int numOfCannibals = atoi(argv[1]);

	int numOfMissionaries = atoi(argv[2]);

	int numOfBoatLoads = atoi(argv[3]);

	if(numOfCannibals == 0){
		numOfCannibals = 8;
	}
	if(numOfMissionaries == 0){
		numOfMissionaries = 8;
	}
	if(numOfBoatLoads == 0){
		numOfBoatLoads = 5;
	}

	char * name = "name";
	// initialize monitor
	boatMonitor * boatMon; 
	boatMon = new boatMonitor(name);

	// initialize threads
	cannibalThread * cannibal[numOfCannibals];
	missionaryThread * missionary[numOfMissionaries];
	boatThread * boat;

	boat = new boatThread(numOfBoatLoads, boatMon);			// create thread
	boat -> Begin();						// run thread

	for(int i = 1; i <= numOfCannibals; i = i + 1){
		cannibal[i] = new cannibalThread(i, boatMon);		// create thread
		cannibal[i] -> Begin();					// run thread
	}

	for(int j = 1; j <= numOfMissionaries; j = j + 1){
		missionary[j] = new missionaryThread(j, boatMon);	// create thread
		missionary[j] -> Begin();				// run thread
	}

	// join threads
	boat -> Join();

	for(int x = 1; x <= numOfCannibals; x = x + 1){
		cannibal[x] -> Join();
	}

	for(int y = 1; y <= numOfMissionaries; y = y + 1){
		missionary[y] -> Join();
	}
}

