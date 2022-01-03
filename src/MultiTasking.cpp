#include "Arduino.h" /*Adding standard arduino library.*/
#include "MultiTasking.h" /*Adding MultiTasking header file.*/

TaskList superTask;

void TaskList::startTasks( void )  /*This function is an endless loop which calls all timers and threads*/
{ 								   /*depending on their state.*/
  _TaskListState = 1; //Setting TaskListState as True to run the endless loop.
  do { //Endless loop which controls the TaskListState variable.
    cycleTasks();
  } while( _TaskListState );
}

inline void TaskList::cycleTasks( void )  /*This function is an finite call which calls all timers and threads*/
{ 								   /*depending on their state.*/

  for( byte i = 0; i <= _lastThreadFunction; i++ ) { //A cycle for running all threads.
    if( _threadList[i].placeHolder ) { //Controlling whether the thread is active or not.
      ( *_threadList[i].functionPointer )(); //Calling the thread.
    }
  }

  for( byte i = 0; i <= _lastTimerFunction; i++ ) { //A cycle for running all timers.

    if( _timerList[i].placeHolder ) { //Controlling whether the timer is active or not.

      static unsigned long currentTime = 0; //Getting the current time for checking the timer information.

      if( _timerList[i].timerType ) { //Checking if the timer is a micro or milli timer.
        currentTime = micros();  //Recording the current time for calculations.
      }
      else {
        currentTime = millis(); //Recording the current time for calculations.
      }

      if( ( currentTime - _timerList[i].lastExecTime ) >= _timerList[i].execInterval ) { //Checking for execution.

        if( _timerList[i].timerType ) { //Checking if the timer is a micro or milli timer.
          _timerList[i].lastExecTime = micros();  //Recording the current time for last execution time.
	       }
	      else {
	        _timerList[i].lastExecTime = millis();  //Recording the current time for last execution time.
	      }

        ( *_timerList[i].functionPointer )(); //Calling the function.

        if( _timerList[i].timesToExec == 1 ) { //If timesToExec is 1, run it one time and remove it.
          _killIndexedTimer(i); //Function for removing timers.
        }
        else if( _timerList[i].timesToExec > 1 ) { //If timesToExec is more than 1, run it and make it one less.
          _timerList[i].timesToExec--; //Subtracting 1 from timesToExec.
        }
      }
    }
  }

  //loop(); //Calling the loop function.

  _lastExecutionTime = _currentExecutionTime;  //Updating the last execution time for calculations.
  _currentExecutionTime = micros();  //Recording the current execution time for calculations.
}

unsigned long TaskList::getSpeed( void )
{ //Defining a function to get the refreshing speed of the library in Hertz. (Executions per second)
  unsigned long timeDifference = ( _currentExecutionTime - _lastExecutionTime );
  if( timeDifference == 0 ) { //Avoiding the divide by zero error
    return 0; //0 Means infinite speed :)
  }
  else {
    return (unsigned long)round( ( 1000000 / (double)timeDifference ) ); //Calculating the speed, 1 Second = 1000000 Micro Seconds
  }
}

void TaskList::stopTasks( void )
{ //Defining a function to stop endless loop of MultiTasking.
	_TaskListState = 0; //Setting the bool value as False.
}

bool TaskList::addThread( void ( *threadFunction )( void ) ) //A function for adding a thread.
{ //Checking the thread list for empty space and same functions.
  if( ( _lastThreadFunction < ( MAXIMUM_THREADS - 1 ) ) && !isThreadRunning( threadFunction ) ) {
    _threadList[ _firstThreadSpace ].functionPointer = threadFunction; //Adding function pointer to the list.
    _threadList[ _firstThreadSpace ].placeHolder = true; //Setting the thread as active.
    if( _lastThreadFunction < _firstThreadSpace ) { //Checking if the last thread is before than the first space
      _lastThreadFunction = _firstThreadSpace; //If so, setting the last thread as first space.
    }
    findEmptyThread(); //Finding the next empty space.
    return true; //If operation succeed, return 1.
  }
  else { //If operation is not succeed,
	 return false; //return 0.
  }
}

bool TaskList::setTimer( void ( *timerFunction )( void ), unsigned long repeatInterval, unsigned int execAmount )
{ //Overloaded setTimer function, calls setTimer method with MILLI parameter. ( Milli Timer )
  return setTimer( timerFunction, repeatInterval, execAmount, MILLI );
}

//A function for adding a timer.

bool TaskList::setTimer( void ( *timerFunction )( void ), unsigned long repeatInterval, unsigned int execAmount, bool typeSelection )
{ //Checking the timer lislt for empty space and same functions.
  if( ( _lastTimerFunction < ( MAXIMUM_TIMERS - 1 ) ) && !isTimerRunning( timerFunction ) ) {
    _timerList[ _firstTimerSpace ].execInterval = repeatInterval; //Setting repeat interval as given value.
    _timerList[ _firstTimerSpace ].timesToExec = execAmount; //Setting execAmount as given value.
    _timerList[ _firstTimerSpace ].functionPointer = timerFunction; //Adding function pointer to the list.
    _timerList[ _firstTimerSpace ].timerType = typeSelection;

    if( typeSelection ) { //Checking the type of the timer
    	_timerList[ _firstTimerSpace ].lastExecTime = micros(); //Setting the adding time as last run time.
    }
    else {
    	_timerList[ _firstTimerSpace ].lastExecTime = millis(); //Setting the adding time as last run time.
    }

    _timerList[ _firstTimerSpace ].placeHolder = true; //Setting the timer as active.
    if( _lastTimerFunction < _firstTimerSpace ){ //Checking if the last timer is before than the first space
      _lastTimerFunction = _firstTimerSpace; //If so, setting the last timer as first space.
    }

    findEmptyTimer(); //Finding the next empty space.
    return true; //If operation succeed, return 1.
  }
  else { //If operation is not succeed,
	return false; //return 0.
  }
}

