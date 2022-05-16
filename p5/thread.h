/*
* NAME: Alec Trent                             USER ID: ajtrent
* DUE DATE: 04/15/2022
* PROGRAM ASSIGNMENT 5
* FILE NAME: thread.h
* PROGRAM PURPOSE:
*	Set up threads and functions so that paramaters are correct and passed correctly.
* DAYS LATE: 1
* LATE DAYS REMAINING: 2
*/

#include "ThreadClass.h"
#include "boat-monitor.h"

//---------------------------------------------------------------//
//  PROCESS THREAD CLASS: cannibalThread
//      houses paramaters and function prototype to pass values
// PARAMETERS: 
// FUNCTIONS CALLED: ThreadFunc()
//---------------------------------------------------------------//
class cannibalThread : public Thread{
	public:
		cannibalThread(int i, boatMonitor * boatMon);

	private:
		void ThreadFunc();	// thread body
		int i;
		boatMonitor * boatMon;

};


//---------------------------------------------------------------//
//  PROCESS THREAD CLASS: missionaryThread
//      houses paramaters and function prototype to pass values
// PARAMETERS: 
// FUNCTIONS CALLED: ThreadFunc()
//---------------------------------------------------------------//
class missionaryThread : public Thread{
	public:
		missionaryThread(int j, boatMonitor * boatMon);

	private:
		void ThreadFunc();	// thread body
		int j;
		boatMonitor * boatMon;

};


//---------------------------------------------------------------//
//  PROCESS THREAD CLASS: boatThread
//      houses paramaters and function prototype to pass values
// PARAMETERS: 
// FUNCTIONS CALLED: ThreadFunc()
//---------------------------------------------------------------//
class boatThread : public Thread{
	public:
		boatThread(int numOfBoatLoads, boatMonitor * boatMon);

	private:
		void ThreadFunc();	// thread body
		int numOfBoatLoads;
		boatMonitor * boatMon;

};

