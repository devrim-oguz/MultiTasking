#include <MultiTasking.h>  //Including the MultiTasking library

void setup() {
  Serial.begin(9600);  //Starting serial interface at 9600 bauds
  superTask.setTimer(helloWorld, 1000, 0);  //Setting a timer to call hello world function every second forever
  superTask.startTasks();  //Starting the MultiTasking library
}

void loop() {
}

void helloWorld() {  //Defining a function named as "helloWorld"
  Serial.println("Hello, World!"); //Printing "Hello, World!" over serial
}