void TaskList::findEmptyThread( void ) //A function for finding the first space of thread list.
{
  for( byte currentThread = _firstThreadSpace; currentThread < MAXIMUM_THREADS; currentThread++ ) {
    if( !_threadList[ currentThread ].placeHolder ){ //Scanning the whole active list.
      _firstThreadSpace = currentThread; //If a space found, setting the space as first empty space.
      break; //quitting the loop.
    }
    else if( currentThread == ( MAXIMUM_THREADS - 1 ) ) { //If nothing found,
      _firstThreadSpace = currentThread; //Setting the last space as first empty space
    }
  }
}

void TaskList::findEmptyTimer( void ) //A function for finding the first space of timer list.
{ //This function works just like the same as before. ( findEmptyThread )
  for( byte currentTimer = _firstTimerSpace; currentTimer < MAXIMUM_TIMERS; currentTimer++ ) {
    if( !_timerList[ currentTimer ].placeHolder ){
      _firstTimerSpace = currentTimer;
      break;
    }
    else if( currentTimer == ( MAXIMUM_TIMERS - 1 ) ) {
      _firstTimerSpace = currentTimer;
    }
  }
}

void TaskList::removeThread( void ( *threadAddress )( void ) ) //A function for removing a thread.
{
  for( byte j = 0; j <= _lastThreadFunction; j++ ) { //Scanning whole list to find a match
    if( threadAddress == _threadList[j].functionPointer ) { //If found,
      if( j < _firstThreadSpace ) { //If the function was before than first space,
        _firstThreadSpace = j; //Set it as first space.
      }
      if( j == _lastThreadFunction ) {
        _lastThreadFunction--;
      }
      _threadList[j].functionPointer = NULL;
      _threadList[j].placeHolder = false; //Set as unactive.
    }
  }
}

void TaskList::killTimer( void ( *timerAddress )( void ) ) //A function for removing a timer.
{ //This function works just like the same as before. ( removeThread )
  for( byte j = 0; j <= _lastTimerFunction; j++ ) {
    if( timerAddress == _timerList[j].functionPointer ) {
      if( j < _firstTimerSpace ) {
        _firstTimerSpace = j;
      }
      if( j == _lastTimerFunction ) {
        _lastTimerFunction--;
      }
      _timerList[j].functionPointer = NULL;
      _timerList[j].placeHolder = false;
    }
  }
}

inline void TaskList::_killIndexedTimer( byte inputIndex ) //A function for removing a timer.
{ //This function works just like the same as before. ( removeThread )
  if( inputIndex < _firstTimerSpace ) {
    _firstTimerSpace = inputIndex;
  }
  if( inputIndex == _lastTimerFunction ) {
    _lastTimerFunction--;
  }
  _timerList[inputIndex].functionPointer = NULL;
  _timerList[inputIndex].placeHolder = false;
}

bool TaskList::isThreadRunning( void ( *threadAddress )( void ) ) //A function for checking the state of a thread.
{
  for( byte j = 0; j <= _lastThreadFunction; j++ ) { //Searching whole list for a match.
    if( threadAddress == _threadList[j].functionPointer && _threadList[j].placeHolder ) { //If found,
    	return true; //return 1.
    }
  }
  return false; //If not found, return 0.
}

bool TaskList::isTimerRunning( void ( *timerAddress )( void ) )
{ //This function works just like the same as before. ( isThreadRunning )
  for( byte j = 0; j <= _lastTimerFunction; j++ ) {
    if( timerAddress == _timerList[j].functionPointer && _timerList[j].placeHolder ) {
    	return true;
    }
  }
  return false;
}

void TaskList::flushThreads( void )  //A function for deleting all threads.
{
  for( byte i = 0; i < MAXIMUM_THREADS; i++ ) { //Selecting all threads one by one,
    _threadList[i].functionPointer = NULL; //Making the function pointer as NULL,
    _threadList[i].placeHolder = false; //Setting it as unactive,
  }
  _firstThreadSpace = 0; //Setting the first thread space as 0,
  _lastThreadFunction = -1; //Setting the last thread function as -1, which is none.
}

void TaskList::flushTimers( void )
{ //This function works just like the same as before. ( flushThreads )
  for( byte i = 0; i < MAXIMUM_TIMERS; i++ ) {
    _timerList[i].execInterval = 0; //Setting execInterval as 0,
    _timerList[i].lastExecTime = 0; //Setting lastExecTime as 0,
    _timerList[i].timesToExec = 0; //Setting timesToExec as 0,
    _timerList[i].functionPointer = NULL; //Setting function pointer as NULL,
    _timerList[i].placeHolder = false; //Setting it as unactive,
  }
  _firstTimerSpace = 0; //Setting the first timer space as 0,
  _lastTimerFunction = -1; //Setting the last timer function as -1, which is none.
}

void TaskList::flushAll( void ) //A function for deleting all threads and timers.
{
  flushThreads(); //Deleting all threads.
  flushTimers(); //Deleting all timers.
}

//!!!Thank You For Using The Library!!! <3 <3 <3
