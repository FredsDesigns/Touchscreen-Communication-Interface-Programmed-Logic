# Touchscreen-Communication-Interface-Programmed-Logic
Attached is the code used for the Two-way communication project, that has been compiled into one main file.

Key notes:
Microcontroller used - Arduino Uno WiFi Rev2
Display - TFT ILI9341
*The microcontroller used outputs 5V signals, make sure to use proper level shifters to shift down 5V's to 3.3V, otherwise if your board outputs 3.3V signals you may proceed and skip using level shifters*
Level Shifters - Two are needed to lower output voltages for the display to read in properly

Here is a realized look at the connections (Its a mess! My advice is to instead use a board that outputs 3v3, it heavily reduces the connections needed and work done!)

![TwoWayConnections](https://github.com/user-attachments/assets/f6c7f582-084a-46ed-8751-a0d6544c0626)

Since we are using an Arduino Uno WiFi Rev2 board, the supplied code can be run through the Arduino IDE.

Make note of one thing however! This display (TFT ILI9341) has custom libraries that this device uses, both it and the touchscreen detection library (XPT 2046) can be found easily in the Arduino IDE. If your not using the IDE make sure to verify if those libraries are supported, if not, you may have to find a workaround, as the code uses library specific commands quite often!

Also worth noting, if you do opt for a different display, since the code does use the ILI9341 library it also follows its dimensions (320x240) which you will need to accomodate for your new display.

Pinout used:
Chip Select (CS) [Pin 10]: Highlights device used by the microcontroller
Data/Command (DC) [Pin 9]: Indicates if data is being sent by the Serial Peripheral Interface (SPI)
MISO & T_DO [ISCP 1]: Collect data from Touchscreen and send to Arduino
MOSI & T_DIN [ISCP 4]: Collect data from Arduino and send to Touchscreen
SCK & T_CLK [ISCP 3]: Internal clock that share the same timing 
RESET [ISCP 5]: Used to synchronize with Arduino’s reset if needed
VCC and LED require 5V input, VCC handles the boards power, while LED controls what the board displays.

As I've shared before, this video by XTronical highlights the setup process extremely well! Please take a look if the connections are hard to follow, or if it is not displaying properly: https://youtu.be/rq5yPJbX_uk?si=M9JvL1F955ZWJjc-

Once the code and setup is complete, you can then compile and run. Both users (One using the touchscreen interface, the other using the serial monitor on a pc) can then start by typing there name and then sending messages between eachother. The user on the touchscreen will be able to view older messages, see what message they are currently typing, and have access to shifting and capitalization for extra keys / lettering. 

Heres a look at the menu you should see assuming all went correctly!
![TwoWayDisplay](https://github.com/user-attachments/assets/ca35106c-e191-43ba-8d88-f8718d2acf27)

