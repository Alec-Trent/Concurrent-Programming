/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 04/15/2022
* PROGRAM ASSIGNMENT 5
* FILE NAME: boat-monitor.cpp
* PROGRAM PURPOSE:
*	This is where the functions and implementation of
*	the boat monitor lives and operates 
* DAYS LATE: 1
* LATE DAYS REMAINING: 2
*/

#include <cstring>      // strlen()
#include "boat-monitor.h"

boatMonitor::boatMonitor(char * Name) : Monitor("Boatmonitor", HOARE){

	crossingNumber = 0;
	cCount = 0;
	mCount = 0;
	position = 0;
	makeup[3];
	charac[3];
	sorted[3];

	leaveBoat = new Condition("leaveBoat");
	missionary = new Condition("missionary");
	cannibal = new Condition("cannibal");
	safe = new Condition("safe");	
}


//---------------------------------------------------------------//
// FUNCTION: CannibalArrives()
// PARAMETERS: int i
// FUNCTIONS CALLED: strlen(), sprintf(), write(), Signal(), Wait()
// 		MonitorBegin(), MonitorEnd()
//---------------------------------------------------------------//
void boatMonitor::CannibalArrives(int i){
	MonitorBegin();			// begin

	char output[1000];		// printing 
	int count = 0;
	while(count < i-1){
		sprintf(output, " ");
		write(1, output, strlen(output));
		count = count + 1;
	}
	sprintf(output, "Cannibal %d arrives\n", i);
	write(1, output, strlen(output));

	cCount = cCount + 1;		// number of cannibals waiting

	if(cCount >= 1 && mCount >= 2){
		safe -> Signal();
	}

	cannibal -> Wait();		// wait to load into boat, block
	
	charac[position] = "c";
	makeup[position] = i;		// store cannibal in array of boat makeup
	position = position + 1;	// increase counter

	leaveBoat -> Wait();		// boat is full/being loaded, block until empty
	cCount = cCount - 1;    	// cannibal crossed, decrease number

	if(position > 2){		// reset position
		position = 0;
	}

	MonitorEnd();			// end
}


//---------------------------------------------------------------//
// FUNCTION: MissionaryArrives()
// PARAMETERS: int j
// FUNCTIONS CALLED: strlen(), sprintf(), write(), Signal(), Wait()
// 		MonitorBegin(), MonitorEnd()
//---------------------------------------------------------------//
void boatMonitor::MissionaryArrives(int j){
	MonitorBegin();			// begin

	char output[1000];		// printing 
	int count = 0;
	while(count < j-1){
		sprintf(output, " ");
		write(1, output, strlen(output));
		count = count + 1;
	}
	sprintf(output, "Missionary %d arrives\n", j);
	write(1, output, strlen(output));

	mCount = mCount + 1;		// number of missionaries waiting

	if(mCount >= 2 && cCount >= 1){
		safe -> Signal();
	}

	missionary -> Wait();		// wait to load into boat, block

	charac[position] = "m";
	makeup[position] = j;   	// store missionary in array of boat makeup
	position = position + 1;        // increase counter

	leaveBoat -> Wait();		// boat is full/being loaded, block until empty
	mCount = mCount - 1;		// missionary crossed, decrease number

	if(position > 2){		// reset position here
		position = 0;
	}

	MonitorEnd();			// end
}


//---------------------------------------------------------------//
// FUNCTION: BoatReady()
// PARAMETERS: int numOfCrossings
// FUNCTIONS CALLED: strlen(), sprintf(), write(), MonitorBegin(),
// 		MonitorEnd(), rand(), Exit(), Signal(), Wait()
//---------------------------------------------------------------//
void boatMonitor::BoatReady(int numOfCrossings){
	MonitorBegin();
	char output[1000];      // printing
	
	crossingNumber = crossingNumber + 1;	// this is init to 0 so 1 on first run

	// all crossing completed, end the program gracefully
	if(crossingNumber == numOfCrossings + 1){
		sprintf(output, "MONITOR: %d crosses have been made.\n", crossingNumber - 1);
		write(1, output, strlen(output));
		sprintf(output, "MONITOR: This river cross is closed indefinitely for renovation.\n");
		write(1, output, strlen(output));

		Exit();
	}

	// extreme bias towards cases so to fix that, add condition
	int num = rand() % 3;	// random number from 0 to 2

	if(cCount >= 3 && num == 0){		// three cannibals
		cannibal -> Signal();
		cannibal -> Signal();
		cannibal -> Signal();

		sprintf(output, "MONITOR(%d): three cannibals (%d, %d, %d) are selected\n", crossingNumber, makeup[0], makeup[1], makeup[2]);
		write(1, output, strlen(output));

	} else if(mCount >= 3 && num == 1){	// three missionaries
		missionary -> Signal();
		missionary -> Signal();
		missionary -> Signal();

		sprintf(output, "MONITOR(%d): three missionaries (%d, %d, %d) are selected\n", crossingNumber, makeup[0], makeup[1], makeup[2]);
		write(1, output, strlen(output));

	} else {				// 1 cannibal, 2 missionaries
		if(cCount < 1 || mCount < 2){	// this keeps the boat from going with
			safe -> Wait();		// less than 3 people being loaded
		}

		missionary -> Signal();
		missionary -> Signal();
		cannibal -> Signal();

		int cnt = 0;			// print correct cannibal value / formatting
		int srt = 1;
		while(cnt < 3){
			if(charac[cnt] == "c"){
				sorted[0] = makeup[cnt];	
			} else {
				sorted[srt] = makeup[cnt];
				srt = srt + 1;
			}
			cnt = cnt + 1;
		}

		sprintf(output, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", crossingNumber,sorted[0], sorted[1], sorted[2]);
		write(1, output, strlen(output));
	}

	sprintf(output, "***** Boat load (%d): Passenger list (%s%d, %s%d, %s%d)\n", crossingNumber, charac[0], makeup[0], charac[1], makeup[1], charac[2], makeup[2]);
	write(1, output, strlen(output));

	MonitorEnd();
}


//---------------------------------------------------------------//
// FUNCTION: BoatDone()
// PARAMETERS:
// FUNCTIONS CALLED: strlen(), sprintf(), write(), MonitorBegin(),
// 		MonitorEnd(), Signal()
//---------------------------------------------------------------//
void boatMonitor::BoatDone(){
	MonitorBegin();
	
	// signals that tell us that passengers are exiting the boat
	leaveBoat -> Signal();
	leaveBoat -> Signal();
	leaveBoat -> Signal();

	MonitorEnd();
}

