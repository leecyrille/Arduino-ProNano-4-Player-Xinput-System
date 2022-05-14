# Arduino-4-Player-Xinput-System
A system that uses 5 Arduino Pro Nano to put 4 player arcade controls in order. Also provides admin buttons with keyboard output.

This scheme uses a 5th Arduino which just boots up 4 encoders in order with a delay between them. There is a 4 and 2 player button which can be used to start up only the middle 2 controllers, or all 4 in the correct order.

There are also other admin buttons that do things like volume control in windows, ALT+F4 for exiting programs, windows power button, keyboard shortcut for launching apps (setup in a windows shortcut for whatever app you want), disable all encoders to allow use of wireless gamepads.

The system works fairly well, but the players will be in the wrong order if you reboot windows. To get them in the right order again hit the 2 or 4 player button (if you hit the same mode twice it will do nothing, to prevent annoying accidentaly restarting twice).

It takes roughly 20 seconds for the players to up and running in the right order.

I am currently working on another project using 4 pico encoders which works almost instantly by sharing the controls over serial communication to the encoder in correct position, I think it will generally work better than this project, so I won't be working on this anymore. This is mostly here for future reference.

Please note that the diagrams make reference to receiving an incorrect position status from the other controllers to automatically trigger a restart sequence. This function would be very useful, but is not currently working (I did not finish figuring out the reported LED status in the Xinput library).

The folder with Serialandbuttonsequencer is the managing arduino code which is wired to the run pins on all the other encoders. In that folder you will also see some batch scripts which can change between 1 and 2 player mode. You can also use the batch script in your windows startup to put the players in the correct order after rebooting your computer. Check the port number of this arduino in device manager, and make the .bat files and device manager assigned port match (easier to change in windows, unless that port is unavailable).

My encoder software is setup with normal closed buttons because I was using some cheap EGstarts arcade buttons before, which I would not recomend. Look to David Madison's basic examples for your encoder firmware. They can be used with my management code since they are being rebooted by pulling down the run pin, which is only dependent on hardware.

Note for MAME users:
If you start up MAME without your mapped controllers in place, it will set the controls back to default. To avoid this, make your control config files read only once you have them setup. This is part of the reason I am working on the new serial control method with raspberry pi pico encoders.

The xinput library was created by David Madisson
https://github.com/dmadison/ArduinoXInput
