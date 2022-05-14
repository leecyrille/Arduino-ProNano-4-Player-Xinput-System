#include <Bounce2.h>

Bounce bounce1 = Bounce(); // Instantiate a Bounce object
Bounce bounce2 = Bounce();
Bounce bounce3 = Bounce();
Bounce bounce4 = Bounce();

int reset1state = LOW;
int reset2state = LOW;
int reset3state = LOW;
int reset4state = LOW;
int resetAction = LOW;

void setup() {

  Serial.begin(9600); //debug
  
  bounce1.attach(1,INPUT); // Attach the debouncer to a pin with INPUT mode
  bounce1.interval(10000); // Use a debounce interval of 20 seconds
  bounce2.attach(2,INPUT);
  bounce2.interval(10000);
  bounce3.attach(3,INPUT);
  bounce3.interval(10000);
  bounce4.attach(4,INPUT);
  bounce4.interval(10000);
  
  pinMode(5,OUTPUT); // Setup output pin to reset on player encoders
  digitalWrite(5,HIGH); // set reset output HIGH, which causes NO RESET
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  pinMode(7,OUTPUT);
  digitalWrite(7,HIGH);
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
}

void loop() {

   bounce1.update(); // Update the Bounce instance
   bounce2.update();
   bounce3.update();
   bounce4.update();
   reset1state = bounce1.read();
   reset2state = bounce2.read();
   reset3state = bounce3.read();
   reset4state = bounce4.read();

   Serial.println(digitalRead(1)+"  "+digitalRead(2)+"  "+digitalRead(3)+"  "+digitalRead(4));
   
  if ((reset1state = HIGH) || (reset2state = HIGH) || (reset3state = HIGH) || (reset4state = HIGH)){
    resetAction = HIGH;
    }
  else{
    resetAction = LOW;
    }

  if (resetAction = HIGH){
    //REBOOT SEQUENCE
    digitalWrite (5, LOW);
    digitalWrite (6, LOW);
    digitalWrite (7, LOW);
    digitalWrite (8, LOW);
    delay (2000);
    digitalWrite (5, HIGH);
    delay (500);
    digitalWrite (6, HIGH);
    delay (500);
    digitalWrite (7, HIGH);
    delay (500);
    digitalWrite (8, HIGH);
    delay (20000);
    resetAction = LOW;
  }
}
