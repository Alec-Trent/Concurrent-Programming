/*
 * NAME: Alec Trent                             USER ID: ajtrent
 * DUE DATE: 04/01/2022
 * PROGRAM ASSIGNMENT 4
 * FILE NAME: thread-main.cpp
 * PROGRAM PURPOSE:
 *       This is where: 
 *       	we accept input. 
 *       	1 mom thread and n baby eagles are created and initialized. 
 *       	we also initialize our semaphores and mutex here as well
 * DAYS LATE: 1
 * LATE DAYS REMAINING: 3
 */

#include "ThreadClass.h"
#include <cstring>
#include "thread.h"	// threadMentor

using namespace std;	// control over scope

// declare semaphores
Semaphore * Baby;
Semaphore * Mom;
Semaphore * Finish;
Semaphore * Hold;
Semaphore * Break;
Mutex * MyLock;

/*
 * Summary
 * 0 < m <= n, m = feeding pots, n = baby eagles
 * Each baby must use a feeding pot, and each pot can only serve one at a time
 * All feeding pots are assumed to be empty at the very beginning and the mother eagle is sleeping
 * Once pot has food, baby can eat, it empties, mother refills
 * Mother sleeps until baby wakes her, she then fills all pots up, then sleeps
 * ONLY one baby can wake the mother up, weirdo thread, so mother gets more sleep
 * NOTE baby should not wake mother if others are eating and remaining pots are empty
 * After t-th meal mother retires, and babies are independent. END 
 *
 */

//---------------------------------------------------------------//
// FUNCTION: main()
//		accept input, initialize, and setup threads
// PARAMETERS:
// FUNCTIONS CALLED: strlen(), Begin(), Join(), atoi(), sprintf(), write()
//---------------------------------------------------------------//
int main(int argc, char *argv[]){

	// take input, if 0, use default 10
	int numOfPots = atoi(argv[1]);		// take input 1

	int numOfBabies = atoi(argv[2]);	// take input 2

	int numOfFeedings = atoi(argv[3]);	// take input 3

	if(numOfPots == 0){
		numOfPots = 10;
	}

	if(numOfBabies == 0){
		numOfBabies = 10;
	}

	if(numOfFeedings == 0){
		numOfFeedings = 10;
	}

	// print formatting
	char output[500];
	sprintf(output ,"MAIN: There are %d baby eagles, %d feeding pots, and %d feedings.\n", numOfBabies, numOfPots, numOfFeedings);
	write(1, output, strlen(output));

	sprintf(output,"MAIN: Game starts!!!!!\n");
	write(1, output, strlen(output));

	// init semaphores
	Baby = new Semaphore("Baby", 1);
	Mom = new Semaphore("Mom", 0);		// need to fill pots first before babies can eat
	Finish = new Semaphore("Finish", numOfPots);
	Mom = new Semaphore("Mom", 0);
	Hold = new Semaphore("Hold", 0);
	Break = new Semaphore("Break", 0);
	MyLock = new Mutex("MyLock");		// unlocked by default

	// create 1 thread for the Mom and numOfBabies threads for children
	momThread * mom;
	babyThread * baby[numOfBabies];

	mom = new momThread(Mom, Baby, numOfPots, numOfFeedings, Finish, Hold, Break);	// create the thread
	mom -> Begin();					// run the thread

	for(int i = 1; i <= numOfBabies; i = i + 1){
		baby[i] = new babyThread(i, Mom, numOfPots, Baby, Finish, MyLock, Hold, Break);// create the thread
		baby[i] -> Begin();				// run the thread
	}

	mom -> Join();                                  // join to make threads wait for mom retirement
	for(int j = 1; j <= numOfBabies; j = j + 1){
		baby[j] -> Join();
	}
}

