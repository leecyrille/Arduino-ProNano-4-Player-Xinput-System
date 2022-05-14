
/*
 *  Project     4 Player Arcade Cabinet with Xinput
 *  @author     Lee Maskell
 *  @link       xxxxxxx
 *  @license    MIT - Copyright (c) 2019 David Madison
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 *  Example:      Arcade encoder sequencer + Administration Buttons Rev0(Partially Untested)
 *  Description:  10 buttons are wired as inputs, and 5 outputs are used to switch 
 *                on and off 4 arcade encoders + an Xbox wireless receiver to get
 *                controls in the correct order. windows defines player order for
 *                xinput devices in the order they are added. To reset the order,
 *                all controllers must be disconnected at the same time. This scheme
 *                is also setup to allow all arcade encoders to be disabled and
 *                xbox wireless receiver be enabled for games which need gamepads.
 *  WIRING:                         
The outputs can be used to control a MOSFET board spliced into a USB cable to toggle the +5V line to encoder,
or wired to the reset pin on arduino based xinput encoders

+------------------------+------------+-------------+-----------------------------------------------------+
| Button (Left to Right) | Wire Color | Arduino Pin |                     Description                     |
+------------------------+------------+-------------+-----------------------------------------------------+
| 1                      | Brown      | 10          | Power                                               |
| 2                      | Red        | 16          | Exit (Alt+F4)                                       |
| 3                      | Orange     | 14          | Mute                                                |
| 4                      | Yellow     | 15          | Vol Down                                            |
| 5                      | Green      | 18 (A0)     | Vol Up                                              |
| 6                      | Blue       | 19 (A1)     | Pause                                               |
| 7                      | Violet     | 20 (A2)     | LaunchBox (Create Windows Shortcut Shift+LeftAlt+L) |
| 8                      | Gray       | 21 (A3)     | 2 player arcade                                     |
| 9                      | White      | 2           | 4 Player Arcade                                     |
| 10                     | Black      | 3           | Gamepads Only                                       |
|                        |            |             |                                                     |
| NA                     | NA         | 5           | Player 1 Encoder Reset Pin                          |
| NA                     | NA         | 6           | Player 2 Encoder Reset Pin                          |
| NA                     | NA         | 7           | Player 3 Encoder Reset Pin                          |
| NA                     | NA         | 8           | Player 4 Encoder Reset Pin                          |
| NA                     | NA         | 9           | Xbox Wireless Receiver Power                        |
+------------------------+------------+-------------+-----------------------------------------------------+

The player encoders are laid out as P3-P1-P2-P4 (typical for 4 player cabinets)

//WINDOWS BATCH FILES TO SEND SERIAL COMMANDS TO SET CORRECT CONTROLS FROM LAUNCHBOX (run as additional app)
//com port in batch files is for this Arduino's serial- edit sketch or COM port in device manager to match

  //USB_2_Player.bat (Only enable player 1 and 2 arcade controls, located in middle of 4 player panel)
    mode COM50 BAUD=9600 PARITY=n DATA=8 DTR=OFF
    set /p x="99,6,7,0,0,0" <nul >\\.\COM50

  //USB_4_Player.bat (enables 4 player arcade controls with order left to right, encoders setup as P3,P1,P2,P4)
    mode COM50 BAUD=9600 PARITY=n DATA=8 DTR=OFF
    set /p x="99,7,5,6,8,0" <nul >\\.\COM50

  //USB_Gamepads.bat (Disable all arcade controls and turn on xbox controller wireless receiver)
    mode COM50 BAUD=9600 PARITY=n DATA=8 DTR=OFF
    set /p x="99,9,0,0,0,0" <nul >\\.\COM50

 */

#include "muTimer.h"
#include "HID-Project.h"

muTimer button10timer = muTimer(); //Timers used to debounce button inputs
muTimer button16timer = muTimer();
muTimer button14timer = muTimer();
muTimer button15timer = muTimer();
muTimer button18timer = muTimer();
muTimer button19timer = muTimer();
muTimer button20timer = muTimer();
muTimer button21timer = muTimer();
muTimer button2timer = muTimer();
muTimer button3timer = muTimer();

muTimer state2timer = muTimer();
muTimer state3timer = muTimer();
muTimer state4timer = muTimer();
muTimer state5timer = muTimer();
muTimer state6timer = muTimer();

bool button10; //Variables to hold button readings
bool button16;
bool button14;
bool button15;
bool button18;
bool button19;
bool button20;
bool button21;
bool button2;
bool button3;

