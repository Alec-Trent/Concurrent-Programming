/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 04/15/2022
* PROGRAM ASSIGNMENT 5
* FILE NAME: boat-monitor.h
* PROGRAM PURPOSE:
*		Determine class attributes and initialize
*		functions/variables/conditions needed in
*		the monitor
* DAYS LATE: 1
* LATE DAYS REMAINING: 2
*/

#include "ThreadClass.h"

//---------------------------------------------------------------//
//  PROCESS MONITOR CLASS: boatMonitor
//      houses paramaters and function prototype to pass values
// PARAMETERS: 
// FUNCTIONS CALLED: CannibalArrives(), MissionaryArrives(),
// 		BoatReady(), BoatDone(), ThreadFunc()
//---------------------------------------------------------------//
class boatMonitor : public Monitor{
	public:
		boatMonitor(char * Name);
		void CannibalArrives(int i);
		void MissionaryArrives(int j);
		void BoatReady(int numOfBoatLoads);
		void BoatDone();

	private:
		void ThreadFunc();
		int crossingNumber;
		int cCount;
		int mCount;
		int position;
		int makeup[3];
		char *charac[3];
		int sorted[3];
		Condition * leaveBoat;
		Condition * missionary;
		Condition * cannibal;
		Condition * safe;
};

