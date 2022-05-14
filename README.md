# Arduino-4-Player-Xinput-System
A system that uses 5 Arduino Pro Nano to put 4 player arcade controls in order. Also provides admin buttons with keyboard output.

This scheme uses a 5th Arduino which just boots up 4 encoders in order with a delay between them. There is a 4 and 2 player button which can be used to start up only the middle 2 controllers, or all 4 in the correct order.

There are also other admin buttons that do things like volume control in windows, ALT+F4 for exiting programs, windows power button, keyboard shortcut for launching apps (setup in a windows shortcut for whatever app you want), disable all encoders to allow use of wireless gamepads.

The system works fairly well, but the players will be in the wrong order if you reboot windows. To get them in the right order again hit the 2 or 4 player button (if you hit the same mode twice it will do nothing, to prevent annoying accidentaly restarting twice).

It takes roughly 20 seconds for the players to up and running in the right order.

I am currently working on another project using 4 pico encoders which works almost instantly by sharing the controls over serial communication to the encoder in correct position, I think it will generally work better than this project, so I won't be working on this anymore. This is here for future reference mostly.