int controllerDelay = 4000;
int startOrder[] = {7,5,6,8,0};     //Define pins to be initialized at startup - 9 is xbox wireless receiver
int oldStartOrder[] = {0,0,0,0,0};  //Value to compare when new order needs to be executed
int setBoot = 0;                    //bit to set when writing new order with serial
int tempStore = 0;                  //Used to get values from array to digitalwrite functions (cannot directly use array to digital write)

bool setState0 = HIGH;               //States for player assignment sequence
bool setState1 = LOW;
bool setState2 = LOW;
bool setState3 = LOW;
bool setState4 = LOW;
bool setState5 = LOW;
bool setState6 = LOW;
bool setState7 = LOW;

void setup() {
  // initialize serial:
  Serial.begin(9600);  //Leave at 9600 for windows default settings
  Keyboard.begin();
  Consumer.begin();
  System.begin();
  
  pinMode(10, INPUT_PULLUP); //Power
  pinMode(16, INPUT_PULLUP); //Exit (alt+F4)- Change to ESC for Raspberry Pi/Linux Applications
  pinMode(14, INPUT_PULLUP); //Mute
  pinMode(15, INPUT_PULLUP); //Vol Down
  pinMode(18, INPUT_PULLUP); //Vol Up
  pinMode(19, INPUT_PULLUP); //Pause (P)
  pinMode(20, INPUT_PULLUP); //Launchbox (Left Shift+Left Alt+L)
  pinMode(21, INPUT_PULLUP); //2 Player Arcade (5,6)
  pinMode(2, INPUT_PULLUP);  //4 Player Arcade (7,5,6,8) 
  pinMode(3, INPUT_PULLUP);  //GamePad Only (9)

  pinMode(5, OUTPUT); // O X O O Reset button for encoder in "X" Postitions
  pinMode(6, OUTPUT); // O O X O
  pinMode(7, OUTPUT); // X O O O
  pinMode(8, OUTPUT); // O O O X
  pinMode(9, OUTPUT); // Xbox one wireless receiver

  digitalWrite(5, LOW); //Start with all devices turned off
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

  pinMode(13, OUTPUT); //Test LED
  digitalWrite(13, LOW);

} //END VOID SETUP

void loop() {
//READ BUTTON VALUES INTO VARIABLES
 button10 = !digitalRead(10);
 button16 = !digitalRead(16);
 button14 = !digitalRead(14);
 button15 = !digitalRead(15);
 button18 = !digitalRead(18);
 button19 = !digitalRead(19);
 button20 = !digitalRead(20);
 button21 = !digitalRead(21);
 button2  = !digitalRead(2);
 button3  = !digitalRead(3);
 
/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////EMULATED KEYBOARD OUTPUT/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
  switch (button10timer.delayOnOffTrigger(button10, 10, 10))
    {
      case 0: //Button Released
      //Nothing Needed
      break;

      case 1: //Button Pressed
      System.write(SYSTEM_SLEEP);
      Serial.println("Pin 10 button pressed - Power");
      break;
    }
///////////////////////////////////////////////////////////////////////////////////////////////
  switch (button16timer.delayOnOffTrigger(button16, 10, 10)) //ALT+F4 will close most programs in windows
    {
      case 0: //Button Released
      Keyboard.release(HID_KEYBOARD_LEFT_ALT);
      Keyboard.release(HID_KEYBOARD_F4);
      break;

      case 1: //Button Pressed
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);         //For retropie, change to KEY_ESC
      Keyboard.press(HID_KEYBOARD_F4);
      Serial.println("Pin 16 button pressed - Alt+F4");
      break;
    }
/////////////////////////////////////////////////////////////////////////////////////////////
  switch (button14timer.delayOnOffTrigger(button14, 10, 10))
    {
      case 0: //Button Released
      Consumer.release(MEDIA_VOLUME_MUTE);
      break;

      case 1: //Button Pressed
      Consumer.press(MEDIA_VOLUME_MUTE);
      Serial.println("Pin 14 button pressed - mute");
      break;
    }
///////////////////////////////////////////////////////////////////////////////////////////////
  switch (button15timer.delayOnOffTrigger(button15, 10, 10))
    {
      case 0: //Button Released
      Consumer.release(MEDIA_VOLUME_DOWN);
      break;

      case 1: //Button Pressed
      Consumer.press(MEDIA_VOLUME_DOWN);
      Serial.println("Pin 15 button pressed - volume down");
      break;
    }
///////////////////////////////////////////////////////////////////////////////////////////////
  switch (button18timer.delayOnOffTrigger(button18, 10, 10))
     {
      case 0: //Button Released
      Consumer.release(MEDIA_VOLUME_UP);
      break;

      case 1: //Button Pressed
      Consumer.press(MEDIA_VOLUME_UP);
      Serial.println("Pin 18 button pressed - volume up");
      break;
    }
