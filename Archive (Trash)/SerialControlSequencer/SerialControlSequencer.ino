/*
  Reading a serial ASCII-encoded string.

  This sketch demonstrates the Serial parseInt() function.
  It looks for an ASCII string of comma-separated values.
  It parses them into ints, and uses those to fade an RGB LED.

  Circuit: Common-Cathode RGB LED wired like so:
  - red anode: digital pin 3
  - green anode: digital pin 5
  - blue anode: digital pin 6
  - cathode: GND

  created 13 Apr 2012
  by Tom Igoe
  modified 14 Mar 2016
  by Arturo Guadalupi

  This example code is in the public domain.
*/


int startOrder[] = {6,7,5,8,9,10,11};    //Define pins to be initialized and default order
int setBoot = 0;                         //bit to set when writing new order

void setup() {
  // initialize serial:
  Serial.begin(9600);  //Leave at 9600 for windows default settings
  pinMode(13, OUTPUT); //Test LED
  pinMode(1, INPUT); //TEMPORARY - inputs 1-4 set as inputs to prevent accidental short since they are wired to output on arcade right now
  pinMode(2, INPUT); //TEMPORARY - inputs 1-4 set as inputs to prevent accidental short since they are wired to output on arcade right now
  pinMode(3, INPUT); //TEMPORARY - inputs 1-4 set as inputs to prevent accidental short since they are wired to output on arcade right now
  pinMode(4, INPUT); //TEMPORARY - inputs 1-4 set as inputs to prevent accidental short since they are wired to output on arcade right now
  digitalWrite(13, LOW);

  for (int i = 0; i < 7; i++) {                          //Set all pins in start order to high on initial startup
                pinMode(startOrder[i], OUTPUT);          // make the pins outputs:
                digitalWrite(startOrder[i], HIGH);       //make the pins start high
                Serial.print("Pin ");
                Serial.print(startOrder[i]);
                Serial.println(" set HIGH for startup");               
                }
  Serial.println("to set a boot order, enter first number as 99, then write output pin numbers ex. (99,2,3,4,5,6,7,8). Leave as zeros, to not turn back on");
  Serial.println();
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
     Serial.println("Serial Received");
     
      // look for the next valid integer in the incoming serial stream:
     setBoot = Serial.parseInt();
     if(setBoot == 99){      //if first value in string is 99, following values will set boot order
       setBoot = 0;          //reset the set bit
       for (int i = 0; i <7; i++){
           startOrder[i] = Serial.parseInt();
           if(startOrder[i] < 2 || startOrder[i] >8) startOrder[i] = 0;        //If the value for a boot sequence value is outside 2-8, set it to 0 to prevent setting other pins
           
       }

       Serial.println("New Boot Order Received");

       for (int i = 5; i < 12; i++) {    //set pins 5 to 11 low
                digitalWrite(i, LOW);
                Serial.print("Pin ");
                Serial.print(i);
                Serial.println(" set LOW");
                }

       delay(5000);                   //wait  seconds to allow windows to forget order of controllers
       
           for (int i = 0; i < 7; i++) {    //set pins high in the order of startOrder
               if(startOrder[i] != 0){
                digitalWrite(startOrder[i], HIGH);
                Serial.print("Pin ");
                Serial.print(startOrder[i]);
                Serial.println(" set HIGH");
                delay(1000);
                }
             }
     Serial.println("New Boot Order Execution Complete");
     }
     

       Serial.print("Boot Order: ");
       Serial.print(startOrder[0]);
       Serial.print(" ");
       Serial.print(startOrder[1]);
       Serial.print(" ");
       Serial.print(startOrder[2]);
       Serial.print(" ");
       Serial.print(startOrder[3]);
       Serial.print(" ");
       Serial.print(startOrder[4]);
       Serial.print(" ");
       Serial.print(startOrder[5]);
       Serial.print(" ");
       Serial.println(startOrder[6]);
       Serial.println("to set a boot order, enter first number as 99, then write output pin numbers ex. (99,2,3,4,5,6,7,8). Leave as zeros, to not turn back on");
    }

if(startOrder[0] == 6) digitalWrite(13, HIGH);  //Just for testing windows communication
if(startOrder[0] != 6) digitalWrite(13, LOW);

}
