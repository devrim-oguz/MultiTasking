#include <MultiTasking.h>  //Adding the MultiTasking library.

TaskList myTasks; //Creating a TaskList named myTasks.

int LED = 13; //Defining LED pin as 13.
int analogPin = A0; //Defining analog input pin as A0.
int analogValue = 0; //Creating an integer to hold analog readings.

void setup() { //Setup Function
  Serial.begin(9600); //Starting serial in 9600 bauds.
  pinMode( LED, OUTPUT ); //Setting LED pin as OUTPUT.
  myTasks.setTimer( count, 1000, 10 ); //Adding a timer to call the function "count" every 1000 milisecond for 10 times.
  myTasks.setTimer( newTasks, 15000, 1 ); //Adding a timer to call the function "newTasks" just for 1 time.
                                          //Timer will start after 15000 miliseconds.( 15 second )
  myTasks.startTasks(); //Starting tasks.
  //No code will work underneath here until you stop MultiTasking.
  //Don't use any delay function or endless loop, because it will stop the other tasks.
  //Every code works one by one. So if you stop the flow of the code, the whole program will stop.
}

void loop() { //Loop function still works as a Thread. You can use it like a Thread.
}

void count() { //Declaring a function to print seconds until reset or start.
  Serial.println(millis()/1000); //Printing seconds.
}

void newTasks() { //Declaring a function named newTasks which will add new tasks to the TaskList.
  myTasks.addThread( measureAnalogValue ); //Adding a thread to run continuously withouth any delays.
  myTasks.setTimer( blinkLED, 1000, 0 ); //Calling blinkLED function every second, forever.( or until you stop them )
  myTasks.setTimer( sayHello, 5000, 0 ); //Calling sayHello function every 5 seconds, forever.
  myTasks.setTimer( printAnalogValue, 500, 0 ); //Calling printAnalogValue function every 500 milisecond.
}

void measureAnalogValue() { //Declaring a function to run continuously. It reads the analog pin and writes it to an int.
  analogValue = analogRead( analogPin ); //Reading analog pin...
}

void blinkLED() { //Declaring a function to blink a led.
  static int LED_State = 0; //Declaring a static variable to know led's state.
  if( LED_State == 0 ) { //If the led is off,
    digitalWrite( LED, HIGH ); //Make it on and
    LED_State = 1; //Save the state as on.
  }
  else { //If the led is on,
    digitalWrite( LED, LOW ); //Make it off and
    LED_State = 0; //Save the state as off.
  }
}

void sayHello() { //Declaring a function to print some string to serial.
  Serial.print( "Hello, Wolrd!    " ); //Printing some string
  Serial.print( "Millis: " );          //       .
  Serial.println( millis() );          //Printing millis.
}

void printAnalogValue() { //Declaring a function to print analogValue readed from the analog pin.
  Serial.print( "Analog Input: " ); //Printing some string
  Serial.println( analogValue ); //Printing analogValue.
}

//!!!Thank You For Using The Library!!! <3 <3 <3