///////////////////////////////////////////////////////////////////////////////////////////////
  switch (button19timer.delayOnOffTrigger(button19, 10, 10))    //Pause button for MAME
     {
      case 0: //Button Released
      Keyboard.release(KEY_P);
      break;

      case 1: //Button Pressed
      Keyboard.press(KEY_P);
      Serial.println("Pin 19 button pressed - P for pause");
      break;
    }
///////////////////////////////////////////////////////////////////////////////////////////////
  switch (button20timer.delayOnOffTrigger(button20, 10, 10)) //Create a windows shortcut for ALT+SHIFT+L
     {
      case 0: //Button Released
      Keyboard.release(HID_KEYBOARD_LEFT_ALT);
      Keyboard.release(HID_KEYBOARD_LEFT_SHIFT);
      Keyboard.release(KEY_L);
      break;

      case 1: //Button Pressed
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);         //For Launchbox/BigBox/Coinops
      Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
      Keyboard.press(KEY_L);
      Serial.println("Pin 20 button pressed - alt-shift-L for launchbox");
      break;
    }
///////////////////////////////////////////////////////////////////////////////////////////////
//BUTTON INPUT CONTROLLER ASSIGNMENT
///////////////////////////////////////////////////////////////////////////////////////////////
  if (button21timer.delayOnTrigger(button21, 10)){  //2 Player Arcade Controls  (P1=P1, P2=P2)
    setState0 = HIGH; //Put state machine back at beginning to interupt any ongoing assignment
    setState1 = LOW;
    setState2 = LOW;
    setState3 = LOW;
    setState4 = LOW;
    setState5 = LOW;
    setState6 = LOW;
    setState7 = LOW;
    startOrder[0] = 5;
    startOrder[1] = 6;
    startOrder[2] = 0;
    startOrder[3] = 0;
    startOrder[4] = 0;
    Serial.println("Pin 21 button pressed - 2 player");
    }
///////////////////////////////////////////////////////////////////////////////////////////////
  if (button2timer.delayOnTrigger(button2, 10)){  //4 Player Arcade Controls  (P3=P1, P1=P2, P2=P3, P4=P4)
    setState0 = HIGH; //Put state machine back at beginning to interupt any ongoing assignment
    setState1 = LOW;
    setState2 = LOW;
    setState3 = LOW;
    setState4 = LOW;
    setState5 = LOW;
    setState6 = LOW;
    setState7 = LOW;
    startOrder[0] = 7;
    startOrder[1] = 5;
    startOrder[2] = 6;
    startOrder[3] = 8;
    startOrder[4] = 0;
    Serial.println("Pin 2 button pressed - 4player");
    }
///////////////////////////////////////////////////////////////////////////////////////////////
  if (button3timer.delayOnTrigger(button3, 10)){  //Gamepads Only
    setState0 = HIGH; //Put state machine back at beginning to interupt any ongoing assignment
    setState1 = LOW;
    setState2 = LOW;
    setState3 = LOW;
    setState4 = LOW;
    setState5 = LOW;
    setState6 = LOW;
    setState7 = LOW;
    startOrder[0] = 9;
    startOrder[1] = 0;
    startOrder[2] = 0;
    startOrder[3] = 0;
    startOrder[4] = 0;
    Serial.println("Pin 3 button pressed - gamepads");
    }

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////RECEIVE SERIAL COMMAND FOR PLAYER SEQUENCE///////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

  // if there's any serial available, read it:
  while (Serial.available() > 0) {
      // look for the next valid integer in the incoming serial stream:
     setBoot = Serial.parseInt();
        if(setBoot == 99){      //if first value in string is 99, following values will set boot order
             setBoot = 0;          //reset the set bit
              for (int i = 0; i <5; i++){
                   startOrder[i] = Serial.parseInt();
                    if(startOrder[i] < 5 || startOrder[i] >9) startOrder[i] = 0;        //If the value for a boot sequence value is outside 2-6 (5 positions following the set bit), set it to 0 to prevent setting other pins  
              }
        }
      setState0 = HIGH;
      setState1 = LOW;
      setState2 = LOW;
      setState3 = LOW;
      setState4 = LOW;
      setState5 = LOW;
      setState6 = LOW;
      setState7 = LOW;
      Serial.println("Serial Received");
  }

///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////PLAYER ASSIGNMENT - STATE MACHINE/////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//The setState variables will be incremented to step through the sequence to turn on player
//controllers in the order desired.


