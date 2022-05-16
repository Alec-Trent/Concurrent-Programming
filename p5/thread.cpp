/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 04/15/2022
* PROGRAM ASSIGNMENT 5
* FILE NAME: thread.cpp
* PROGRAM PURPOSE:
*	This is where the class Cannibal, Missionary,
*	and Boat are implemented, all actions of
*	each class are handled here.
* DAYS LATE: 1
* LATE DAYS REMAINING: 2
*/

#include "thread.h"     // threadMentor
#include <cstring>      // strlen()

int skip = 1;
int crossingNumber = 0;

//---------------------------------------------------------------//
// CLASS: cannibalThread
//      class constructor
// PARAMETERS: 
// FUNCTIONS CALLED:
//---------------------------------------------------------------//
cannibalThread::cannibalThread(int i, boatMonitor * boatMon) : i(i), boatMon(boatMon){

}

//---------------------------------------------------------------//
// CLASS: missionaryThread
//      class constructor
// PARAMETERS: 
// FUNCTIONS CALLED:
//---------------------------------------------------------------//
missionaryThread::missionaryThread(int j, boatMonitor * boatMon) : j(j), boatMon(boatMon){

}

//---------------------------------------------------------------//
// CLASS: boatThread
//      class constructor
// PARAMETERS: 
// FUNCTIONS CALLED:
//---------------------------------------------------------------//
boatThread::boatThread(int numOfBoatLoads, boatMonitor * boatMon) : numOfBoatLoads(numOfBoatLoads), boatMon(boatMon){

}

//---------------------------------------------------------------//
// CLASS BODY: cannibalThread
//      operations performed by cannibal thread
// PARAMETERS:
// FUNCTIONS CALLED: strlen(), sprintf(), write(), CannibalArrives()
// 		Delay(), srand(), time()
//---------------------------------------------------------------//
void cannibalThread::ThreadFunc(){
	Thread:: ThreadFunc();

	char output[1000];      		// printing
	int count = 0;
	while(count < i-1){
		sprintf(output, " ");
		write(1, output, strlen(output));
		count = count + 1;
	}
	sprintf(output, "Cannibal %d starts\n", i);
	write(1, output, strlen(output));

	while (1) { 
		srand(time(0));                 // documentation, seed it once
		int random = rand() % 1000;     // generate random number
		int rCount = 0;
		while(rCount < random){
			Delay();		// take a rest
			rCount = rCount + 1;
		}

     		boatMon -> CannibalArrives(i);  // register to cross the river  
     	}
} 


//---------------------------------------------------------------//
// CLASS BODY: missionaryThread
//      operations performed by missionary thread
// PARAMETERS:
// FUNCTIONS CALLED: strlen(), sprintf(), write(), MissionaryArrives()
// 		Delay(), srand(), time()
//---------------------------------------------------------------//
void missionaryThread::ThreadFunc(){
	Thread:: ThreadFunc();

	char output[1000];     			 // printing
	int count = 0;
	while(count < j-1){
		sprintf(output, " ");
		write(1, output, strlen(output));
		count = count + 1;
	}
	sprintf(output, "Missionary %d starts\n", j);
	write(1, output, strlen(output));
	
	while (1) { 
		srand(time(0));                 // documentation, seed it once
		int random = rand() % 1000;     // generate random number
		int rCount = 0;
		while(rCount < random){
			Delay();		// take a rest
			rCount = rCount + 1;
		}

     		boatMon -> MissionaryArrives(j); // register to cross the river  
     	}
} 


//---------------------------------------------------------------//
// CLASS BODY: boatThread
//      operations performed by boat thread
// PARAMETERS:
// FUNCTIONS CALLED: strlen(), sprintf(), write(), BoatReady(), BoatDone()
// 		Delay(), srand(), time()
//---------------------------------------------------------------//
void boatThread::ThreadFunc(){
	Thread:: ThreadFunc();

	char output[1000];      			// printing
	sprintf(output, "***** BOAT thread starts\n");
	write(1, output, strlen(output));

	while (1) { 
		srand(time(0));                 	// documentation, seed it once
		int random = rand() % 1000;     	// generate random number
		int rCount = 0;
		while(rCount < random){
			Delay();			// take a rest
			rCount = rCount + 1;
		}
		
		// a boat load completes only if the boat comes back for the next boat load
		if(skip == 0){
			sprintf(output, "***** Boat load (%d): Complete\n", crossingNumber);
			write(1, output, strlen(output));
		}
		skip = 0;       			// update toggle to update crossing except on the first run
	
		crossingNumber = crossingNumber + 1;

		sprintf(output, "***** The boat is ready\n");
		write(1, output, strlen(output));

     		boatMon -> BoatReady(numOfBoatLoads);  	// ready for the next round 

		random = rand() % 1000;     		// generate random number
		rCount = 0;
		while(rCount < random){
			Delay();			// take a rest
			rCount = rCount + 1;
		}

     		boatMon -> BoatDone();   // all people are on the other side 
    	}
} 

