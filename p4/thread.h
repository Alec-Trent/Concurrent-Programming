/*
 * NAME: Alec Trent                             USER ID: ajtrent
 * DUE DATE: 04/01/2022
 * PROGRAM ASSIGNMENT 4
 * FILE NAME: thread.h
 * PROGRAM PURPOSE:
 *       Set up threads and functions so that paramaters are correct and passed correctly.
 * DAYS LATE: 1
 * LATE DAYS REMAINING: 3
 */

 #include "ThreadClass.h"

//---------------------------------------------------------------//
//  PROCESS THREAD CLASS: babyThread
//      houses paramaters and function prototype to pass values
// PARAMETERS: int i, Semaphore * Mom, int numOfPots, Semaphore * Baby, Semaphore * Finish, 
// 	Mutex * MyLock, Semaphore * Hold, Semaphore * Break
// FUNCTIONS CALLED:
//---------------------------------------------------------------//
 class babyThread : public Thread{
 	public:	
 		babyThread(int i, Semaphore * Mom, int numOfPots, Semaphore * Baby, Semaphore * Finish, Mutex * MyLock, Semaphore * Hold, Semaphore * Break);     
 
 	private:
		void ThreadFunc();      // thread body
		int i;                  // 
		Semaphore * Mom;
		int numOfPots;
		Semaphore * Baby;
		Semaphore * Finish;
		Mutex * MyLock;
		Semaphore * Hold;
		Semaphore * Break;
 };

//---------------------------------------------------------------//
//  PROCESS THREAD CLASS: momThread
//      houses paramaters and function prototype to pass values
// PARAMETERS: Semaphore * Mom, Semaphore * Baby, int numOfPots, int numOfFeedings, 
// 	Semaphore * Finish, Semaphore * Hold, Semaphore * Break
// FUNCTIONS CALLED:
//---------------------------------------------------------------//
class momThread : public Thread{
 	public:
 		momThread(Semaphore * Mom, Semaphore * Baby, int numOfPots, int numOfFeedings, Semaphore * Finish, Semaphore * Hold, Semaphore * Break);    
 
 	private:
		void ThreadFunc();      // thread body
		Semaphore * Mom;
		Semaphore * Baby;
		int numOfPots;
		int numOfFeedings;
		Semaphore * Finish;
		Semaphore * Hold;
		Semaphore * Break;
 };

void food_ready(Semaphore * Baby, int numOfPots, Semaphore * Hold, Semaphore * Break, int retire);
void goto_sleep(Semaphore * Mom, int numOfPots, Semaphore * Finish, int retire, Semaphore * Break);
void finish_eating(Semaphore * Finish, int i, Semaphore * Break);
int ready_to_eat(int i, Semaphore * Mom, int numOfPots, Semaphore * Baby, Mutex * MyLock, Semaphore * Hold, Semaphore * Break);

