/*
 * NAME: Alec Trent                             USER ID: ajtrent
 * DUE DATE: 04/01/2022
 * PROGRAM ASSIGNMENT 4
 * FILE NAME: thread.cpp
 * PROGRAM PURPOSE:
 *       This is where the class babyThread and momThread are implemented,
 *       all actions of baby and mom are handled here
 * DAYS LATE: 1
 * LATE DAYS REMAINING: 3
 */

#include "thread.h"     // threadMentor
#include <cstring>      // strlen()

//---------------------------------------------------------------//
// CLASS: babyThread
//      class constructor
// PARAMETERS: int i, Semaphore * Mom, int numOfPots, Semaphore * Baby,
// 	 Semaphore * Finish, Mutex * MyLock, Semaphore * Hold, Semaphore * Break
// FUNCTIONS CALLED: strlen(), sprintf(), write()
//---------------------------------------------------------------//
babyThread::babyThread(int i, Semaphore * Mom, int numOfPots, Semaphore * Baby, Semaphore * Finish, Mutex * MyLock, Semaphore * Hold, Semaphore * Break) : i(i), Mom(Mom), numOfPots(numOfPots), Baby(Baby), Finish(Finish), MyLock(MyLock), Hold(Hold), Break(Break){
	char output[1000];
	int count = 0;
	while(count < i){
		sprintf(output, " ");
		write(1, output, strlen(output));
		count = count + 1;
	}
	sprintf(output, "Baby eagle %d started.\n", i);
	write(1, output, strlen(output));
}

//---------------------------------------------------------------//
// CLASS: momThread
//      class constructor
// PARAMETERS: Semaphore * Mom, Semaphore * Baby, int numOfPots, int numOfFeedings, 
// 	Semaphore * Finish, Semaphore * Hold, Semaphore * Break
// FUNCTIONS CALLED: strlen(), sprintf(), write()
//---------------------------------------------------------------//
momThread::momThread(Semaphore * Mom, Semaphore * Baby, int numOfPots, int numOfFeedings, Semaphore * Finish, Semaphore * Hold, Semaphore * Break) : Mom(Mom), Baby(Baby), numOfPots(numOfPots), numOfFeedings(numOfFeedings), Finish(Finish), Hold(Hold), Break(Break){
	char output[1000];
	sprintf(output + strlen(output), "Mother eagle started.\n");
	write(1, output, strlen(output));
}


//---------------------------------------------------------------//
// CLASS BODY: babyThread
//	operations performed by each baby eagle thread. loop baby gets
//		hungry and eats, then reports done eating, loop
// PARAMETERS:
// FUNCTIONS CALLED: strlen(), sprintf(), write(), ready_to_eat(), finish_eating()
//---------------------------------------------------------------//
void babyThread::ThreadFunc(){
	Thread::ThreadFunc();

	while(1){
		char output[1000];		// buffer to store output
		srand(time(0));			// documentation, seed it once 
		int random = rand() % 1000;		// generate random number

		int rCount = 0;
		while(rCount < random){		// play for a while
			Delay();		// you may call Delay() multiple times
						// (use a random number generater)
						// to have a longer and more random delay
			rCount = rCount + 1;
		}

		int count = 0;
		while(count < i){
			sprintf(output, " ");
			write(1, output, strlen(output));
			count = count + 1;
		}
		sprintf(output, "Baby eagle %d is ready to eat.\n", i);
		write(1, output, strlen(output));

		int rtn = ready_to_eat(i, Mom, numOfPots, Baby, MyLock, Hold, Break);

		rCount = 0;
		while(rCount < random){
			Delay();		// eat for a while you may call Delay() multiple times
						// (use a random number generater)
						// to have a longer and more random delay
			rCount = rCount + 1;
		}

		if(rtn != -1){
     			finish_eating(Finish, i, Break); 		// finish eating     
                }          			// do some other thing
	}
	//Exit();
}

//---------------------------------------------------------------//
// CLASS BODY: momThread
//	operations performed by the mom eagle thread. loop for each feeding
//		she sleeps, woken, fills pots, back to sleep
// PARAMETERS:
// FUNCTIONS CALLED: strlen(), sprintf(), write(), goto_sleep(), food_ready()
//---------------------------------------------------------------//
void momThread::ThreadFunc(){
	Thread::ThreadFunc();
	
	char output[1000];		// buffer to store output
	srand(time(0));			// documentation, seed it once 
	int random = rand() % 100;		// generate random number

	int retire = 0;			// bool to help mother retire at correct time
	int feedCount = 0;
	while(feedCount < numOfFeedings) {             
     		
		goto_sleep(Mom, numOfPots, Finish, retire, Break);    		// take a nap 

		int rCount = 0;
		while(rCount < random){
			Delay();		// you may call Delay() multiple times
						// (use a random number generater)
						// to have a longer and more random delay
			rCount = rCount + 1;
		}
		
		if(feedCount == numOfFeedings - 1){	// last run
			retire = 1;	// mom is ging to retire
		}
     		food_ready(Baby, numOfPots, Hold, Break, retire);    	// make food available 
		sprintf(output, "Mother eagle says 'Feeding (%d)'.\n", feedCount + 1);
		write(1, output, strlen(output));

		rCount = 0;
		while(rCount < random){
			Delay();		// you may call Delay() multiple times
						// (use a random number generater)
						// to have a longer and more random delay
			rCount = rCount + 1;
		}

     		feedCount = feedCount + 1;
	}	

	//retire = 1;		// provides a signla to get through wait on last run
	goto_sleep(Mom, numOfPots, Finish, retire, Break);	// allows us to wait for children to stop eating
	sprintf(output, "Mother eagle retires after serving %d feedings. Game ends!!!.\n", numOfFeedings);
	write(1, output, strlen(output));
	Exit();
}

