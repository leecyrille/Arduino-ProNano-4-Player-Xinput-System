
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


#include "HID-Project.h"

int startOrder[] = {7,5,6,8,0};    //Define pins to be initialized and default order - 9 is xbox wireless receiver
int oldStartOrder[] = {0,0,0,0,0};   //Value to compare when new order needs to be executed
int setBoot = 0;                   //bit to set when writing new order with serial

void setup() {
  // initialize serial:
  Serial.begin(9600);  //Leave at 9600 for windows default settings
  
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

  pinMode(13, OUTPUT); //Test LED
  digitalWrite(13, LOW);

  for (int i = 0; i < 5; i++) {                          //Set all pins in start order on initial startup
                pinMode(startOrder[i], OUTPUT);          // make the pins outputs:
                digitalWrite(startOrder[i], LOW);        //make the pins start LOW             
                }
  Serial.println("Void Setup Complete: to set a boot order, enter first number as 99, then write output pin numbers ex. (99,5,6,7,8,9). Leave as zeros, or only enter 99,9 , to not turn back on others. Type 13 to to turn on pin 13 LED for testing");
  Serial.println();
  Keyboard.begin();
  Consumer.begin();
}

void loop() {
//EMULATED KEYBOARD OUTPUT
  if (digitalRead(10) == LOW){
    Keyboard.write(HID_KEYBOARD_POWER);
    delay(300);
  }
  
  if (digitalRead(16) == LOW){              //For Linux/Retropie, change this to simply Keyboard.write(KEY_ESC);
    Keyboard.press(HID_KEYBOARD_LEFT_ALT); 
    Keyboard.press(HID_KEYBOARD_F4);
    delay(300);
    Keyboard.releaseAll();
  }
  
  if (digitalRead(14) == LOW){
    Consumer.write(MEDIA_VOLUME_MUTE);
    delay(300);
  }
  
  if (digitalRead(15) == LOW){
    Consumer.write(MEDIA_VOLUME_UP);
    delay(300);
  }
  
  if (digitalRead(18) == LOW){
    Consumer.write(MEDIA_VOLUME_DOWN);
    delay(300);
  }
  
  if (digitalRead(19) == LOW){
    Keyboard.write(KEY_P);
    delay(300);
  }
  
  if (digitalRead(20) == LOW){              //Windows Shortcut Key for Launchbox
    Keyboard.press(HID_KEYBOARD_LEFT_ALT);
    Keyboard.press(HID_KEYBOARD_LEFT_SHIFT);
    delay(20);
    Keyboard.press(KEY_L);
    delay(280);
    Keyboard.releaseAll();
  }


//BUTTON INPUT CONTROLLER ASSIGNMENT
  if (digitalRead(21) == LOW){            //2 Player Arcade Controls  (P1=P1, P2=P2)
    startOrder[0] = 5;
    startOrder[1] = 6;
    startOrder[2] = 0;
    startOrder[3] = 0;
    startOrder[4] = 0;
    Serial.println("digitalRead(21) = LOW");
    delay(300);
  }
  
  if (digitalRead(2) == LOW){             //4 Player Arcade Controls  (P3=P1, P1=P2, P2=P3, P4=P4)
    startOrder[0] = 7;
    startOrder[1] = 5;
    startOrder[2] = 6;
    startOrder[3] = 8;
    startOrder[4] = 0;
    Serial.println("digitalRead(2) = LOW");
    delay(300);
  }
  
  if (digitalRead(3) == LOW){             //Gamepads Only
    startOrder[0] = 9;
    startOrder[1] = 0;
    startOrder[2] = 0;
    startOrder[3] = 0;
    startOrder[4] = 0;
    Serial.println("digitalRead(3) = LOW");
    delay(300);
  }


//SERIAL INPUT CONTROLLER ASSIGNMENT
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
         }
       
//EXECUTE NEW CONTROLLER ASSIGNMENT   
   if((startOrder[0] != oldStartOrder[0]) || (startOrder[1] != oldStartOrder[1]) || (startOrder[2] != oldStartOrder[2]) ||
                                             (startOrder[3] != oldStartOrder[3]) || (startOrder[4] != oldStartOrder[4])){ 
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

          for (int i = 5; i < 10; i++) {    //set pins 5 to 9 low
                digitalWrite(i, LOW);
                Serial.print("Pin ");
                Serial.print(i);
                Serial.println(" set LOW");
                delay(50);
                }

           delay(5000);                   //wait  seconds to allow windows to forget order of controllers
       
           for (int i = 0; i < 5; i++) {    //set pins high in the order of startOrder
               if(startOrder[i] != 0){
                digitalWrite(startOrder[i], HIGH);
                Serial.print("Pin ");
                Serial.print(startOrder[i]);
                Serial.println(" set HIGH");
                delay(4000);
                }
             }

       oldStartOrder[0] = startOrder[0];
       oldStartOrder[1] = startOrder[1];
       oldStartOrder[2] = startOrder[2];
       oldStartOrder[3] = startOrder[3];
       oldStartOrder[4] = startOrder[4];
       Serial.println("New Boot Order Execution Complete");
       Serial.print("oldStartOrder Set: ");
       Serial.print(oldStartOrder[0]);
       Serial.print(" ");
       Serial.print(oldStartOrder[1]);
       Serial.print(" ");
       Serial.print(oldStartOrder[2]);
       Serial.print(" ");
       Serial.print(oldStartOrder[3]);
       Serial.print(" ");
       Serial.println(oldStartOrder[4]);
       delay(1000);
      }
//For testing Windows Serial Commands, send "13" to turn on LED on pin 13
if(startOrder[0] == 13) digitalWrite(13, HIGH);  //Just for testing windows communication
if(startOrder[0] != 13) digitalWrite(13, LOW);

}
