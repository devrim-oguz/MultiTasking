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

#ifndef MAIN_LOOP_ENABLED
#define MAIN_LOOP_ENABLED true //Change for activating or deactivating the main loop
#endif

#ifndef MILLI
#define MILLI	false //Max is 255
#endif

#ifndef MICRO
#define MICRO	true //Max is 255
#endif

#ifndef IF_LOOP_ENABLED
#define IF_LOOP_ENABLED	if(MAIN_LOOP_ENABLED)  //Used for either enabling or disabling the main loop
#endif

#include "Arduino.h" //Adding standard Arduino library.

typedef struct { //Declaring a struct to hold thread informations.
	void ( *functionPointer )( void ); //A function pointer which turns nothing and gets nothing.
	boolean placeHolder; //A boolean value to hold the state of thread.( used or unused )
} ThreadElement; //Declaring a typedef to call the struct as ThreadElement.

typedef struct { //Declaring a struct to hold timer informations and timings.
	unsigned long execInterval, lastExecTime; //2 unsigned longs to hold function timings.
	unsigned int timesToExec; //1 unsigned int to hold execution amount.
	void ( *functionPointer )( void ); //A function pointer which turns nothing and gets nothing.
	boolean placeHolder, timerType; //A boolean value to hold the state of timer.( used or unused )
} TimerElement; //Declaring a typedef to call the struct as TimerElement.

class TaskList //Class definition for a ThreadList ( A list that holds all threads and timers )
{
	private:
		ThreadElement _threadList[ MAXIMUM_THREADS ]; //Creating a ThreadElement array to hold threads.
		TimerElement _timerList[ MAXIMUM_TIMERS ]; //Creating a TimerElement array to hold timers.
		byte _firstThreadSpace = 0, _firstTimerSpace = 0; //Two values to hold the first empty thread and timer.
		int  _lastThreadFunction = -1, _lastTimerFunction = -1; //Two values to hold the last thread and timer.
		boolean _TaskListState = 0; //A boolean value to hold the state of TaskList.
		unsigned long _threadListStartTime = 0, _threadListEndTime = 0, _lastThreadListStartTime = 0;
		//Variables to hold the thread start and stop times to calculate the refresh rate of the library.
		void findEmptyThread( void ); //A function to find the first empty thread of the thread array.
		void findEmptyTimer( void ); //A function to find the first empty timer of the timer array.
	public: //Public functions;
		boolean addThread( void ( * )( void ) ); //A function for adding a thread to run continuously.
		boolean setTimer( void ( * )( void ), unsigned long, unsigned int );  //Overloaded setTimer function. Calls milli timer.
		boolean setTimer( void ( * )( void ), unsigned long, unsigned int, boolean ); //A function to add a timer to run periodically.
		boolean isThreadRunning( void ( * )( void ) ); //A function to determine whether the thread is added or not.
		boolean isTimerRunning( void ( * )( void ) ); //A function to determine whether the timer is added or not.
		void removeThread( void ( * )( void ) ); //A function for removing an added thread.
		void killTimer( void ( * )( void ) ); //A function for removing an added timer.
		void startTasks( void ); //A function to start MultiTasking, which is an endless loop.
		void cycleTasks( void ); //A function to cycle trough MultiTasking tasks once, used for user defined schedulers and loops.
		void stopTasks( void ); //A function for stopping MultiTasking. ( stopping the endless loop )
		void flushThreads( void ); //A function for deleting all threads in thread array.
		void flushTimers( void ); //A function for deleting all timers in timer array.
		void flushAll( void ); //A function for deleting all timers and threads.
		unsigned long getSpeed( void );
};

extern TaskList superTask;

#endif //Ending include protection.

//!!!Thank You For Using The Library!!! <3 <3 <3
