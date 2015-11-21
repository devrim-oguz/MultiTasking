#ifndef MultiTasking_h  /*Standard including protection to avoid including the same library multiple times.*/
#define MultiTasking_h

//Maximum amount of threads can be set here:
#ifndef MAXIMUM_THREADS
#define MAXIMUM_THREADS 50 //Max is 255 
#endif
//Maximum amount of timers can be set here:
#ifndef MAXIMUM_TIMERS
#define MAXIMUM_TIMERS	20 //Max is 255
#endif

#include "Arduino.h" //Adding standard Arduino library.

typedef struct { //Declaring a struct to hold thread informations.
	void ( *functionPointer )( void ); //A function pointer which turns nothing and gets nothing.
	boolean placeHolder; //A boolean value to hold the state of thread.( used or unused )
} ThreadElement; //Declaring a typedef to call the struct as ThreadElement.

typedef struct { //Declaring a struct to hold timer informations and timings.
	unsigned int execInterval, lastExecTime, timesToExec; //3 unsigned ints to hold function timings.
	void ( *functionPointer )( void ); //A function pointer which turns nothing and gets nothing.
	boolean placeHolder; //A boolean value to hold the state of timer.( used or unused )
} TimerElement; //Declaring a typedef to call the struct as TimerElement.

class TaskList //Class definition for a ThreadList ( A list that holds all threads and timers )
{
	private:
		ThreadElement _threadList[ MAXIMUM_THREADS ]; //Creating a ThreadElement array to hold threads.
		TimerElement _timerList[ MAXIMUM_TIMERS ]; //Creating a TimerElement array to hold timers.
		byte _firstThreadSpace = 0, _firstTimerSpace = 0; //Two values to hold the first empty thread and timer.
		int  _lastThreadFunction = -1, _lastTimerFunction = -1; //Two values to hold the last thread and timer.
		boolean _TaskListState = 0; //A boolean value to hold the state of TaskList.
		void findEmptyThread( void ); //A function to find the first empty thread of the thread array.
		void findEmptyTimer( void ); //A function to find the first empty timer of the timer array.
	public: //Public functions;
		boolean addThread( void ( * )( void ) ); //A function for adding a thread to run continuously.
		boolean setTimer( void ( * )( void ), unsigned int, unsigned int ); //A function to add a timer to run periodically.
		boolean isThreadRunning( void ( * )( void ) ); //A function to determine whether the thread is added or not.
		boolean isTimerRunning( void ( * )( void ) ); //A function to determine whether the timer is added or not.
		void removeThread( void ( * )( void ) ); //A function for removing an added thread.
		void killTimer( void ( * )( void ) ); //A function for removing an added timer.
		void startTasks( void ); //A function to start MultiTasking, which is an endless loop.
		void stopTasks( void ); //A function for stopping MultiTasking. ( stopping the endless loop )
		void flushThreads( void ); //A function for deleting all threads in thread array.
		void flushTimers( void ); //A function for deleting all timers in timer array.
		void flushAll( void ); //A function for deleting all timers and threads.
};

#endif //Ending include protection.

//!!!Thank You For Using The Library!!! <3 <3 <3