/*
 * NAME: Alec Trent                             USER ID: ajtrent
 * DUE DATE: 04/01/2022
 * PROGRAM ASSIGNMENT 4
 * FILE NAME: thread-support.cpp
 * PROGRAM PURPOSE:
 *       This is where the functions of the threads live and operate
 *       Help consolidate synchronization.
 * DAYS LATE: 1
 * LATE DAYS REMAINING: 3
 */

#include "thread.h"     // threadMentor
#include <cstring>	// strlen()
int pots = 0;	// counter
int ret = 0; 	// bool stating mothers retirement

//---------------------------------------------------------------//
// FUNCTION: ready_to_eat()
//	blocks the caller, a baby eagle, if all feeding pots are empty.
//	One and only one baby eagle who finds out all feeding pots being
//	empty should wake up the mother eagle. This function returns only 
//	if there is a feeding pot with food available to this baby eagle.
// PARAMETERS: int i, Semaphore * Mom, int numOfPots, Semaphore * Baby, Mutex * MyLock, 
// 	Semaphore * Hold, Semaphore * Break
// FUNCTIONS CALLED: strlen(), sprintf(), write(), Wait(), Lock(), Signal(), Unlock()
//---------------------------------------------------------------//
int ready_to_eat(int i, Semaphore * Mom, int numOfPots, Semaphore * Baby, Mutex * MyLock, Semaphore * Hold, Semaphore * Break){
	char output[1000];

	Baby -> Wait();		// lets a child in per pot
	MyLock -> Lock();
	//sprintf(output, "Mutex locked \n");
	//write(1, output, strlen(output));

		//sprintf(output, "pots %d\n", pots);
		//write(1, output, strlen(output));
		if(pots == 0){	// global counter, if empty
			
			int count = 0;
        		while(count < i){
                		sprintf(output, " ");
				write(1, output, strlen(output));
               	 		count = count + 1;
              		}
			sprintf(output, "Baby eagle %d sees all feeding pots are empty and wakes up the mother.\n", i);
			write(1, output, strlen(output));

			count = 0;
        		while(count < i){
                		sprintf(output, " ");
				write(1, output, strlen(output));
               	 		count = count + 1;
              		}
			sprintf(output, "Mother eagle is awoke by baby eagle %d and starts preparing food.\n", i);
			write(1, output, strlen(output));

			Mom -> Signal();	// wake mom up
			Hold -> Wait();         // holds thread here while mom preps food

			MyLock -> Unlock();	// frees the mutex
			//sprintf(output, "Mutex unlocked \n");
			//write(1, output, strlen(output));
			return -1;
			
		} else {
			int count = 0;
        		while(count < i){
                		sprintf(output, " ");
				write(1, output, strlen(output));
               	 		count = count + 1;
              		 }
			pots = pots - 1;
			sprintf(output, "Baby eagle %d is eating using feeding pot %d.\n", i, numOfPots - pots);
			write(1, output, strlen(output));

			Baby -> Signal();       // feed next baby

			MyLock -> Unlock();
			//sprintf(output, "Mutex unlocked \n");
			//write(1, output, strlen(output));

			// return if feeding pot to eat from
			return 0;
		}
}


//---------------------------------------------------------------//
// FUNCTION: finish_eating()
// 	should be called when a baby eagle finishes eating.
// PARAMETERS: Semaphore * Finish, int i, Semaphore * Break
// FUNCTIONS CALLED: strlen(), sprintf(), write(), Wait(), Signal(), Exit()
//---------------------------------------------------------------//
void finish_eating(Semaphore * Finish, int i, Semaphore * Break){

	char output[1000];
	
	int count = 0;
        while(count < i){
        	sprintf(output, " ");
		write(1, output, strlen(output));
                count = count + 1;
        }
	sprintf(output, "Baby eagle %d finishes eating.\n", i);
	write(1, output, strlen(output));

	Finish -> Signal();                     // baby not hungry, just ate, emptied a pot

	if (ret == 1){		// mothers retired this keeps the children from exiting before the mother
		Break -> Wait();
		Exit();
	}
}


//---------------------------------------------------------------//
// FUNCTION: goto_sleep()
// 	only called by the mother eagle when she wants to take a nap.
// PARAMETERS: Semaphore * Mom, int numOfPots, Semaphore * Finish, int retire, Semaphore * Break
// FUNCTIONS CALLED: strlen(), sprintf(), write(), Wait(), Signal()
//---------------------------------------------------------------//
void goto_sleep(Semaphore * Mom, int numOfPots, Semaphore * Finish, int retire, Semaphore * Break){

	char output[1000];

	sprintf(output, "Mother eagle takes a nap.\n");
	write(1, output, strlen(output));	
	
	if (retire == 1){
		Mom -> Signal();	// last iteration, lets us bypass having a child signal
	}
	Mom -> Wait();                  // mom takes a nap, need to signal first cuz of the first iteration	

	// cant print here cuz i dont have i

	int count = 0;
	while(count < numOfPots){
		Finish -> Wait();	// checks # of signals against the wait to make sure all babies are done
		count = count + 1;
	}				// once the babies are all done we can wake mom
	
	if (retire == 1){		// if mom is retiring we let children exit after eating
		Break -> Signal();
	}
}


//---------------------------------------------------------------//
// FUNCTION: food_ready()
// 	called when the mother eagle has finished adding food in all m feeding pots.
// PARAMETERS: Semaphore * Baby, int numOfPots, Semaphore * Hold, Semaphore * Break, int retire
// FUNCTIONS CALLED: strlen(), sprintf(), write(),  Signal()
//---------------------------------------------------------------//
void food_ready(Semaphore * Baby, int numOfPots, Semaphore * Hold, Semaphore * Break, int retire){

	char output[1000];

	ret = retire;
	pots = numOfPots;		// reset counter
	//sprintf(output, "rdy pots %d\n", pots);
	//write(1, output, strlen(output));

	int count = 0;
	while(count < numOfPots){
		Baby -> Signal();       // babies can now begin eating
		count = count + 1;
	}

	Hold -> Signal();
}

