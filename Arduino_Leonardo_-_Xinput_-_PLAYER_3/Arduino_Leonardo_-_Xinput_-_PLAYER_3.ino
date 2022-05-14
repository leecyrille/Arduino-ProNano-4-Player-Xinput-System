/*
 *  Project     Arduino - Xinput - Digital Input Analog Output
 *  @author     Lee Maskell
 *  @link       github.com/xxxxxxxxx
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
 * 
 *This is a modification of an Arduino X-Input Example designed for using arcade style controls with xinput output
 *
 *Requires Arduino with USB interface
 *
 *It is setup to have digital joystick wired to 4 pins, but it will output the left analog stick at 100% in given directions.
 *
 *All input pins should be wired to button, and then other terminal on button to ground.
 *
 *Install the X-input library:
 *https://www.arduino.cc/reference/en/libraries/xinput/
 *
 *Install The X-Input AVR board to Arduino IDE if using Arduino Pro Micro (ATmega32U4)
 *https://github.com/dmadison/ArduinoXInput_AVR
 *https://www.amazon.ca/gp/product/B07WPCLF8Y/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1
 *
 *Windows 10 will set player order according to sequence they are plugged in
 *
 *THIS PROJECT WAS CREATED USING THE FOLLOWING EXAMPLE:
 * *  Project     Arduino XInput Library
 * *  @author     David Madison
 * *  @link       github.com/dmadison/ArduinoXInput
 * *  @license    MIT - Copyright (c) 2019 David Madison

*/
#include <XInput.h>

// Setup

const int ADC_Max = 1023;  // 10 bit

// Joystick Input Pins
const int Pin_DpadUp    = 19;
const int Pin_DpadDown  = 18;
const int Pin_DpadLeft  = 21;
const int Pin_DpadRight = 20;

// Button Pins
const int Pin_ButtonA = 1;
const int Pin_ButtonB = 0;
const int Pin_ButtonX = 2;
const int Pin_ButtonY = 3;

const int Pin_ButtonLB = 4;
const int Pin_ButtonRB = 5;
const int Pin_TriggerL = 6;
const int Pin_TriggerR = 7;

const int Pin_ButtonBack  = 8;
const int Pin_ButtonStart = 9;

const int Pin_ButtonL3 = 10;
const int Pin_ButtonR3 = 16;

const int Pin_wrongPlayer = 14; //This pin to be jumpered to RST pin to fix player order

//Variables
int leftJoyX = 511; //Variable for Left Stick
int leftJoyY = 511; //Variable for Left Stick

void setup() {

	// Set buttons as inputs, using internal pull-up resistors
	pinMode(Pin_ButtonA, INPUT);
	pinMode(Pin_ButtonB, INPUT);
	pinMode(Pin_ButtonX, INPUT);
	pinMode(Pin_ButtonY, INPUT);

	pinMode(Pin_ButtonLB, INPUT);
	pinMode(Pin_ButtonRB, INPUT);
  pinMode(Pin_TriggerL, INPUT);
  pinMode(Pin_TriggerR, INPUT);

	pinMode(Pin_ButtonBack, INPUT);
	pinMode(Pin_ButtonStart, INPUT);

	pinMode(Pin_DpadUp, INPUT_PULLUP); //Converted to Analog Left Stick Later
	pinMode(Pin_DpadDown, INPUT_PULLUP);
	pinMode(Pin_DpadLeft, INPUT_PULLUP);
	pinMode(Pin_DpadRight, INPUT_PULLUP);

  pinMode(Pin_ButtonL3, INPUT_PULLUP);
  pinMode(Pin_ButtonR3, INPUT_PULLUP);

  pinMode(Pin_wrongPlayer, OUTPUT);
  digitalWrite(Pin_wrongPlayer, LOW);

	XInput.setJoystickRange(0, ADC_Max);  // Set joystick range to the ADC
	XInput.setAutoSend(false);  // Wait for all controls before sending

	XInput.begin();

}

void loop() {


	// Read pin values and store in variables
  boolean dpadUp    = !digitalRead(Pin_DpadUp);
  boolean dpadDown  = !digitalRead(Pin_DpadDown);
  boolean dpadLeft  = !digitalRead(Pin_DpadLeft);
  boolean dpadRight = !digitalRead(Pin_DpadRight);
	boolean buttonA = digitalRead(Pin_ButtonA);
	boolean buttonB = digitalRead(Pin_ButtonB);
	boolean buttonX = digitalRead(Pin_ButtonX);
	boolean buttonY = digitalRead(Pin_ButtonY);
	boolean buttonLB = digitalRead(Pin_ButtonLB);
	boolean buttonRB = digitalRead(Pin_ButtonRB);
	boolean buttonBack  = digitalRead(Pin_ButtonBack);
	boolean buttonStart = digitalRead(Pin_ButtonStart);
	boolean buttonL3 = !digitalRead(Pin_ButtonL3);
	boolean buttonR3 = !digitalRead(Pin_ButtonR3);



	// Send Button Variables to PC
	XInput.setButton(BUTTON_A, buttonA);
	XInput.setButton(BUTTON_B, buttonB);
	XInput.setButton(BUTTON_X, buttonX);
	XInput.setButton(BUTTON_Y, buttonY);
	XInput.setButton(BUTTON_LB, buttonLB);
	XInput.setButton(BUTTON_RB, buttonRB);
	XInput.setButton(BUTTON_BACK, buttonBack);
	XInput.setButton(BUTTON_START, buttonStart);
	XInput.setButton(BUTTON_L3, buttonL3);
	XInput.setButton(BUTTON_R3, buttonR3);

  // Set the triggers as if they were buttons
	boolean triggerLeft  = digitalRead(Pin_TriggerL);
	boolean triggerRight = digitalRead(Pin_TriggerR);
	XInput.setButton(TRIGGER_LEFT, triggerLeft);
	XInput.setButton(TRIGGER_RIGHT, triggerRight);

	// Set left joystick Variable
 if (dpadLeft == true)leftJoyX = 0;
 if (dpadRight == true)leftJoyX = 1023;
 if (dpadLeft == false && dpadRight == false)leftJoyX = 511;

 if (dpadDown == true)leftJoyY = 0;
 if (dpadUp == true)leftJoyY = 1023;
 if (dpadDown == false && dpadUp == false)leftJoyY = 511;

  //Write Joystick Variable to PC
  XInput.setJoystickX(JOY_LEFT, leftJoyX);
  XInput.setJoystickY(JOY_LEFT, leftJoyY);

  // Check for wrong player number - PLAYER 3
  uint8_t player = XInput.getPlayer();
  if (player == 0 || player == 3)digitalWrite(Pin_wrongPlayer, LOW); //0 = player not connected
  if (player == 1 || player == 2 || player ==4)digitalWrite(Pin_wrongPlayer, HIGH); //Send signal to seperate restarter board
  
	// Send control data to the computer
	XInput.send();
}