///////////////////////////////////////SET STATE 0/////////////////////////////////////////////
//If a change to the start order is detected, and it is in setState 0, which is idle
  if(((startOrder[0] != oldStartOrder[0]) || (startOrder[1] != oldStartOrder[1]) || (startOrder[2] != oldStartOrder[2]) ||
    (startOrder[3] != oldStartOrder[3]) || (startOrder[4] != oldStartOrder[4])) && setState0 == HIGH){ 
       Serial.print("New startOrder:");
       Serial.print(startOrder[0]);
       Serial.print(" ");
       Serial.print(startOrder[1]);
       Serial.print(" ");
       Serial.print(startOrder[2]);
       Serial.print(" ");
       Serial.print(startOrder[3]);
       Serial.print(" ");
       Serial.println(startOrder[4]);
       setState0 = LOW;
       setState1 = HIGH;
   }
///////////////////////////////////////SET STATE 1/////////////////////////////////////////////
  if (setState1 == HIGH){
       digitalWrite(5, LOW);  //turn off all devices to allow windows to reset order
       digitalWrite(6, LOW);
       digitalWrite(7, LOW);
       digitalWrite(8, LOW);
       digitalWrite(9, LOW);
       Serial.println("All Outputs Set Low");
       setState1 = LOW;
       setState2 = HIGH;
       
  }
///////////////////////////////////////SET STATE 2/////////////////////////////////////////////
  if (state2timer.delayOnTrigger(setState2, 5000)){  //This is delay with all controllers off
      if(startOrder[0] != 0){
          tempStore = startOrder[0];
          digitalWrite(tempStore, HIGH);
          Serial.print("Pin ");
          Serial.print(tempStore);
          Serial.println(" set HIGH");
          setState2 = LOW;
          setState3 = HIGH;
          }
          else
          {
          setState2 = LOW;
          setState7 = HIGH;  
          }
  }
///////////////////////////////////////SET STATE 3/////////////////////////////////////////////
  if (state3timer.delayOnTrigger(setState3, controllerDelay)){
      if(startOrder[1] != 0){
          tempStore = startOrder[1];
          digitalWrite(tempStore, HIGH);
          Serial.print("Pin ");
          Serial.print(tempStore);
          Serial.println(" set HIGH");
          setState3 = LOW;
          setState4 = HIGH;
          }
          else
          {
          setState3 = LOW;
          setState7 = HIGH;  
          }
  }
///////////////////////////////////////SET STATE 4/////////////////////////////////////////////
  if (state4timer.delayOnTrigger(setState4, controllerDelay)){
      if(startOrder[2] != 0){
          tempStore = startOrder[2];
          digitalWrite(tempStore, HIGH);
          Serial.print("Pin ");
          Serial.print(tempStore);
          Serial.println(" set HIGH");
          setState4 = LOW;
          setState5 = HIGH;
          }
          else
          {
          setState4 = LOW;
          setState7 = HIGH;  
          }
  }
///////////////////////////////////////SET STATE 5/////////////////////////////////////////////
  if (state5timer.delayOnTrigger(setState5, controllerDelay)){
      if(startOrder[3] != 0){
          tempStore = startOrder[3];
          digitalWrite(tempStore, HIGH);
          Serial.print("Pin ");
          Serial.print(tempStore);
          Serial.println(" set HIGH");
          setState5 = LOW;
          setState6 = HIGH;
          }
          else
          {
          setState5 = LOW;
          setState7 = HIGH;  
          }
  }
///////////////////////////////////////SET STATE 6/////////////////////////////////////////////
  if (state6timer.delayOnTrigger(setState6, controllerDelay)){
     if(startOrder[4] != 0){
          tempStore = startOrder[4];
          digitalWrite(tempStore, HIGH);
          Serial.print("Pin ");
          Serial.print(tempStore);
          Serial.println(" set HIGH");
          setState6 = LOW;
          setState7 = HIGH;
          }
          else
          {
          setState6 = LOW;
          setState7 = HIGH;  
          }
  }
///////////////////////////////////////SET STATE 7/////////////////////////////////////////////
  if (setState7 == HIGH){
     oldStartOrder[0] = startOrder[0];     //Set old start order to allow comparison
     oldStartOrder[1] = startOrder[1];
     oldStartOrder[2] = startOrder[2];
     oldStartOrder[3] = startOrder[3];
     oldStartOrder[4] = startOrder[4];
     Serial.println("New Boot Order Execution Complete");
     setState7 = LOW;
     setState0 = HIGH;
     }



} //END VOID LOOP
