#include "Arduino.h" /*Adding standard arduino library.*/
#include "MultiTasking.h" /*Adding MultiTasking header file.*/

void TaskList::startTasks( void )  /*This function is an endless loop which calls all timers and threads*/
{ 								   /*depending on their state.*/
  _TaskListState = 1; //Setting TaskListState as True to run the endless loop.
  while( _TaskListState ) { //Endless loop which controls the TaskListState variable.
    for( byte i = 0; i <= _lastThreadFunction; i++ ) { //A cycle for running all threads.
	  if( _threadList[i].placeHolder ) { //Controlling whether the thread is active or not.
	  	( *_threadList[i].functionPointer )(); //Calling a thread.
	  }
    }
    for( byte i = 0; i <= _lastTimerFunction; i++ ) { //A cycle for running all timers.
      if( _timerList[i].placeHolder ) { //Controlling whether the timer is active or not.
        unsigned int currentTime = millis(); //Getting the current time for checking the timer information.
        if( ( currentTime - _timerList[i].lastExecTime ) >= _timerList[i].execInterval ) { //Checking for execution.
          if( _timerList[i].timesToExec == 1 ) { //If timesToExec is 1, run it one time and remove it.
            killTimer( _timerList[i].functionPointer ); //Function for removing timers.
          }
          else if( _timerList[i].timesToExec > 1 ) { //If timesToExec is more than 1, run it and make it one less.
            _timerList[i].timesToExec--; //Subtracting 1 from timesToExec.
          }
          _timerList[i].lastExecTime = millis(); //Setting the last runtime of the function.
          ( *_timerList[i].functionPointer )(); //Calling the function.
        }
      }
    }
    loop(); //Calling the loop function.
  }
}

void TaskList::stopTasks( void ) { //Defining a function to stop endless loop of MultiTasking.
	_TaskListState = 0; //Setting the boolean value as False.
}

int TaskList::addThread( void ( *threadFunction )( void ) ) //A function for adding a thread.
{ //Checking the thread list for empty space and same functions.
  if( ( _lastThreadFunction < ( MAXIMUM_THREADS - 1 ) ) && !isThreadRunning( threadFunction ) ) {
    _threadList[ _firstThreadSpace ].functionPointer = threadFunction; //Adding function pointer to the list.
    _threadList[ _firstThreadSpace ].placeHolder = 1; //Setting the thread as active.
    if( _lastThreadFunction < _firstThreadSpace ) { //Checking if the last thread is before than the first space
      _lastThreadFunction = _firstThreadSpace; //If so, setting the last thread as first space.
    }
    findEmptyThread(); //Finding the next empty space.
    return 1; //If operation succeed, return 1.
  }
  else { //If operation is not succeed,
	 return 0; //return 0.
  }
}
//A function for adding a timer.
int TaskList::setTimer( void ( *timerFunction )( void ), unsigned int repeatInterval, unsigned int execAmount )
{ //Checking the timer lislt for empty space and same functions.
  if( ( _lastTimerFunction < ( MAXIMUM_TIMERS - 1 ) ) && !isTimerRunning( timerFunction ) ) {
    _timerList[ _firstTimerSpace ].execInterval = repeatInterval; //Setting repeat interval as given value.
    _timerList[ _firstTimerSpace ].timesToExec = execAmount; //Setting execAmount as given value.
    _timerList[ _firstTimerSpace ].functionPointer = timerFunction; //Adding function pointer to the list.
    _timerList[ _firstTimerSpace ].lastExecTime = millis(); //Setting the lastruntime as adding time.
    _timerList[ _firstTimerSpace ].placeHolder = 1; //Setting the timer as active.
    if( _lastTimerFunction < _firstTimerSpace ){ //Checking if the last timer is before than the first space
      _lastTimerFunction = _firstTimerSpace; //If so, setting the last timer as first space.
    }
    findEmptyTimer(); //Finding the next empty space.
    return 1; //If operation succeed, return 1.
  }
  else { //If operation is not succeed,
	return 0; //return 0.
  }
}

void TaskList::findEmptyThread( void ) //A function for finding the first space of thread list.
{
  for( byte currentThread = _firstThreadSpace; currentThread < MAXIMUM_THREADS; currentThread++ ) {
    if( _threadList[ currentThread ].placeHolder == 0 ){ //Scanning the whole active list.
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
    if( _timerList[ currentTimer ].placeHolder == 0 ){
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
      _threadList[j].placeHolder = 0; //Set as unactive.
      if( j < _firstThreadSpace ) { //If the function was before than first space,
        _firstThreadSpace = j; //Set it as first space.
      }
    }
  }
}

void TaskList::killTimer( void ( *timerAddress )( void ) ) //A function for removing a timer.
{ //This function works just like the same as before. ( removeThread )
  for( byte j = 0; j <= _lastTimerFunction; j++ ) {
    if( timerAddress == _timerList[j].functionPointer ) {
      _timerList[j].placeHolder = 0;
      if( j < _firstTimerSpace ) {
        _firstTimerSpace = j;
      }
    }
  }
}

boolean TaskList::isThreadRunning( void ( *threadAddress )( void ) ) //A function for checking the state of a thread.
{
  for( byte j = 0; j <= _lastThreadFunction; j++ ) { //Searching whole list for a match.
    if( threadAddress == _threadList[j].functionPointer ) { //If found,
    	return 1; //return 1.
    }
  }
  return 0; //If not found, return 0.
}

boolean TaskList::isTimerRunning( void ( *timerAddress )( void ) ) 
{ //This function works just like the same as before. ( isThreadRunning )
  for( byte j = 0; j <= _lastTimerFunction; j++ ) {
    if( timerAddress == _timerList[j].functionPointer ) {
    	return 1;
    }
  }
  return 0;
}

void TaskList::flushThreads( void )  //A function for deleting all threads.
{
  for( byte i = 0; i < MAXIMUM_THREADS; i++ ) { //Selecting all threads one by one,
    _threadList[i].functionPointer = NULL; //Making the function pointer as NULL,
    _threadList[i].placeHolder = 0; //Setting it as unactive,
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
    _timerList[i].placeHolder = 0; //Setting it as unactive,
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