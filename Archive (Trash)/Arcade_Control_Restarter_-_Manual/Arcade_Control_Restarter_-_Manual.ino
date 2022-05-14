
// constants won't change. They're used here to set pin numbers:
const int buttonPin1 = 1;         // the number of the pushbutton pin
const int buttonPin2 = 2;
const int buttonPin3 = 3;
const int buttonPin4 = 4;

const int ledPin1 = 5;  // the number of the LED pin
const int ledPin2 = 6;
const int ledPin3 = 7;
const int ledPin4 = 8;

// Variables will change:
int ledState1 = LOW;         // the current state of the output pin
int ledState2 = LOW;
int ledState3 = LOW;
int ledState4 = LOW;

int buttonState1;             // the current reading from the input pin
int buttonState2;
int buttonState3;
int buttonState4;

int lastButtonState1 = LOW;   // the previous reading from the input pin
int lastButtonState2 = LOW;
int lastButtonState3 = LOW;
int lastButtonState4 = LOW;

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime1 = 0;  // the last time the output pin was toggled
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;

unsigned long debounceDelay = 10000;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600); //debug
  
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);


  //Perform Initial Controller Sequenced Startup

    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);
    delay(5000);
    digitalWrite(ledPin1,HIGH);
    delay(1000);
    digitalWrite(ledPin2,HIGH);
    delay(1000);
    digitalWrite(ledPin3,HIGH);
    delay(1000);
    digitalWrite(ledPin4,HIGH);
}

void loop() {
  // read the state of the switch into a local variable:
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);
  int reading3 = digitalRead(buttonPin3);
  int reading4 = digitalRead(buttonPin4);

  Serial.print(reading1, BIN);
  Serial.print(reading2, BIN);
  Serial.print(reading3, BIN);
  Serial.println(reading4, BIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading1 != lastButtonState1) {
    // reset the debouncing timer
    lastDebounceTime1 = millis();
  }
  if (reading2 != lastButtonState2) {
    // reset the debouncing timer
    lastDebounceTime2 = millis();
  }
  if (reading3 != lastButtonState3) {
    // reset the debouncing timer
    lastDebounceTime3 = millis();
  }
  if (reading4 != lastButtonState4) {
    // reset the debouncing timer
    lastDebounceTime4 = millis();
  }



  
//Write inputs to state values if value is held for longer than debounce time delay
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    ledState1 = reading1;
  }
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    ledState2 = reading2;
  }
  if ((millis() - lastDebounceTime3) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    ledState3 = reading3;
  }
  if ((millis() - lastDebounceTime4) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    ledState4 = reading4;
  }


//If any of the inputs were reading high longer than the debounce time delay, begin the staggered reboot sequence by pulling RST pins on other controllers to GND and starting them up again in proper order.
  if (ledState1 == HIGH || ledState2 == HIGH || ledState3 == HIGH || ledState4 == HIGH){
  return;
}
  

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
  lastButtonState3 = reading3;
  lastButtonState4 = reading4;
}
